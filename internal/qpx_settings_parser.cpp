#include "internal/qpx_settings_parser.h"

#include "QPxCore/QPxSettings.h"

#include "pcx/str.h"
#include "pcx/join_str.h"

#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/QTextStream>

namespace
{

struct Base64
{
    QString text;
};

class Token
{
public:
    enum class Type { Id, Int, Float, Double, String, Base64, Lb, Rb, Eq, Comma, True, False, Eof };

    Type type;
    QString text;
};

class Source
{
public:
    explicit Source(const QString &text) : text(text), pos(0) { }

    QChar get(){ return pos < text.length() ? text[pos++] : QChar(); }
    void unget(){ if(pos) --pos; }

private:
    QString text;
    int pos;
};

QChar skip(Source &is)
{
    QChar c = is.get();
    while(c.isSpace()) c = is.get();

    return c;
}

bool validId(QChar c)
{
    return !c.isSpace() && QString("-={},#").indexOf(c) < 0;
}

QChar getWhile(Source &is, QString &s, bool(*f)(QChar))
{
    QChar c = is.get();
    while(f(c))
    {
        s += c;
        c = is.get();
    }

    return c;
}

Token getString(Source &is, QString &s)
{
    QChar c = is.get();
    while(c != '\"')
    {
        if(c == '\n' || c.isNull()) throw std::runtime_error("non-terminated string");

        s += c;
        c = is.get();
    }

    c = skip(is);
    if(c == '\"')
    {
        return getString(is, s);
    }

    is.unget();
    return { Token::Type::String, s };
}

class Scanner
{
public:
    explicit Scanner(const QString &source) : is(source) { }

    Token next(bool get);
    Token match(Token::Type type, bool get);

    Token token() const { return t; }

private:
    Token nextToken();

    Source is;
    Token t;
};

Token Scanner::next(bool get)
{
    if(get) t = nextToken();
    return t;
}

Token Scanner::match(Token::Type type, bool get)
{
    next(get);
    if(t.type != type)
    {
        throw std::runtime_error(pcx::str("unexpected token type - ", t.text.toStdString()));
    }

    return t;
}

Token Scanner::nextToken()
{
    QChar c = skip(is);

    if(c.isNull()) return { Token::Type::Eof, "EOF" };

    if(c == '{') return { Token::Type::Lb, QString(c) };
    if(c == '}') return { Token::Type::Rb, QString(1, c) };
    if(c == '=') return { Token::Type::Eq, QString(1, c) };
    if(c == ',') return { Token::Type::Comma, QString(1, c) };

    if(c == '\"')
    {
        QString s;
        return getString(is, s);
    }

    if(c == '#')
    {
        QString s;
        getWhile(is, s, [](QChar c){ return c.isLetterOrNumber() || c == '+' || c == '/' || c == '='; });

        is.unget();
        return { Token::Type::Base64, s };
    }

    if(c.isDigit() || c == '-')
    {
        QString s;

        s += c;
        c = getWhile(is, s, [](QChar c) -> bool { return c.isDigit(); });

        if(s == "-") throw std::runtime_error("invalid number");

        if(c == '.')
        {
            s += c;;
            c = getWhile(is, s, [](QChar c) -> bool { return c.isDigit(); });

            if(c == 'f')
            {
                return { Token::Type::Float, s };
            }

            is.unget();
            return { Token::Type::Double, s };
        }

        is.unget();
        return { Token::Type::Int, s };
    }

    if(validId(c))
    {
        QString s;

        s += c;
        getWhile(is, s, [](QChar c){ return validId(c) || c.isDigit(); });

        is.unget();

        if(s == "true") return { Token::Type::True, s };
        if(s == "false") return { Token::Type::False, s };

        return { Token::Type::Id, s };
    }

    throw std::runtime_error("invalid character");
}

class Context
{
public:
    explicit Context(const QString &text) : scanner(text) { }

    Scanner scanner;
};

QVariant next(Context &c, const QVariant &value)
{
    c.scanner.next(true);
    return value;
}

QVariant base64(Context &c, const QString &b64)
{
    auto ba = QByteArray::fromBase64(b64.toLocal8Bit());
    QDataStream ds(ba);

    QVariant v;
    ds >> v;

    c.scanner.next(true);
    return v;
}

void stringlistItem(Context &c, QStringList &values, bool get)
{
    auto tok = c.scanner.match(Token::Type::String, get);
    values.append(tok.text);

    tok = c.scanner.next(true);
    if(tok.type == Token::Type::Comma)
    {
        stringlistItem(c, values, true);
    }
}

QVariant stringlist(Context &c)
{
    QStringList values;

    auto tok = c.scanner.next(true);
    if(tok.type != Token::Type::Rb)
    {
        stringlistItem(c, values, false);
    }

    c.scanner.match(Token::Type::Rb, false);
    c.scanner.next(true);

    return values;
}

QVariant expr(Context &c, bool get)
{
    auto tok = c.scanner.next(get);

    switch(tok.type)
    {
        case Token::Type::Int: return next(c, tok.text.toInt());
        case Token::Type::Float: return next(c, tok.text.toFloat());
        case Token::Type::Double: return next(c, tok.text.toDouble());
        case Token::Type::String: return next(c, tok.text);
        case Token::Type::Base64: return base64(c, tok.text);
        case Token::Type::True: return next(c, true);
        case Token::Type::False: return next(c, false);
        case Token::Type::Lb: return stringlist(c);

        default: return QVariant();
    }
}

void construct(Context &c, QPx::Settings &parent, bool get)
{
    auto key = c.scanner.match(Token::Type::Id, get).text;
    auto &node = parent.append(key);

    auto tok = c.scanner.next(true);
    if(tok.type == Token::Type::Eq)
    {
        node.setValue(expr(c, true));
    }

    if(c.scanner.token().type == Token::Type::Lb)
    {
        c.scanner.next(true);
        while(c.scanner.token().type != Token::Type::Rb)
        {
            construct(c, node, false);
        }

        c.scanner.next(true);
    }
}

QString toStringList(const QStringList &values)
{
    if(values.isEmpty())
    {
        return "{ }";
    }

    QString result;
    QTextStream ts(&result);

    ts << "{ " << pcx::join_str(values, ", ", [](QString s){ return pcx::str("\"", s.toStdString(), "\""); }).c_str() << " }";

    return result;
}

QString toBase64(const QVariant &value)
{
    QByteArray ba;
    QDataStream ds(&ba, QIODevice::WriteOnly);

    ds << value;

    return QString::fromLocal8Bit(ba.toBase64());
}

void write(int indent, const QPx::Settings &parent, const QPx::Settings &node, int index, QTextStream &ts)
{
    auto ind = QString(indent * 4, ' ');

    ts << ind << node.key();

    if(node.value().isValid())
    {
        ts << " = ";

        auto v = node.value();
        switch(static_cast<QMetaType::Type>(v.type()))
        {
            case QMetaType::Int: ts << v.toInt(); break;
            case QMetaType::Float: ts << v.toFloat(); break;
            case QMetaType::Double: ts << v.toDouble(); break;
            case QMetaType::QString: ts << "\"" << v.toString() << "\""; break;
            case QMetaType::QStringList: ts << toStringList(v.toStringList()); break;
            case QMetaType::Bool: ts << (v.toBool() ? "true" : "false"); break;

            default: ts << "#" << toBase64(v);
        }
    }

    ts << "\n";

    if(node.count())
    {
        ts << ind << "{\n";

        for(int i = 0; i < node.count(); ++i)
        {
            write(indent + 1, node, node[i], i, ts);
        }

        ts << ind << "}\n";

        if(index < parent.count() - 1)
        {
            ts << "\n";
        }
    }

    if(!node.count() && index < parent.count() - 1 && parent[index + 1].count())
    {
        ts << "\n";
    }
}

}

void qpx_settings_parse(QPx::Settings &root, const QString &source)
{
    Context c(source);

    c.scanner.next(true);
    while(c.scanner.token().type != Token::Type::Eof)
    {
        construct(c, root, false);
    }
}

QString qpx_settings_write(const QPx::Settings &root)
{
    QString result;
    QTextStream ts(&result);

    for(int i = 0; i < root.count(); ++i)
    {
        write(0, root, root[i], i, ts);
    }

    return result;
}

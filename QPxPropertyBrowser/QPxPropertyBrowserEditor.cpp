#include "QPxPropertyBrowser/QPxPropertyBrowserEditor.h"

#include "QPxPropertyBrowserType.h"

#include "internal/qpx_combo_box.h"

#include <QtCore/QTimer>

#include <QtGui/QIntValidator>
#include <QtGui/QDoubleValidator>

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>

namespace
{

class EnumCache
{
public:
    EnumCache(const QPx::AbstractEnumPropertyBrowserType *type, QWidget *parent) : type(type), combo(new qpx_combo_box(parent)) { }

    const QPx::AbstractEnumPropertyBrowserType *type;
    QComboBox *combo;
};

template<typename T, typename V> V *setupValidator(const QVariant &min, const QVariant &max, V *validator)
{
    if(min.isValid()) validator->setBottom(qvariant_cast<T>(min));
    if(max.isValid()) validator->setTop(qvariant_cast<T>(max));

    return validator;
}

template<typename T> T fromString(const QString &value, bool &ok);

template<> int fromString(const QString &value, bool &ok){ return value.toInt(&ok); }
template<> float fromString(const QString &value, bool &ok){ return value.toFloat(&ok); }

template<typename T> QVariant lineEditValue(const QString &text)
{
    bool ok = false;
    T value = fromString<T>(text, ok);

    return ok ? value : QVariant();
}

}

QPx::PropertyBrowserEditor::PropertyBrowserEditor(QWidget *parent) : QWidget(parent)
{
}

QPx::StringPropertyBrowserEditor::StringPropertyBrowserEditor(QWidget *parent) : PropertyBrowserEditor(parent)
{
    auto &edit = cache.alloc<QLineEdit*>();
    edit = new QLineEdit(this);

    auto layout = new QVBoxLayout(this);
    layout->setMargin(0);

    layout->addWidget(edit);
    setFocusProxy(edit);
}

QVariant QPx::StringPropertyBrowserEditor::value() const
{
    return cache.get<QLineEdit*>()->text();
}

void QPx::StringPropertyBrowserEditor::setValue(const QVariant &value)
{
    cache.get<QLineEdit*>()->setText(value.isValid() ? value.toString() : QString());
}

QPx::IntPropertyBrowserEditor::IntPropertyBrowserEditor(const QVariant &min, const QVariant &max, QWidget *parent) : StringPropertyBrowserEditor(parent)
{
    cache.get<QLineEdit*>()->setValidator(setupValidator<int>(min, max, new QIntValidator(this)));
}

QVariant QPx::IntPropertyBrowserEditor::value() const
{
    return lineEditValue<int>(cache.get<QLineEdit*>()->text());
}

QPx::FloatPropertyBrowserEditor::FloatPropertyBrowserEditor(const QVariant &min, const QVariant &max, QWidget *parent) : StringPropertyBrowserEditor(parent)
{
    cache.get<QLineEdit*>()->setValidator(setupValidator<float>(min, max, new QDoubleValidator(this)));
}

QVariant QPx::FloatPropertyBrowserEditor::value() const
{
    return lineEditValue<float>(cache.get<QLineEdit*>()->text());
}

void QPx::FloatPropertyBrowserEditor::setValue(const QVariant &value)
{
    cache.get<QLineEdit*>()->setText(QString::number(value.toFloat()));
}

QPx::EnumPropertyBrowserEditor::EnumPropertyBrowserEditor(const AbstractEnumPropertyBrowserType *type, const QMap<int, QString> &values, QWidget *parent) : PropertyBrowserEditor(parent)
{
    auto &c = cache.alloc<EnumCache>(type, this);

    auto layout = new QVBoxLayout(this);
    layout->setMargin(0);

    foreach(int i, values.keys())
    {
        c.combo->addItem(values[i], i);
    }

    layout->addWidget(c.combo);
    setFocusProxy(c.combo);

    connect(c.combo, SIGNAL(currentIndexChanged(int)), SIGNAL(commit()));

    QTimer::singleShot(0, static_cast<qpx_combo_box*>(c.combo), SLOT(open()));
}

QVariant QPx::EnumPropertyBrowserEditor::value() const
{
    auto &c = cache.get<EnumCache>();
    return c.type->toEnumValue(c.combo->itemData(c.combo->currentIndex()));
}

void QPx::EnumPropertyBrowserEditor::setValue(const QVariant &value)
{
    auto &c = cache.get<EnumCache>();

    for(int i = 0; i < c.combo->count(); ++i)
    {
        if(c.combo->itemData(i) == value.toInt())
        {
            c.combo->setCurrentIndex(i);
            return;
        }
    }

    c.combo->setCurrentIndex(-1);
}

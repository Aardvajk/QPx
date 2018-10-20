#ifndef QPX_PROPERTY_H
#define QPX_PROPERTY_H

#include <pcx/aligned_store.h>
#include <pcx/flags.h>

#include <QtCore/QObject>
#include <QtCore/QVariant>

namespace QPx
{

class Property : public QObject
{
    Q_OBJECT

public:
    enum class Flag : std::uint8_t
    {
        ReadOnly = 1,
        Hidden = 2
    };

    using Flags = pcx::flags<Flag>;

    explicit Property(QObject *parent = nullptr);
    Property(const QString &name, Flags flags, const QVariant &value, QObject *parent = nullptr);

    template<typename T> void setValue(const T &t){ setVariant(QVariant::fromValue(t)); }

    QString name() const;
    Flags flags() const;
    QVariant variant() const;

    template<typename T> T value() const { return qvariant_cast<T>(variant()); }

public slots:
    void setName(const QString &value);
    void setFlags(const Flags &flags);
    void setVariant(const QVariant &value);

private:
    pcx::aligned_store<32> cache;
};

}

template<> struct pcx_is_flag_enum<QPx::Property::Flag> : std::true_type { };

#endif // QPX_PROPERTY_H

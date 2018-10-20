#ifndef QPX_SETTINGS_H
#define QPX_SETTINGS_H

#include <QtCore/QString>
#include <QtCore/QVariant>

#include <pcx/aligned_store.h>

namespace QPx
{

class Settings
{
public:
    void setValue(const QVariant &value);

    Settings &append(const QString &key);
    Settings &operator[](const QString &key);

    Settings &operator[](int index);
    const Settings &operator[](int index) const;

    QString key() const;
    QVariant value(const QVariant &defaultValue = { }) const;

    int count() const;

private:
    friend class SettingsMap;

    Settings(const QString &key);

    pcx::aligned_store<104> cache;
};

class SettingsMap : public Settings
{
public:
    explicit SettingsMap(const QString &path = { });

    void sync();

private:
    pcx::aligned_store<8> cache;
};

}

#endif // QPX_SETTINGS_H

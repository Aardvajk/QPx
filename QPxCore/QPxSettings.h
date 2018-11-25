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
    Settings();

    void setValue(const QVariant &value);

    bool load(const QString &path);
    bool save(const QString &path) const;

    Settings &append(const QString &key);

    Settings &operator[](const QString &key);
    const Settings &operator[](const QString &key) const;

    Settings &operator[](int index);
    const Settings &operator[](int index) const;

    QString key() const;
    QVariant value(const QVariant &defaultValue = { }) const;

    int count() const;

private:
    friend class SettingsRoot;
    friend class SettingsMap;

    Settings(const QString &key);

    pcx::aligned_store<104> cache;
};

class SettingsMap : public Settings
{
public:
    explicit SettingsMap(const QString &path = { });

    void sync() const;

private:
    pcx::aligned_store<8> cache;
};

}

#endif // QPX_SETTINGS_H

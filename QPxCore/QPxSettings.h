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

    void clear();
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
    friend class ApplicationSettings;

    Settings(const QString &key);

    pcx::aligned_store<104> cache;
};

class ApplicationSettings : public Settings
{
public:
    explicit ApplicationSettings(const QString &path = { });

    void sync() const;

private:
    pcx::aligned_store<8> cache;
};

}

#endif // QPX_SETTINGS_H

#include "QPxCore/QPxSettings.h"

#include "internal/qpx_settings_parser.h"

#include <pcx/ptr_vector.h>

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include <QtCore/QCoreApplication>

#include <vector>
#include <unordered_map>

namespace
{

class Proxy
{
public:
    Proxy(const QString &s) : s(&s) { }
    bool operator==(const Proxy &p) const { return *s == *(p.s); }

    const QString *s;
};

struct ProxyHash
{
    std::size_t operator()(const Proxy &p) const { return std::hash<std::string>()(p.s->toStdString()); }
};

class SettingsCache
{
public:
    explicit SettingsCache(QString key) : key(std::move(key)) { }

    QString key;
    std::unordered_map<Proxy, std::size_t, ProxyHash> map;
    pcx::ptr_vector<QPx::Settings> nodes;
    QVariant value;
};

class MapCache
{
public:
    explicit MapCache(const QString &path) : path(path) { }

    QString path;
};

}

QPx::Settings::Settings()
{
    cache.alloc<SettingsCache>(QString());
}

void QPx::Settings::clear()
{
    auto &c = cache.get<SettingsCache>();

    c.map.clear();
    c.nodes.clear();
}

void QPx::Settings::setValue(const QVariant &value)
{
    auto &v = cache.get<SettingsCache>().value;
    if(v != value)
    {
        v = value;
        emit valueChanged(v);
    }
}

bool QPx::Settings::load(const QString &path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    qpx_settings_parse(*this, QString::fromUtf8(file.readAll()));
    return true;
}

bool QPx::Settings::save(const QString &path) const
{
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }

    file.write(qpx_settings_write(*this).toUtf8());
    return true;
}

QPx::Settings &QPx::Settings::append(const QString &key)
{
    auto &c = cache.get<SettingsCache>();

    if(c.map.find(key) == c.map.end())
    {
        return (*this)[key];
    }

    c.nodes.push_back(new QPx::Settings(key));
    return c.nodes.back();
}

QPx::Settings &QPx::Settings::operator[](const QString &key)
{
    auto &c = cache.get<SettingsCache>();

    auto i = c.map.find(key);
    if(i == c.map.end())
    {
        c.nodes.push_back(new QPx::Settings(key));
        c.map.insert({ c.nodes.back().cache.get<SettingsCache>().key, c.nodes.size() - 1});

        return c.nodes.back();
    }

    return c.nodes[i->second];
}

const QPx::Settings &QPx::Settings::operator[](const QString &key) const
{
    auto &c = cache.get<SettingsCache>();

    auto i = c.map.find(key);
    if(i == c.map.end())
    {
        static const Settings null;
        return null;
    }

    return c.nodes[i->second];
}

QPx::Settings &QPx::Settings::operator[](int index)
{
    return cache.get<SettingsCache>().nodes[static_cast<std::size_t>(index)];
}

const QPx::Settings &QPx::Settings::operator[](int index) const
{
    return cache.get<SettingsCache>().nodes[static_cast<std::size_t>(index)];
}

QString QPx::Settings::key() const
{
    return cache.get<SettingsCache>().key;
}

int QPx::Settings::count() const
{
    return static_cast<int>(cache.get<SettingsCache>().nodes.size());
}

QPx::Settings::Settings(const QString &key)
{
    cache.alloc<SettingsCache>(key);
}

QVariant QPx::Settings::variant(const QVariant &defaultValue) const
{
    auto &value = cache.get<SettingsCache>().value;
    return value.isValid() ? value : defaultValue;
}

QPx::ApplicationSettings::ApplicationSettings(const QString &path) : QPx::Settings()
{
    auto &c = cache.alloc<MapCache>(path.isEmpty() ? QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + QCoreApplication::applicationName() + ".qps" : path);
    load(c.path);
}

void QPx::ApplicationSettings::sync() const
{
    QDir dir(QFileInfo(cache.get<MapCache>().path).dir());
    if(!dir.exists())
    {
        dir.mkpath(dir.path());
    }

    if(!save(cache.get<MapCache>().path))
    {
        throw std::runtime_error("unable to sync settings");
    }
}


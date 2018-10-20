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

class Cache
{
public:
    Cache(QString key) : key(std::move(key)) { }

    QString key;
    std::unordered_map<Proxy, std::size_t, ProxyHash> map;
    pcx::ptr_vector<QPx::Settings> nodes;
    QVariant value;
};

}

void QPx::Settings::setValue(const QVariant &value)
{
    cache.get<Cache>().value = value;
}

QPx::Settings &QPx::Settings::append(const QString &key)
{
    auto &c = cache.get<Cache>();

    if(c.map.find(key) == c.map.end())
    {
        return (*this)[key];
    }

    c.nodes.push_back(new QPx::Settings(key));
    return c.nodes.back();
}

QPx::Settings &QPx::Settings::operator[](const QString &key)
{
    auto &c = cache.get<Cache>();

    auto i = c.map.find(key);
    if(i == c.map.end())
    {
        c.nodes.push_back(new QPx::Settings(key));
        c.map.insert({ c.nodes.back().cache.get<Cache>().key, c.nodes.size() - 1});

        return c.nodes.back();
    }

    return c.nodes[i->second];
}

QPx::Settings &QPx::Settings::operator[](int index)
{
    return cache.get<Cache>().nodes[static_cast<std::size_t>(index)];
}

const QPx::Settings &QPx::Settings::operator[](int index) const
{
    return cache.get<Cache>().nodes[static_cast<std::size_t>(index)];
}

QString QPx::Settings::key() const
{
    return cache.get<Cache>().key;
}

QVariant QPx::Settings::value(const QVariant &defaultValue) const
{
    auto &value = cache.get<Cache>().value;

    return value.isValid() ? value : defaultValue;
}

int QPx::Settings::count() const
{
    return static_cast<int>(cache.get<Cache>().nodes.size());
}

QPx::Settings::Settings(const QString &key)
{
    cache.alloc<Cache>(key);
}

QPx::SettingsMap::SettingsMap(const QString &path) : QPx::Settings({ })
{
    cache.alloc<QString>(path.isEmpty() ? QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + QCoreApplication::applicationName() + ".qps" : path);

    QFile file(cache.get<QString>());
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qpx_settings_parse(*this, QString::fromUtf8(file.readAll()));
    }
}

void QPx::SettingsMap::sync()
{
    QDir dir(QFileInfo(cache.get<QString>()).dir());
    if(!dir.exists())
    {
        dir.mkpath(dir.path());
    }

    QFile file(cache.get<QString>());
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        throw std::runtime_error("unable to sync settings");
    }

    file.write(qpx_settings_write(*this).toUtf8());
}

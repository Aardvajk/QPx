#include "QPxProperties/QPxPropertyMap.h"

#include "QPxProperties/QPxProperty.h"

#include <pcx/ptr_vector.h>

#include <vector>
#include <unordered_map>

namespace
{

class Cache
{
public:
    pcx::ptr_vector<QPx::Property> v;
    std::unordered_map<uint, std::vector<std::size_t> > m;
};

}

QPx::PropertyMap::PropertyMap()
{
    cache.alloc<Cache>();
}

void QPx::PropertyMap::clear()
{
    cache.get<Cache>().v.clear();
    cache.get<Cache>().m.clear();
}

QPx::Property &QPx::PropertyMap::operator[](const QString &name)
{
    auto &c = cache.get<Cache>();
    auto &bin = c.m[qt_hash(name)];

    for(auto i: bin)
    {
        if(c.v[i].name() == name) return c.v[i];
    }

    bin.push_back(c.v.size());
    c.v.push_back(new Property(name, Property::Flags(), QVariant()));

    return c.v.back();
}

QPx::Property &QPx::PropertyMap::operator[](int index)
{
    return cache.get<Cache>().v[static_cast<std::size_t>(index)];
}

const QPx::Property &QPx::PropertyMap::operator[](int index) const
{
    return cache.get<Cache>().v[static_cast<std::size_t>(index)];
}

int QPx::PropertyMap::count() const
{
    return static_cast<int>(cache.get<Cache>().v.size());
}

#ifndef QPXPROPERTYMAP_H
#define QPXPROPERTYMAP_H

#include <pcx/aligned_store.h>

#include <QtCore/QString>

namespace QPx
{

class Property;

class PropertyMap
{
public:
    PropertyMap();

    void clear();

    Property &operator[](const QString &name);

    Property &operator[](int index);
    const Property &operator[](int index) const;

    int count() const;

private:
    pcx::aligned_store<80> cache;
};

}

#endif // QPXPROPERTYMAP_H

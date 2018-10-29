#include "QPxPropertyBrowserModel.h"

#include "QPxProperties/QPxPropertyBrowserItem.h"

#include <pcx/scoped_lock.h>

#include <QtCore/QMap>
#include <QtCore/QPersistentModelIndex>

namespace
{

class Cache
{
public:
    Cache() : valueChangeLock(false) { }

    QMap<QPx::PropertyBrowserItem*, QPersistentModelIndex> map;
    bool valueChangeLock;
};

}

QPx::PropertyBrowserModel::PropertyBrowserModel(QObject *parent) : TreeModel(parent)
{
    cache.alloc<Cache>();
}

QModelIndex QPx::PropertyBrowserModel::appendItem(PropertyBrowserItem *item, const QModelIndex &parent)
{
    auto index = appendRow(item, parent);
    cache.get<Cache>().map[item] = index;

    connect(item, SIGNAL(valueChanged(QVariant)), SLOT(valueChanged(QVariant)));
    return index;
}

Qt::ItemFlags QPx::PropertyBrowserModel::flags(const QModelIndex &index) const
{
    auto value = TreeModel::flags(index);

    if(auto item = static_cast<const PropertyBrowserItem*>(userData(index)))
    {
        if(index.column() == 1 && !rowCount(index))
        {
            value |= Qt::ItemIsEditable;
        }
    }

    return value;
}

QVariant QPx::PropertyBrowserModel::data(const QModelIndex &index, int role) const
{
    if(auto item = static_cast<const PropertyBrowserItem*>(userData(index)))
    {
        if(role == Qt::DisplayRole)
        {
            switch(index.column())
            {
                case 0: return item->name();
                case 1: return item->valueText();
            }
        }
    }

    return QVariant();
}

bool QPx::PropertyBrowserModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(auto item = static_cast<PropertyBrowserItem*>(userData(index)))
    {
        if(role == Qt::EditRole)
        {
            auto lock = pcx::scoped_lock(cache.get<Cache>().valueChangeLock);

            item->setValue(value);

            auto i = index;
            while(i.isValid())
            {
                emit dataChanged(i, i);
                i = i.parent().sibling(i.parent().row(), 1);
            }

            return true;
        }
    }

    return false;
}

int QPx::PropertyBrowserModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

void QPx::PropertyBrowserModel::valueChanged(const QVariant &value)
{
    if(!cache.get<Cache>().valueChangeLock)
    {
        auto index = cache.get<Cache>().map[static_cast<PropertyBrowserItem*>(sender())];
        auto i = index.sibling(index.row(), 1);

        while(i.isValid())
        {
            emit dataChanged(i, i);
            i = i.parent().sibling(i.parent().row(), 1);
        }
    }
}


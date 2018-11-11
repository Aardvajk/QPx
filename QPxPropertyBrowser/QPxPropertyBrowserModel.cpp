#include "QPxPropertyBrowserModel.h"

#include "QPxPropertyBrowser/QPxPropertyBrowserType.h"
#include "QPxPropertyBrowser/QPxPropertyBrowserItem.h"

#include <pcx/scoped_lock.h>

#include <QtCore/QMap>
#include <QtCore/QPersistentModelIndex>

namespace
{

class Cache
{
public:
    void remove(QPx::PropertyBrowserModel *model, int row, int count, const QModelIndex &parent);

    QMap<const QPx::PropertyBrowserItem*, QPersistentModelIndex> map;
};

void Cache::remove(QPx::PropertyBrowserModel *model, int row, int count, const QModelIndex &parent)
{
    for(int i = row; i < row + count; ++i)
    {
        auto index = model->index(i, 0, parent);
        remove(model, 0, model->rowCount(index), index);

        map.remove(static_cast<QPx::PropertyBrowserItem*>(model->userData(index)));
    }
}

}

QPx::PropertyBrowserModel::PropertyBrowserModel(QObject *parent) : TreeModel(parent)
{
    cache.alloc<Cache>();
}

void QPx::PropertyBrowserModel::clear()
{
    cache.get<Cache>().map.clear();
    TreeModel::clear();
}

Qt::ItemFlags QPx::PropertyBrowserModel::flags(const QModelIndex &index) const
{
    auto value = TreeModel::flags(index);

    if(auto item = static_cast<const PropertyBrowserItem*>(userData(index)))
    {
        if(index.column() == 1 && !item->type()->readOnly() && !(item->flags() & PropertyBrowserItem::Flag::ReadOnly))
        {
            if(item->value().type() == QVariant::Bool)
            {
                value |= Qt::ItemIsUserCheckable;
            }
            else
            {
                value |= Qt::ItemIsEditable;
            }
        }
    }

    return value;
}

QVariant QPx::PropertyBrowserModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0: return "Property";
            case 1: return "Value";
        }
    }

    return QVariant();
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
                case 1: return item->type()->valueText(item);
            }
        }
        else if(role == Qt::CheckStateRole)
        {
            if(item->value().type() == QVariant::Bool && index.column() == 1)
            {
                return item->value().toBool() ? Qt::Checked : Qt::Unchecked;
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
            if(!item->type()->compare(item->value(), value))
            {
                item->setValue(value);

                emit dataChanged(index, index);
                return true;
            }
        }
        else if(role == Qt::CheckStateRole)
        {
            bool checkState = value.toInt() == Qt::Checked ? true : false;

            if(item->value().toBool() != checkState)
            {
                item->setValue(checkState);

                emit dataChanged(index, index);
                return true;
            }
        }
    }

    return false;
}

bool QPx::PropertyBrowserModel::removeRows(int row, int count, const QModelIndex &parent)
{
    cache.get<Cache>().remove(this, row, count, parent);

    return TreeModel::removeRows(row, count, parent);
}

QModelIndex QPx::PropertyBrowserModel::insertRow(int row, void *userData, const QModelIndex &parent)
{
    auto index = TreeModel::insertRow(row, userData, parent);
    auto item = static_cast<PropertyBrowserItem*>(userData);

    cache.get<Cache>().map[item] = index;
    connect(item, SIGNAL(valueChanged(QVariant)), SLOT(valueChanged(QVariant)));

    return index;
}

QModelIndex QPx::PropertyBrowserModel::appendRow(void *item, const QModelIndex &parent)
{
    return insertRow(rowCount(parent), item, parent);
}

QModelIndex QPx::PropertyBrowserModel::indexOf(const QPx::PropertyBrowserItem *item) const
{
    return cache.get<Cache>().map[item];
}

int QPx::PropertyBrowserModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

void QPx::PropertyBrowserModel::valueChanged(const QVariant &value)
{
    auto index = cache.get<Cache>().map[static_cast<PropertyBrowserItem*>(sender())];
    auto i = index.sibling(index.row(), 1);

    emit dataChanged(i, i);
}

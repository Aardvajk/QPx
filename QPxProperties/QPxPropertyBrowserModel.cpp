#include "QPxPropertyBrowserModel.h"

#include "QPxProperties/QPxPropertyBrowserItem.h"

QPx::PropertyBrowserModel::PropertyBrowserModel(QObject *parent) : TreeModel(parent)
{
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

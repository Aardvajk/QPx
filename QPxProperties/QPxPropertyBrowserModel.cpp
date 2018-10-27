#include "QPxPropertyBrowserModel.h"

#include "QPxProperties/QPxPropertyBrowserItem.h"

QPx::PropertyBrowserModel::PropertyBrowserModel(QObject *parent) : TreeModel(parent)
{
}

QModelIndex QPx::PropertyBrowserModel::appendRow(PropertyBrowserItem *item, const QModelIndex &parent)
{
    return TreeModel::appendRow(QVariant::fromValue(item), parent);
}

QPx::PropertyBrowserItem *QPx::PropertyBrowserModel::browserItem(const QModelIndex &index) const
{
    return qvariant_cast<PropertyBrowserItem*>(userData(index));
}

Qt::ItemFlags QPx::PropertyBrowserModel::flags(const QModelIndex &index) const
{
    auto value = TreeModel::flags(index);

    if(auto item = browserItem(index))
    {
    }

    return value;
}

QVariant QPx::PropertyBrowserModel::data(const QModelIndex &index, int role) const
{
    if(auto item = browserItem(index))
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

int QPx::PropertyBrowserModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

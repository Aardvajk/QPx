#include "internal/qpx_property_browser_model.h"

qpx_property_browser_model::qpx_property_browser_model(QObject *parent) : QPx::TreeModel(parent)
{
}

QVariant qpx_property_browser_model::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        return index.data(QPx::UserDataRole);
    }

    return QPx::TreeModel::data(index, role);
}

int qpx_property_browser_model::columnCount(const QModelIndex &parent) const
{
    return 2;
}



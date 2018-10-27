#ifndef QPXPROPERTYBROWSERMODEL_H
#define QPXPROPERTYBROWSERMODEL_H

#include <QPxCore/QPxTreeModel.h>

namespace QPx
{

class PropertyBrowserItem;

class PropertyBrowserModel : public TreeModel
{
    Q_OBJECT

public:
    PropertyBrowserModel(QObject *parent = nullptr);

    QModelIndex appendRow(PropertyBrowserItem *item, const QModelIndex &parent = QModelIndex());

    PropertyBrowserItem *browserItem(const QModelIndex &index) const;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
};

}

#endif // QPXPROPERTYBROWSERMODEL_H

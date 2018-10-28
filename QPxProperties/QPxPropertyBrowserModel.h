#ifndef QPXPROPERTYBROWSERMODEL_H
#define QPXPROPERTYBROWSERMODEL_H

#include <QPxCore/QPxTreeModel.h>

namespace QPx
{

class PropertyBrowserModel : public TreeModel
{
    Q_OBJECT

public:
    PropertyBrowserModel(QObject *parent = nullptr);

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
};

}

#endif // QPXPROPERTYBROWSERMODEL_H

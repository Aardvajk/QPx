#ifndef QPX_PROPERTYBROWSERMODEL_H
#define QPX_PROPERTYBROWSERMODEL_H

#include <QPxCore/QPxTreeModel.h>

#include <pcx/aligned_store.h>

namespace QPx
{

class PropertyBrowserItem;

class PropertyBrowserModel : public TreeModel
{
    Q_OBJECT

public:
    PropertyBrowserModel(QObject *parent = nullptr);

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    virtual QModelIndex insertRow(int row, void *userData, const QModelIndex &parent = QModelIndex()) override;
    virtual QModelIndex appendRow(void *item, const QModelIndex &parent = QModelIndex()) override;

    virtual QModelIndex indexOf(const QPx::PropertyBrowserItem *item) const;

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private slots:
    void valueChanged(const QVariant &value);

private:
    pcx::aligned_store<32> cache;
};

}

#endif // QPX_PROPERTYBROWSERMODEL_H

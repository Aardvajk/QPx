#ifndef QPX_TREEMODEL_H
#define QPX_TREEMODEL_H

#include <QtCore/QAbstractItemModel>

#include <pcx/aligned_store.h>

namespace QPx
{

extern int UserDataRole;
extern int UserRole;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(QObject *parent = nullptr);

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex &index) const override;

    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    pcx::aligned_store<8> cache;
};

}

#endif // QPX_TREEMODEL_H

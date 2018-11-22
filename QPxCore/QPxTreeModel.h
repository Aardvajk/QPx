#ifndef QPX_TREEMODEL_H
#define QPX_TREEMODEL_H

#include <QPxCore/QPxDeleter.h>

#include <QtCore/QAbstractItemModel>

namespace QPx
{

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(QObject *parent = nullptr);
    virtual ~TreeModel() override;

    virtual void clear();

    virtual bool setUserData(const QModelIndex &index, void *value);
    virtual void setUserDataDeleter(AbstractDeleter *value);

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex &index) const override;

    virtual QModelIndex insertRow(int row, void *userData, const QModelIndex &parent = QModelIndex());
    virtual QModelIndex appendRow(void *userData, const QModelIndex &parent = QModelIndex());

    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    static void *userData(const QModelIndex &index);

private:
    class Node;
    friend class Node;

    Node *root;
    AbstractDeleter *deleter;
};

}

#endif // QPX_TREEMODEL_H

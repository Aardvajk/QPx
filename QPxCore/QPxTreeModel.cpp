#include "QPxCore/QPxTreeModel.h"

#include <pcx/scoped_ptr.h>

namespace
{

class Node
{
public:
    explicit Node(Node *parent, int row, const QVariant &userData = QVariant()) : parent(parent), row(row), userData(userData) { }
    ~Node(){ qDeleteAll(children); }

    Node *parent;
    QList<Node*> children;
    int row;

    QVariant userData;
};

class Cache
{
public:
    explicit Cache() : root(new Node(nullptr, 0)) { }

    pcx::scoped_ptr<Node> root;
};

}

QPx::TreeModel::TreeModel(QObject *parent) : QAbstractItemModel(parent)
{
    cache.alloc<Cache>();
}

Qt::ItemFlags QPx::TreeModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index);
}

QVariant QPx::TreeModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

QVariant QPx::TreeModel::userData(const QModelIndex &index) const
{
    if(auto node = static_cast<Node*>(index.internalPointer()))
    {
        return node->userData;
    }

    return QVariant();
}

bool QPx::TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

bool QPx::TreeModel::setUserData(const QModelIndex &index, const QVariant &value)
{
    if(auto node = static_cast<Node*>(index.internalPointer()))
    {
        node->userData = value;

        emit dataChanged(this->index(index.row(), 0, index.parent()), this->index(index.row(), columnCount(index.parent()), index.parent()));
        return true;
    }

    return false;
}

QModelIndex QPx::TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    const Node *parentNode = parent.isValid() ? static_cast<const Node*>(parent.internalPointer()) : cache.get<Cache>().root.get();
    Node *node = parentNode->children.value(row);

    if(node)
    {
        return createIndex(row, column, node);
    }

    return QModelIndex();
}

QModelIndex QPx::TreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        return QModelIndex();
    }

    Node *parentNode = static_cast<Node*>(index.internalPointer())->parent;

    if(parentNode == cache.get<Cache>().root.get())
    {
        return QModelIndex();
    }

    return createIndex(parentNode->row, 0, parentNode);
}

bool QPx::TreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Node *parentNode = parent.isValid() ? static_cast<Node*>(parent.internalPointer()) : cache.get<Cache>().root.get();

    beginInsertRows(parent, row, (row + count) - 1);

    for(int i = 0; i < count; ++i)
    {
        auto node = new Node(parentNode, row);
        parentNode->children.insert(row, node);
    }

    endInsertRows();
    return true;
}

bool QPx::TreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Node *parentNode = parent.isValid() ? static_cast<Node*>(parent.internalPointer()) : cache.get<Cache>().root.get();

    beginRemoveRows(parent, row, (row + count) - 1);

    for(int i = 0; i < count; ++i)
    {
        delete parentNode->children[row];
        parentNode->children.removeAt(row);
    }

    endRemoveRows();
    return true;
}

QModelIndex QPx::TreeModel::insertRow(int row, const QVariant &userData, const QModelIndex &parent)
{
    Node *parentNode = parent.isValid() ? static_cast<Node*>(parent.internalPointer()) : cache.get<Cache>().root.get();

    beginInsertRows(parent, row, row);

    auto node = new Node(parentNode, row, userData);
    parentNode->children.insert(row, node);

    endInsertRows();

    return index(row, 0, parent);
}

QModelIndex QPx::TreeModel::appendRow(const QVariant &userData, const QModelIndex &parent)
{
    return insertRow(rowCount(parent), userData, parent);
}

int QPx::TreeModel::rowCount(const QModelIndex &parent) const
{
    return (parent.isValid() ? static_cast<Node*>(parent.internalPointer()) : cache.get<Cache>().root.get())->children.count();
}

int QPx::TreeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

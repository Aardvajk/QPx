#include "QPxCore/QPxTreeModel.h"

#include <pcx/scoped_ptr.h>

#include "QPxPropertyBrowser/QPxPropertyBrowserItem.h"

namespace
{

class Node
{
public:
    explicit Node(QPx::TreeModel *model, Node *parent, int row, void *userData) : model(model), parent(parent), row(row), userData(userData) { }
    ~Node();

    QPx::TreeModel *model;

    Node *parent;
    QList<Node*> children;
    int row;

    void *userData;
};

Node::~Node()
{
    qDeleteAll(children);
    model->invokeUserDataDeleter(userData);
}

class Cache
{
public:
    Cache(QPx::TreeModel *model) : root(new Node(model, nullptr, 0, nullptr)), deleter(nullptr) { }

    pcx::scoped_ptr<Node> root;
    QPx::AbstractDeleter *deleter;
};

}

QPx::TreeModel::TreeModel(QObject *parent) : QAbstractItemModel(parent)
{
    cache.alloc<Cache>(this);
}

void QPx::TreeModel::clear()
{
    beginResetModel();

    auto &c = cache.get<Cache>();

    qDeleteAll(c.root->children);
    c.root->children.clear();

    endResetModel();
}

bool QPx::TreeModel::setUserData(const QModelIndex &index, void *value)
{
    if(auto node = static_cast<Node*>(index.internalPointer()))
    {
        node->userData = value;

        emit dataChanged(this->index(index.row(), 0, index.parent()), this->index(index.row(), columnCount(index.parent()), index.parent()));
        return true;
    }

    return false;
}

void QPx::TreeModel::setUserDataDeleter(QPx::AbstractDeleter *deleter)
{
    auto &c = cache.get<Cache>();
    if(c.deleter && c.deleter->parent() == this)
    {
        delete c.deleter;
    }

    c.deleter = deleter;
}

void QPx::TreeModel::invokeUserDataDeleter(void *data) const
{
    auto &d = cache.get<Cache>().deleter;
    if(d && data)
    {
        d->operator()(data);
    }
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

QModelIndex QPx::TreeModel::insertRow(int row, void *userData, const QModelIndex &parent)
{
    Node *parentNode = parent.isValid() ? static_cast<Node*>(parent.internalPointer()) : cache.get<Cache>().root.get();

    beginInsertRows(parent, row, row);

    auto node = new Node(this, parentNode, row, userData);
    parentNode->children.insert(row, node);

    endInsertRows();

    return index(row, 0, parent);
}

QModelIndex QPx::TreeModel::appendRow(void *userData, const QModelIndex &parent)
{
    return insertRow(rowCount(parent), userData, parent);
}

bool QPx::TreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Node *parentNode = parent.isValid() ? static_cast<Node*>(parent.internalPointer()) : cache.get<Cache>().root.get();

    beginInsertRows(parent, row, (row + count) - 1);

    for(int i = 0; i < count; ++i)
    {
        auto node = new Node(this, parentNode, row, nullptr);
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

int QPx::TreeModel::rowCount(const QModelIndex &parent) const
{
    return (parent.isValid() ? static_cast<Node*>(parent.internalPointer()) : cache.get<Cache>().root.get())->children.count();
}

void *QPx::TreeModel::userData(const QModelIndex &index)
{
    if(auto node = static_cast<Node*>(index.internalPointer()))
    {
        return node->userData;
    }

    return nullptr;
}

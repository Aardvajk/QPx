#include "QPxCore/QPxTreeModel.h"

#include <pcx/scoped_ptr.h>

#include "QPxPropertyBrowser/QPxPropertyBrowserItem.h"

class QPx::TreeModelNode
{
public:
    explicit TreeModelNode(QPx::TreeModel *model, TreeModelNode *parent, int row, void *userData) : model(model), parent(parent), row(row), userData(userData) { }
    ~TreeModelNode();

    QPx::TreeModel *model;

    TreeModelNode *parent;
    QList<TreeModelNode*> children;
    int row;

    void *userData;
};

namespace
{

class Cache
{
public:
    Cache(QPx::TreeModel *model) : root(new QPx::TreeModelNode(model, nullptr, 0, nullptr)), deleter(nullptr) { }

    pcx::scoped_ptr<QPx::TreeModelNode> root;
    QPx::AbstractDeleter *deleter;
};

}

QPx::TreeModelNode::~TreeModelNode()
{
    qDeleteAll(children);

    auto &d = model->cache.get<Cache>().deleter;
    if(d && userData)
    {
        d->operator()(userData);
    }
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
    if(auto node = static_cast<TreeModelNode*>(index.internalPointer()))
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

QModelIndex QPx::TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    const TreeModelNode *parentNode = parent.isValid() ? static_cast<const TreeModelNode*>(parent.internalPointer()) : cache.get<Cache>().root.get();
    TreeModelNode *node = parentNode->children.value(row);

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

    TreeModelNode *parentNode = static_cast<TreeModelNode*>(index.internalPointer())->parent;

    if(parentNode == cache.get<Cache>().root.get())
    {
        return QModelIndex();
    }

    return createIndex(parentNode->row, 0, parentNode);
}

QModelIndex QPx::TreeModel::insertRow(int row, void *userData, const QModelIndex &parent)
{
    TreeModelNode *parentNode = parent.isValid() ? static_cast<TreeModelNode*>(parent.internalPointer()) : cache.get<Cache>().root.get();

    beginInsertRows(parent, row, row);

    auto node = new TreeModelNode(this, parentNode, row, userData);
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
    TreeModelNode *parentNode = parent.isValid() ? static_cast<TreeModelNode*>(parent.internalPointer()) : cache.get<Cache>().root.get();

    beginInsertRows(parent, row, (row + count) - 1);

    for(int i = 0; i < count; ++i)
    {
        auto node = new TreeModelNode(this, parentNode, row, nullptr);
        parentNode->children.insert(row, node);
    }

    endInsertRows();
    return true;
}

bool QPx::TreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    TreeModelNode *parentNode = parent.isValid() ? static_cast<TreeModelNode*>(parent.internalPointer()) : cache.get<Cache>().root.get();

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
    return (parent.isValid() ? static_cast<TreeModelNode*>(parent.internalPointer()) : cache.get<Cache>().root.get())->children.count();
}

void *QPx::TreeModel::userData(const QModelIndex &index)
{
    if(auto node = static_cast<TreeModelNode*>(index.internalPointer()))
    {
        return node->userData;
    }

    return nullptr;
}

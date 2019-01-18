#include "QPxCore/QPxTreeModel.h"

#include <pcx/scoped_ptr.h>

#include "QPxPropertyBrowser/QPxPropertyBrowserItem.h"

class QPx::TreeModel::Node
{
public:
    explicit Node(TreeModel *model, Node *parent, int row, void *userData) : model(model), parent(parent), row(row), userData(userData) { }
    ~Node();

    TreeModel *model;

    Node *parent;
    QVector<Node*> children;
    int row;

    void *userData;
};

QPx::TreeModel::Node::~Node()
{
    qDeleteAll(children);

    auto &d = model->deleter;
    if(d && userData)
    {
        d->operator()(userData);
    }
}

QPx::TreeModel::TreeModel(QObject *parent) : QAbstractItemModel(parent), deleter(nullptr)
{
    root = new Node(this, nullptr, 0, nullptr);
}

QPx::TreeModel::~TreeModel()
{
    delete root;
}

void QPx::TreeModel::clear()
{
    beginResetModel();

    qDeleteAll(root->children);
    root->children.clear();

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

void QPx::TreeModel::setUserDataDeleter(QPx::AbstractDeleter *value)
{
    if(deleter && deleter->parent() == this)
    {
        delete deleter;
    }

    deleter = value;
}

QModelIndex QPx::TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    const Node *parentNode = parent.isValid() ? static_cast<const Node*>(parent.internalPointer()) : root;
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

    if(parentNode == root)
    {
        return QModelIndex();
    }

    return createIndex(parentNode->row, 0, parentNode);
}

QModelIndex QPx::TreeModel::insertRow(int row, void *userData, const QModelIndex &parent)
{
    Node *parentNode = parent.isValid() ? static_cast<Node*>(parent.internalPointer()) : root;

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
    Node *parentNode = parent.isValid() ? static_cast<Node*>(parent.internalPointer()) : root;

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
    Node *parentNode = parent.isValid() ? static_cast<Node*>(parent.internalPointer()) : root;

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
    return (parent.isValid() ? static_cast<const Node*>(parent.internalPointer()) : root)->children.count();
}

void *QPx::TreeModel::userData(const QModelIndex &index)
{
    if(auto node = static_cast<const Node*>(index.internalPointer()))
    {
        return node->userData;
    }

    return nullptr;
}

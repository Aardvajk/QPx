#include "QPxProperties/QPxPropertyBrowserDelegate.h"

#include "QPxProperties/QPxPropertyEditor.h"
#include "QPxProperties/QPxPropertyBrowserItem.h"
#include "QPxProperties/QPxPropertyBrowserModel.h"

QPx::PropertyBrowserDelegate::PropertyBrowserDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
}

QWidget *QPx::PropertyBrowserDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch(static_cast<const PropertyBrowserItem*>(QPx::TreeModel::userData(index))->value().type())
    {
        case QVariant::Int: return new IntPropertyEditor(parent);
        case QVariant::String: return new StringPropertyEditor(parent);

        default: return nullptr;
    }
}

void QPx::PropertyBrowserDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    static_cast<PropertyEditor*>(editor)->setValue(static_cast<const PropertyBrowserItem*>(QPx::TreeModel::userData(index))->value());
}

void QPx::PropertyBrowserDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    model->setData(index, static_cast<const PropertyEditor*>(editor)->value(), Qt::EditRole);
}

void QPx::PropertyBrowserDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect.adjusted(-1, 0, 0, 0));
}

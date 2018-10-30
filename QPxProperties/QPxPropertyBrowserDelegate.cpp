#include "QPxProperties/QPxPropertyBrowserDelegate.h"

#include "QPxProperties/QPxPropertyBrowserItem.h"
#include "QPxProperties/QPxPropertyBrowserModel.h"
#include "QPxProperties/QPxPropertyBrowserEditor.h"
#include "QPxProperties/QPxPropertyBrowserDialog.h"

#include <pcx/scoped_ptr.h>

#include <QtCore/QEvent>

namespace
{

class Cache
{
public:
    explicit Cache(QWidget *widget) : widget(widget) { }

    QWidget *widget;
};

}

QPx::PropertyBrowserDelegate::PropertyBrowserDelegate(QWidget *parent) : QStyledItemDelegate(parent)
{
    cache.alloc<Cache>(parent);
}

void QPx::PropertyBrowserDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto item = static_cast<const PropertyBrowserItem*>(QPx::TreeModel::userData(index));

    if(item->value().type() != QVariant::Bool && index.column() == 1)
    {
        QStyledItemDelegate::paint(painter, option, QModelIndex());
        item->paint(painter, option.rect);
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QWidget *QPx::PropertyBrowserDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return static_cast<const PropertyBrowserItem*>(QPx::TreeModel::userData(index))->createEditor(parent);
}

void QPx::PropertyBrowserDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    static_cast<PropertyBrowserEditor*>(editor)->setValue(static_cast<const PropertyBrowserItem*>(QPx::TreeModel::userData(index))->value());
}

void QPx::PropertyBrowserDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    model->setData(index, static_cast<const PropertyBrowserEditor*>(editor)->value(), Qt::EditRole);
}

void QPx::PropertyBrowserDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect.adjusted(-1, 0, 0, 0));
}

bool QPx::PropertyBrowserDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonDblClick && !model->rowCount(index))
    {
        auto item = static_cast<const PropertyBrowserItem*>(QPx::TreeModel::userData(index));

        if(pcx::scoped_ptr<PropertyBrowserDialog> dialog = item->createDialog(cache.get<Cache>().widget))
        {
            dialog->setValue(item->value());

            if(dialog->exec() == QDialog::Accepted)
            {
                model->setData(index, dialog->value(), Qt::EditRole);
            }

            return true;
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

#ifndef QPX_PROPERTYBROWSERDELEGATE_H
#define QPX_PROPERTYBROWSERDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

#include <pcx/aligned_store.h>

class QWidget;

namespace QPx
{

class PropertyBrowserDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    PropertyBrowserDelegate(QWidget *parent = nullptr);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

protected:
    virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    pcx::aligned_store<8> cache;
};

}

#endif // QPX_PROPERTYBROWSERDELEGATE_H

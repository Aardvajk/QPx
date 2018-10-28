#ifndef QPX_PROPERTYBROWSERDELEGATE_H
#define QPX_PROPERTYBROWSERDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

namespace QPx
{

class PropertyBrowserDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    PropertyBrowserDelegate(QObject *parent = nullptr);

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

}

#endif // QPX_PROPERTYBROWSERDELEGATE_H

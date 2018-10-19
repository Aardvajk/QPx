#ifndef QPX_KEYBOARDOPTIONSWIDGET_H
#define QPX_KEYBOARDOPTIONSWIDGET_H

#include <QtWidgets/QWidget>

#include <pcx/aligned_store.h>

class QTreeWidgetItem;

namespace QPx
{

class ActionList;

class KeyboardOptionsWidget : public QWidget
{
    Q_OBJECT

public:
    KeyboardOptionsWidget(ActionList *actions, QWidget *parent = nullptr);

public slots:
    void commit();

private slots:
    void filterChanged(const QString &text);
    void currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem*);
    void keySequenceChanged(const QKeySequence &value);
    void warningLinkClicked(const QString &value);

private:
    void populateTree();
    void checkConflicts();
    void updateKeyEditColor();

    pcx::aligned_store<56> cache;
};

}

#endif // QPX_KEYBOARDOPTIONSWIDGET_H

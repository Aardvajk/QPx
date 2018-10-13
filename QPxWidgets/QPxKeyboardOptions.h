#ifndef QPX_KEYBOARDOPTIONS_H
#define QPX_KEYBOARDOPTIONS_H

#include <QtWidgets/QWidget>

#include <pcx/aligned_store.h>

class QTreeWidgetItem;

namespace QPx
{

class ActionList;

class KeyboardOptions : public QWidget
{
    Q_OBJECT

public:
    KeyboardOptions(ActionList *actions, QWidget *parent = nullptr);

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

    pcx::aligned_store<128> cache;
};

}

#endif // QPX_KEYBOARDOPTIONS_H

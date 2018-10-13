#include "QPxWidgets/QPxActionOptionsWidget.h"

#include <QPxActions/QPxAction.h>
#include <QPxActions/QPxActionList.h>

#include "QPxWidgets/QPxKeySequenceEdit.h"
#include "QPxWidgets/QPxIconLabel.h"

#include <QtWidgets/QStyle>

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>

namespace
{

class TreeWidgetItem : public QTreeWidgetItem
{
public:
    TreeWidgetItem(QTreeWidgetItem *parent, const QStringList &strings, QPx::Action *action) : QTreeWidgetItem(parent, strings), action(action), desc(action->statusTip()), conflict(false) { }

    QPx::Action *action;
    QString desc;
    bool conflict;
};

class Cache
{
public:
    Cache(QPx::ActionList *actions) : actions(actions), lock(false) { }

    QTreeWidget *tree;
    QLineEdit *filterEdit;
    QLineEdit *descEdit;
    QPx::KeySequenceEdit *keyEdit;
    QPx::IconLabel *warning;

    QPx::ActionList *actions;
    bool lock;
};

}

QPx::ActionOptionsWidget::ActionOptionsWidget(ActionList *actions, QWidget *parent) : QWidget(parent)
{
    cache.alloc<Cache>(actions);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    auto &c = cache.get<Cache>();

    c.filterEdit = new QLineEdit(this);
    c.filterEdit->setPlaceholderText("Filter");
    c.filterEdit->setClearButtonEnabled(true);

    mainLayout->addWidget(c.filterEdit);

    c.tree = new QTreeWidget(this);

    c.tree->setColumnCount(3);
    c.tree->setHeaderLabels(QStringList() << tr("Name") << tr("Label") << tr("Shortcut"));

    c.tree->setColumnWidth(0, 170);
    c.tree->setColumnWidth(1, 250);

    c.tree->setMinimumWidth(550);
    c.tree->setMinimumHeight(300);

    mainLayout->addWidget(c.tree);

    QFormLayout *form = new QFormLayout();
    form->setMargin(0);

    c.descEdit = new QLineEdit();
    c.descEdit->setClearButtonEnabled(true);
    c.descEdit->setEnabled(false);

    form->addRow("Description:", c.descEdit);

    c.keyEdit = new KeySequenceEdit();
    c.keyEdit->setPlaceholderText("Click to modify");
    c.keyEdit->setEnabled(false);

    form->addRow("Shortcut:", c.keyEdit);

    c.warning = new IconLabel();
    c.warning->setTextFormat(Qt::RichText);

    form->addRow(c.warning);

    mainLayout->addSpacing(4);
    mainLayout->addLayout(form);

    populateTree();
    checkConflicts();

    connect(c.filterEdit, SIGNAL(textChanged(QString)), SLOT(filterChanged(QString)));
    connect(c.tree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), SLOT(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(c.descEdit, SIGNAL(textChanged(QString)), SLOT(descriptionEditChanged(QString)));
    connect(c.keyEdit, SIGNAL(keySequenceChanged(QKeySequence)), SLOT(keySequenceChanged(QKeySequence)));
    connect(c.warning, SIGNAL(linkActivated(QString)), SLOT(warningLinkClicked(QString)));
}

void QPx::ActionOptionsWidget::commit()
{
    auto &c = cache.get<Cache>();

    for(int i = 0; i < c.tree->topLevelItemCount(); ++i)
    {
        auto group = c.tree->topLevelItem(i);

        for(int j = 0; j < group->childCount(); ++j)
        {
            auto item = static_cast<TreeWidgetItem*>(group->child(j));

            item->action->setShortcut(QKeySequence(item->data(2, Qt::DisplayRole).toString()));
            item->action->setStatusTip(item->desc);
        }
    }
}

void QPx::ActionOptionsWidget::filterChanged(const QString &text)
{
    auto &c = cache.get<Cache>();

    QString filter = text.trimmed();

    for(int i = 0; i < c.tree->model()->rowCount(); ++i)
    {
        QModelIndex sectionIndex = c.tree->model()->index(i, 0);

        bool anyVisible = false;

        for(int j = 0; j < c.tree->model()->rowCount(sectionIndex); ++j)
        {
            bool show = false;

            QTreeWidgetItem *item = c.tree->topLevelItem(i)->child(j);

            if(filter.isEmpty())
            {
                show = true;
            }
            else
            {
                for(int k = 0; k < 3; ++k)
                {
                    if(item->data(k, Qt::DisplayRole).toString().contains(filter, Qt::CaseInsensitive))
                    {
                        show = true;
                        break;
                    }
                }
            }

            QModelIndex current = c.tree->selectionModel()->currentIndex();

            if(show)
            {
                c.tree->setRowHidden(j, sectionIndex, false);
                anyVisible = true;

                if(current.parent() == sectionIndex && current.row() == j)
                {
                    currentItemChanged(c.tree->currentItem(), 0);
                }
            }
            else
            {
                c.tree->setRowHidden(j, sectionIndex, true);

                if(current.parent() == sectionIndex && current.row() == j)
                {
                    currentItemChanged(0, 0);
                }
            }
        }

        c.tree->setRowHidden(i, QModelIndex(), !anyVisible);
    }
}

void QPx::ActionOptionsWidget::currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *)
{
    auto &c = cache.get<Cache>();

    c.lock = true;

    if(!current || !current->parent())
    {
        c.keyEdit->setEnabled(false);
        c.keyEdit->setKeySequence(QKeySequence());

        c.descEdit->setEnabled(false);
        c.descEdit->setText({ });
    }
    else
    {
        c.keyEdit->setEnabled(true);
        c.keyEdit->setKeySequence(QKeySequence(current->data(2, Qt::DisplayRole).toString()));

        c.descEdit->setEnabled(true);
        c.descEdit->setText(static_cast<TreeWidgetItem*>(current)->desc);
    }

    c.lock = false;

    if(current && current->parent())
    {
        if(static_cast<TreeWidgetItem*>(current)->conflict)
        {
            c.warning->setText(QString("Key sequence has conflicts. <a href=\"%1\">View</a>.").arg(current->data(2, Qt::DisplayRole).toString()));
            c.warning->setIcon(style()->standardIcon(QStyle::SP_MessageBoxWarning));
        }
        else
        {
            c.warning->setText({ });
            c.warning->setIcon({ });
        }
    }

    updateKeyEditColor();
}

void QPx::ActionOptionsWidget::descriptionEditChanged(const QString &value)
{
    auto &c = cache.get<Cache>();

    if(!c.lock)
    {
        QTreeWidgetItem *current = c.tree->currentItem();

        if(current && current->parent())
        {
            static_cast<TreeWidgetItem*>(current)->desc = value;
        }
    }
}

void QPx::ActionOptionsWidget::keySequenceChanged(const QKeySequence &value)
{
    auto &c = cache.get<Cache>();

    if(!c.lock)
    {
        QTreeWidgetItem *current = c.tree->currentItem();

        if(current && current->parent())
        {
            current->setData(2, Qt::DisplayRole, value.toString(QKeySequence::NativeText));
            checkConflicts();
        }
    }
}

void QPx::ActionOptionsWidget::warningLinkClicked(const QString &value)
{
    cache.get<Cache>().filterEdit->setText(value);
}

void QPx::ActionOptionsWidget::populateTree()
{
    auto &c = cache.get<Cache>();

    QMap<QString, QList<QPair<QString, Action*> > > map;

    for(auto action: c.actions->toList())
    {
        int pos = action->id().indexOf(QLatin1Char('.'));

        QString group = action->id().left(pos);
        QString name = action->id().mid(pos + 1);

        map[group].append(qMakePair(name, action));
    }

    QFont boldFont = c.tree->font();
    boldFont.setBold(true);

    for(auto group = map.begin(); group != map.end(); ++group)
    {
        QTreeWidgetItem *parent = new QTreeWidgetItem(c.tree, QStringList(group.key()));

        qSort(group.value().begin(), group.value().end(), [](const QPair<QString, Action*> &a, const QPair<QString, Action*> &b){ return a.first < b.first; });

        for(auto value: group.value())
        {
            QString label = value.second->text();

            int i = label.indexOf('&');
            if(i >= 0)
            {
                label.remove(i, 1);
            }

            auto item = new TreeWidgetItem(parent, QStringList() << value.first << label << value.second->shortcut().toString(QKeySequence::NativeText), value.second);
            parent->addChild(item);
        }

        c.tree->addTopLevelItem(parent);;
        parent->setData(0, Qt::FontRole, boldFont);
    }

    c.tree->expandAll();
}

void QPx::ActionOptionsWidget::checkConflicts()
{
    auto &c = cache.get<Cache>();

    QMap<QKeySequence, QList<QTreeWidgetItem*> > map;

    for(int i = 0; i < c.tree->topLevelItemCount(); ++i)
    {
        auto group = c.tree->topLevelItem(i);

        for(int j = 0; j < group->childCount(); ++j)
        {
            auto item = static_cast<TreeWidgetItem*>(group->child(j));
            auto seq = QKeySequence(item->data(2, Qt::DisplayRole).toString());

            if(!seq.isEmpty())
            {
                map[seq].append(item);
            }

            item->conflict = false;
        }
    }

    for(auto item = map.begin(); item != map.end(); ++item)
    {
        for(auto child: item.value())
        {
            bool conflict = item.value().count() > 1;

            static_cast<TreeWidgetItem*>(child)->conflict = conflict;
            child->setData(2, Qt::TextColorRole, conflict ? QColor(Qt::red) : c.tree->palette().color(QPalette::Text));
        }
    }

    currentItemChanged(c.tree->currentItem(), 0);
}

void QPx::ActionOptionsWidget::updateKeyEditColor()
{
    auto &c = cache.get<Cache>();
    auto pal = c.keyEdit->palette();

    auto current = c.tree->currentItem();
    if(current && current->parent() && static_cast<TreeWidgetItem*>(current)->conflict)
    {
        pal.setColor(QPalette::Text, Qt::red);
    }
    else
    {
        pal.setColor(QPalette::Text, style()->standardPalette().color(QPalette::Text));
    }

    c.keyEdit->setPalette(pal);
}

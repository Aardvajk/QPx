#include "QPxActions/QPxActionList.h"

#include "QPxActions/QPxAction.h"

#include <QtWidgets/QWidget>

namespace
{

class Cache
{
public:
    QList<QPx::Action*> actions;
    QMap<QString, int> indices;
};

}

QPx::ActionList::ActionList(QWidget *parent) : QObject(parent)
{
    cache.alloc<Cache>();
}

QPx::Action *QPx::ActionList::add(const QString &id, const QString &text, const QString &tip, const QKeySequence &shortcut, const QIcon &icon, Enable enable)
{
    auto &c = cache.get<Cache>();
    
    c.indices[id] = c.actions.count();

    auto a = new Action(id, this);
    c.actions.append(a);

    a->setText(text);
    a->setStatusTip(tip);
    a->setShortcut(shortcut);
    a->setIcon(icon);
    a->setEnabled(enable == Enable::On);

    static_cast<QWidget*>(parent())->addAction(a);
    
    return a;
}

QPx::Action *QPx::ActionList::add(const QString &id, const QString &text, Enable enable)
{
    return add(id, text, { }, { }, { }, enable);
}

QPx::Action *QPx::ActionList::find(const QString &id)
{
    auto &c = cache.get<Cache>();

    auto i = c.indices.find(id);
    return i == c.indices.end() ? nullptr : c.actions[i.value()];
}

QList<QPx::Action*> QPx::ActionList::toList() const
{
    return cache.get<Cache>().actions;
}


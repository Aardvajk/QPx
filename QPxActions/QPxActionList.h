#ifndef QPX_ACTIONLIST_H
#define QPX_ACTIONLIST_H

#include <QtCore/QObject>

#include <pcx/aligned_store.h>

class QWidget;
class QKeySequence;
class QIcon;

namespace QPx
{

class Action;

class ActionList : public QObject
{
    Q_OBJECT

public:
    ActionList(QWidget *parent);

    enum class Enable
    {
        On,
        Off
    };

    Action *add(const QString &id, const QString &text, const QKeySequence &shortcut, const QIcon &icon, Enable enable = Enable::On);
    Action *add(const QString &id, const QString &text, Enable enable = Enable::On);

    Action *find(const QString &id);

    QList<Action*> toList() const;

private:
    pcx::aligned_store<16> cache;    
};

}

#endif // QPX_ACTIONLIST_H

#ifndef QPX_ACTION_H
#define QPX_ACTION_H

#include <QtWidgets/QAction>

namespace QPx
{

class Action : public QAction
{
    Q_OBJECT

public:
    Action(const QString &id, QObject *parent = nullptr);

    QString id() const;

public slots:
    void setId(const QString &id);
};

}

#endif // QPX_ACTION_H

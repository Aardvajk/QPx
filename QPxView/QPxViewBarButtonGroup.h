#ifndef QPX_VIEWBARBUTTONGROUP_H
#define QPX_VIEWBARBUTTONGROUP_H

#include <QtCore/QObject>

#include <pcx/aligned_store.h>

namespace QPx
{

class ViewBarButton;

class ViewBarButtonGroup : public QObject
{
    Q_OBJECT

public:
    explicit ViewBarButtonGroup(QObject *parent = nullptr);

    ViewBarButton *addButton(ViewBarButton *button);

private slots:
    void buttonDestroyed(QObject *button);
    void buttonToggled(bool state);

private:
    pcx::aligned_store<8> cache;
};

}

#endif // QPX_VIEWBARBUTTONGROUP_H

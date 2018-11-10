#ifndef QPX_LAYOUTS_H
#define QPX_LAYOUTS_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>

namespace QPx
{

class VBoxLayout : public QVBoxLayout
{
    Q_OBJECT

public:
    VBoxLayout(QWidget *parent = nullptr);
};

class HBoxLayout : public QHBoxLayout
{
    Q_OBJECT

public:
    HBoxLayout(QWidget *parent = nullptr);
};

class LayoutWidget : public QWidget
{
    Q_OBJECT

public:
    LayoutWidget(Qt::Orientation orientation, QWidget *parent);

    void addWidget(QWidget *widget);
    template<typename T> T *addTypedWidget(T *widget){ addWidget(widget); return widget; }
};

}

#endif // QPX_LAYOUTS_H

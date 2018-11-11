#ifndef QPX_LAYOUTS_H
#define QPX_LAYOUTS_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSplitter>

namespace QPx
{

class VBoxLayout : public QVBoxLayout
{
    Q_OBJECT

public:
    VBoxLayout(QWidget *parent = nullptr);

    template<typename T> T *addTypedWidget(T *widget){ addWidget(widget); return widget; }
};

class HBoxLayout : public QHBoxLayout
{
    Q_OBJECT

public:
    HBoxLayout(QWidget *parent = nullptr);

    template<typename T> T *addTypedWidget(T *widget){ addWidget(widget); return widget; }
};

class LayoutWidget : public QWidget
{
    Q_OBJECT

public:
    LayoutWidget(Qt::Orientation orientation, QWidget *parent);

    void addWidget(QWidget *widget);
    template<typename T> T *addTypedWidget(T *widget){ addWidget(widget); return widget; }
};

class Splitter : public QSplitter
{
    Q_OBJECT

public:
    Splitter(Qt::Orientation orientation, const QVector<QWidget*> &widgets, QWidget *parent = nullptr);
};

}

#endif // QPX_LAYOUTS_H

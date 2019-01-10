#ifndef QPX_LAYOUTS_H
#define QPX_LAYOUTS_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QSplitter>

namespace QPx
{

class VBoxLayout : public QVBoxLayout
{
    Q_OBJECT

public:
    explicit VBoxLayout(QWidget *parent = nullptr);
    VBoxLayout(int border, int spacing, QWidget *parent = nullptr);

    template<typename T> T *addTypedWidget(T *widget, int stretch = 0){ addWidget(widget, stretch); return widget; }
    template<typename T> T *addTypedLayout(T *layout, int stretch = 0){ addLayout(layout, stretch); return layout; }
};

class HBoxLayout : public QHBoxLayout
{
    Q_OBJECT

public:
    explicit HBoxLayout(QWidget *parent = nullptr);
    HBoxLayout(int border, int spacing, QWidget *parent = nullptr);

    template<typename T> T *addTypedWidget(T *widget, int stretch = 0){ addWidget(widget, stretch); return widget; }
    template<typename T> T *addTypedLayout(T *layout, int stretch = 0){ addLayout(layout, stretch); return layout; }
};

class FormLayout : public QFormLayout
{
public:
    explicit FormLayout(QWidget *parent = nullptr);
    FormLayout(int border, int spacing, QWidget *parent = nullptr);

    template<typename T> T *addTypedWidget(T *widget){ addWidget(widget); return widget; }
    template<typename T> T *addTypedRow(const QString &label, T *widget){ addRow(label, widget); return widget; }
};

class LayoutWidget : public QWidget
{
    Q_OBJECT

public:
    LayoutWidget(Qt::Orientation orientation, QWidget *parent);

    void addWidget(QWidget *widget, int stretch = 0);
    template<typename T> T *addTypedWidget(T *widget, int stretch = 0){ addWidget(widget, stretch); return widget; }
};

class Splitter : public QSplitter
{
    Q_OBJECT

public:
    Splitter(Qt::Orientation orientation, QWidget *parent = nullptr);

    void addWidget(QWidget *widget, int stretch = 0);

    template<typename T> T *addTypedWidget(T *widget, int stretch = 0){ addWidget(widget, stretch); return widget; }
};

class FramedWidget : public QFrame
{
    Q_OBJECT

public:
    FramedWidget(QWidget *widget, QWidget *parent = nullptr);
};

}

#endif // QPX_LAYOUTS_H

#ifndef QPX_LAYOUTS_H
#define QPX_LAYOUTS_H

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

}

#endif // QPX_LAYOUTS_H

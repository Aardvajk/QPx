#ifndef QPX_VIEWSEPARATOR_H
#define QPX_VIEWSEPARATOR_H

#include <QtWidgets/QWidget>

namespace QPx
{

class ViewSeparator : public QWidget
{
    Q_OBJECT

public:
    ViewSeparator(Qt::Orientation orientation, QWidget *parent = nullptr);
};

}

#endif // QPX_VIEWSEPARATOR_H

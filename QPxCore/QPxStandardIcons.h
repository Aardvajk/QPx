#ifndef QPXSTANDARDICONS_H
#define QPXSTANDARDICONS_H

#include <QtGui/QPixmap>
#include <QtGui/QIcon>

namespace QPx
{

namespace StandardIcons
{

enum class Type
{
    New,
    Open,
    Save
};

QPixmap pixmap(Type type, int size = 16, const QColor &color = QColor(90, 90, 90));
QIcon icon(Type type, int size = 16, const QColor &color = QColor(90, 90, 90));

}

}

#endif // QPXSTANDARDICONS_H

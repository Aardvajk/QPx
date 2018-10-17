#include "QPxStandardIcons.h"

#include <QtGui/QImage>
#include <QtGui/QPainter>

namespace
{

ushort codes[] =
{
    59331,
    59575,
    59214
};

}

QPixmap QPx::StandardIcons::pixmap(Type type, int size, const QColor &color)
{
    QImage image(size, size, QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    QPainter painter(&image);

    QFont font = painter.font();

    font.setFamily("Segoe MDL2");
    font.setPixelSize(size);

    painter.setFont(font);
    painter.setPen(color);

    painter.drawText(QRect(0, 0, size, size), QChar(codes[static_cast<int>(type)]));

    return QPixmap::fromImage(image);
}

QIcon QPx::StandardIcons::icon(Type type, int size, const QColor &color)
{
    return QIcon(pixmap(type, size, color));
}


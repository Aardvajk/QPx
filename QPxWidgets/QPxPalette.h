#ifndef QPX_PALETTE_H
#define QPX_PALETTE_H

#include <QtWidgets/QWidget>

namespace QPx
{

void setPaletteColor(QWidget *widget, QPalette::ColorRole role, const QColor &color);

}

#endif // QPX_PALETTE_H

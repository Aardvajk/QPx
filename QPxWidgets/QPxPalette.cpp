#include "QPxWidgets/QPxPalette.h"

void QPx::setPaletteColor(QWidget *widget, QPalette::ColorRole role, const QColor &color)
{
    auto p = widget->palette();
    p.setColor(role, color);

    widget->setPalette(p);
}



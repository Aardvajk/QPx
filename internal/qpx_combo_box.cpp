#include "internal/qpx_combo_box.h"

qpx_combo_box::qpx_combo_box(QWidget *parent) : QComboBox(parent)
{
}

void qpx_combo_box::open()
{
    showPopup();
}


#ifndef QPX_COMBO_BOX_H
#define QPX_COMBO_BOX_H

#include <QtWidgets/QComboBox>

class qpx_combo_box : public QComboBox
{
    Q_OBJECT

public:
    qpx_combo_box(QWidget *parent = nullptr);

public slots:
    void open();
};

#endif // QPX_COMBO_BOX_H

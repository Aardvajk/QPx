#ifndef QPX_RICHTEXTCOMBOBOX_H
#define QPX_RICHTEXTCOMBOBOX_H

#include <QtWidgets/QComboBox>

namespace QPx
{

class RichTextComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit RichTextComboBox(QWidget *parent = 0);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
};

}

#endif // QPX_RICHTEXTCOMBOBOX_H

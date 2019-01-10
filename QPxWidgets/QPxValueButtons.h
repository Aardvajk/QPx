#ifndef QPX_VALUEBUTTONS_H
#define QPX_VALUEBUTTONS_H

#include <QtWidgets/QPushButton>

#include <pcx/aligned_store.h>

namespace QPx
{

class BoolButton : public QPushButton
{
    Q_OBJECT

public:
    BoolButton(const QString &text, bool value, QWidget *parent = nullptr);

signals:
    void valueClicked(bool value);

private slots:
    void buttonClicked();

private:
    pcx::aligned_store<8> cache;
};

}

#endif // QPXVALUEBUTTONS_H

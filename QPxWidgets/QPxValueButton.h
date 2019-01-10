#ifndef QPX_VALUEBUTTON_H
#define QPX_VALUEBUTTON_H

#include <QtCore/QVariant>

#include <QtWidgets/QPushButton>

#include <pcx/aligned_store.h>

namespace QPx
{

class ValueButton : public QPushButton
{
    Q_OBJECT

public:
    ValueButton(const QString &text, const QVariant &value, QWidget *parent = nullptr);

signals:
    void clicked(const QVariant &value);

private slots:
    void buttonClicked();

private:
    pcx::aligned_store<16> cache;
};

}

#endif // QPXVALUEBUTTON_H

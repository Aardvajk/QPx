#ifndef QPX_PROPERTYEDITOR_H
#define QPX_PROPERTYEDITOR_H

#include <QtWidgets/QWidget>

class QLineEdit;

namespace QPx
{

class PropertyEditor : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyEditor(QWidget *parent = nullptr);

    virtual QVariant value() const = 0;
    virtual void setValue(const QVariant &value) = 0;
};

class StringPropertyEditor : public PropertyEditor
{
    Q_OBJECT

public:
    StringPropertyEditor(QWidget *parent = nullptr);

    virtual QVariant value() const override;
    virtual void setValue(const QVariant &value) override;

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

    QLineEdit *edit;
};

class IntPropertyEditor : public StringPropertyEditor
{
    Q_OBJECT

public:
    IntPropertyEditor(QWidget *parent = nullptr);

    virtual QVariant value() const override;
};

}

#endif // QPX_PROPERTYEDITOR_H

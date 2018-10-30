#ifndef QPX_PROPERTYBROWSERITEM_H
#define QPX_PROPERTYBROWSERITEM_H

#include <QtCore/QObject>

#include <pcx/aligned_store.h>

class QWidget;
class QModelIndex;
class QPainter;

namespace QPx
{

class PropertyBrowserModel;
class PropertyBrowserEditor;
class PropertyBrowserDialog;

class PropertyBrowserItem : public QObject
{
    Q_OBJECT

public:
    PropertyBrowserItem(const QString &name, const QVariant &value, QObject *parent = nullptr);

    QString name() const;
    QVariant value() const;

    virtual QString valueText() const;

    virtual void paint(QPainter *painter, const QRect &rect) const;
    virtual PropertyBrowserEditor *createEditor(QWidget *parent) const;
    virtual PropertyBrowserDialog *createDialog(QWidget *parent) const;

signals:
    void valueChanged(const QVariant &value);

public slots:
    virtual void setValue(const QVariant &value);

private:
    pcx::aligned_store<24> cache;
};

class StringPropertyBrowserItem : public PropertyBrowserItem
{
    Q_OBJECT

public:
    StringPropertyBrowserItem(PropertyBrowserModel *model, const QModelIndex &index, const QString &name, const QVariant &value, QObject *parent = nullptr);

    virtual PropertyBrowserEditor *createEditor(QWidget *parent) const override;
};

class IntPropertyBrowserItem : public PropertyBrowserItem
{
    Q_OBJECT

public:
    IntPropertyBrowserItem(PropertyBrowserModel *model, const QModelIndex &index, const QString &name, const QVariant &value, QObject *parent = nullptr);

    virtual PropertyBrowserEditor *createEditor(QWidget *parent) const override;
};

class BoolPropertyBrowserItem : public PropertyBrowserItem
{
    Q_OBJECT

public:
    BoolPropertyBrowserItem(PropertyBrowserModel *model, const QModelIndex &index, const QString &name, const QVariant &value, QObject *parent = nullptr);

    virtual QString valueText() const override;
};

class PointPropertyBrowserItem : public PropertyBrowserItem
{
    Q_OBJECT

public:
    PointPropertyBrowserItem(PropertyBrowserModel *model, const QModelIndex &index, const QString &name, const QVariant &value, QObject *parent = nullptr);

    virtual QString valueText() const;

public slots:
    virtual void setValue(const QVariant &value) override;

private slots:
    void changed(const QVariant &value);

private:
    pcx::aligned_store<32> cache;
};

class ColorPropertyBrowserItem : public PropertyBrowserItem
{
    Q_OBJECT

public:
    ColorPropertyBrowserItem(PropertyBrowserModel *model, const QModelIndex &index, const QString &name, const QVariant &value, QObject *parent = nullptr);

    virtual void paint(QPainter *painter, const QRect &rect) const override;
    virtual PropertyBrowserDialog *createDialog(QWidget *parent) const override;
};

}

#endif // QPX_PROPERTYBROWSERITEM_H

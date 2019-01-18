#ifndef QPX_PROPERTYBROWSERTYPE_H
#define QPX_PROPERTYBROWSERTYPE_H

#include <QtCore/QObject>
#include <QtCore/QVariant>

#include <pcx/aligned_store.h>

class QWidget;
class QPainter;
class QStyleOptionViewItem;

namespace QPx
{

class PropertyBrowserItem;
class PropertyBrowserModel;
class PropertyBrowserEditor;
class PropertyBrowserDialog;

class PropertyBrowserType : public QObject
{
    Q_OBJECT

public:
    explicit PropertyBrowserType(QObject *parent = nullptr);

    virtual void addProperties(PropertyBrowserItem *item, PropertyBrowserModel *model, const QModelIndex &parent) const;
    virtual void updateProperties(PropertyBrowserItem *item, const QVariant &value) const;

    virtual QString valueText(const PropertyBrowserItem *item) const;

    virtual bool readOnly() const;

    virtual bool validate(const PropertyBrowserItem *item, const QVariant &value) const;

    virtual void paint(const PropertyBrowserItem *item, QPainter *painter, const QStyleOptionViewItem &option) const;

    virtual PropertyBrowserEditor *createEditor(const PropertyBrowserItem *item, QWidget *parent) const;
    virtual PropertyBrowserDialog *createDialog(const PropertyBrowserItem *item, QWidget *parent) const;
};

class GroupPropertyBrowserType : public PropertyBrowserType
{
    Q_OBJECT

public:
    explicit GroupPropertyBrowserType(QObject *parent = nullptr);

    virtual bool readOnly() const override;
};

class StringPropertyBrowserType : public PropertyBrowserType
{
    Q_OBJECT

public:
    explicit StringPropertyBrowserType(QObject *parent = nullptr);

    virtual PropertyBrowserEditor *createEditor(const PropertyBrowserItem *item, QWidget *parent) const override;
};

class NumericPropertyBrowserType : public PropertyBrowserType
{
    Q_OBJECT

public:
    explicit NumericPropertyBrowserType(QObject *parent = nullptr);
    NumericPropertyBrowserType(const QVariant &min, const QVariant &max, QObject *parent = nullptr);

protected:
    pcx::aligned_store<32> cache;
};

class IntPropertyBrowserType : public NumericPropertyBrowserType
{
    Q_OBJECT

public:
    using NumericPropertyBrowserType::NumericPropertyBrowserType;

    virtual bool validate(const PropertyBrowserItem *item, const QVariant &value) const override;

    virtual PropertyBrowserEditor *createEditor(const PropertyBrowserItem *item, QWidget *parent) const override;
};

class FloatPropertyBrowserType : public NumericPropertyBrowserType
{
    Q_OBJECT

public:
    using NumericPropertyBrowserType::NumericPropertyBrowserType;

    virtual QString valueText(const PropertyBrowserItem *item) const override;

    virtual bool validate(const PropertyBrowserItem *item, const QVariant &value) const override;

    virtual PropertyBrowserEditor *createEditor(const PropertyBrowserItem *item, QWidget *parent) const override;
};

class BoolPropertyBrowserType : public PropertyBrowserType
{
    Q_OBJECT

public:
    explicit BoolPropertyBrowserType(QObject *parent = nullptr);

    virtual QString valueText(const PropertyBrowserItem *item) const override;
};

class AbstractEnumPropertyBrowserType : public PropertyBrowserType
{
    Q_OBJECT

public:
    AbstractEnumPropertyBrowserType(const QStringList &values, QObject *parent = nullptr);
    AbstractEnumPropertyBrowserType(const QVector<QPair<int, QString> > &values, QObject *parent = nullptr);

    virtual QString valueText(const PropertyBrowserItem *item) const override;

    virtual PropertyBrowserEditor *createEditor(const PropertyBrowserItem *item, QWidget *parent) const override;
    virtual QVariant toEnumValue(const QVariant &value) const = 0;

private:
    pcx::aligned_store<64> cache;
};

template<typename T> class EnumPropertyBrowserType : public AbstractEnumPropertyBrowserType
{
public:
    EnumPropertyBrowserType(const QStringList &values, QObject *parent = nullptr) : AbstractEnumPropertyBrowserType(values, parent) { }
    EnumPropertyBrowserType(const QVector<QPair<T, QString> > &values, QObject *parent = nullptr) : AbstractEnumPropertyBrowserType(convert(values), parent) { }

    virtual QVariant toEnumValue(const QVariant &value) const override { return QVariant::fromValue(static_cast<T>(value.toInt())); }

private:
    static QVector<QPair<int, QString> > convert(const QVector<QPair<T, QString> > &values){ QVector<QPair<int, QString> > r; for(auto i: values) r.append(qMakePair(static_cast<int>(i.first), i.second)); return r; }
};

class AbstractFlagPropertyBrowserType : public PropertyBrowserType
{
    Q_OBJECT

public:
    AbstractFlagPropertyBrowserType(const QStringList &values, QObject *parent = nullptr);

    virtual void addProperties(PropertyBrowserItem *item, PropertyBrowserModel *model, const QModelIndex &parent) const override;
    virtual void updateProperties(PropertyBrowserItem *item, const QVariant &value) const override;

    virtual QString valueText(const PropertyBrowserItem *item) const override;
    virtual bool readOnly() const override;

    virtual QVariant toFlagValue(unsigned value) const = 0;
    virtual unsigned toUnsigned(const QVariant &value) const = 0;

private slots:
    void changed(const QVariant &value);

private:
    pcx::aligned_store<64> cache;
};

template<typename F, typename T> class FlagPropertyBrowserType : public AbstractFlagPropertyBrowserType
{
public:
    FlagPropertyBrowserType(const QStringList &values, QObject *parent = nullptr) : AbstractFlagPropertyBrowserType(values, parent) { }

    virtual QVariant toFlagValue(unsigned value) const { return QVariant::fromValue(T(static_cast<F>(value))); }
    virtual unsigned toUnsigned(const QVariant &value) const { return static_cast<unsigned>(static_cast<F>(value.value<T>())); }
};

class ColorPropertyBrowserType : public PropertyBrowserType
{
    Q_OBJECT

public:
    explicit ColorPropertyBrowserType(QObject *parent = nullptr);

    virtual void paint(const PropertyBrowserItem *item, QPainter *painter, const QStyleOptionViewItem &option) const override;

    virtual PropertyBrowserDialog *createDialog(const PropertyBrowserItem *item, QWidget *parent) const override;
};

class PointPropertyBrowserType : public PropertyBrowserType
{
    Q_OBJECT

public:
    PointPropertyBrowserType(QObject *parent = nullptr);

    virtual void addProperties(PropertyBrowserItem *item, PropertyBrowserModel *model, const QModelIndex &parent) const override;
    virtual void updateProperties(PropertyBrowserItem *item, const QVariant &value) const override;

    virtual QString valueText(const PropertyBrowserItem *item) const override;
    virtual bool readOnly() const override;

private slots:
    void changed(const QVariant &value);

private:
    pcx::aligned_store<8> cache;
};

}

#endif // QPX_PROPERTYBROWSERTYPE_H

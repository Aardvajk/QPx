#include "QPxPropertyBrowser/QPxPropertyBrowserEditor.h"

#include "QPxPropertyBrowser/QPxPropertyBrowserType.h"

#include "internal/qpx_combo_box.h"

#include "QPxWidgets/QPxLayouts.h"

#include <QtCore/QTimer>

#include <QtGui/QIntValidator>
#include <QtGui/QDoubleValidator>

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCheckBox>

namespace
{

class EditCache
{
public:
    EditCache(QWidget *parent) : edit(new QLineEdit(parent)) { }

    QLineEdit *edit;
};

class EnumCache
{
public:
    EnumCache(const QPx::AbstractEnumPropertyBrowserType *type, QWidget *parent) : type(type), combo(new qpx_combo_box(parent)) { }

    const QPx::AbstractEnumPropertyBrowserType *type;
    QComboBox *combo;
};

template<typename T, typename V> V *setupValidator(const QVariant &min, const QVariant &max, V *validator)
{
    if(min.isValid()) validator->setBottom(min.value<T>());
    if(max.isValid()) validator->setTop(max.value<T>());

    return validator;
}

template<typename T> T fromString(const QString &value, bool &ok);

template<> int fromString(const QString &value, bool &ok){ return value.toInt(&ok); }
template<> float fromString(const QString &value, bool &ok){ return value.toFloat(&ok); }

template<typename T> QVariant lineEditValue(const QString &text)
{
    bool ok = false;
    T value = fromString<T>(text, ok);

    return ok ? value : QVariant();
}

}

QPx::PropertyBrowserEditor::PropertyBrowserEditor(QWidget *parent) : QWidget(parent)
{
}

QPx::StringPropertyBrowserEditor::StringPropertyBrowserEditor(QWidget *parent) : PropertyBrowserEditor(parent)
{
    auto c = cache.alloc<EditCache>(this);

    auto layout = new QPx::HBoxLayout(0, 2, this);

    layout->addWidget(c.edit);
    setFocusProxy(c.edit);
}

QVariant QPx::StringPropertyBrowserEditor::value() const
{
    return cache.get<EditCache>().edit->text();
}

void QPx::StringPropertyBrowserEditor::setValue(const QVariant &value)
{
    cache.get<EditCache>().edit->setText(value.isValid() ? value.toString() : QString());
}

QPx::IntPropertyBrowserEditor::IntPropertyBrowserEditor(const QVariant &min, const QVariant &max, QWidget *parent) : StringPropertyBrowserEditor(parent)
{
    cache.get<EditCache>().edit->setValidator(setupValidator<int>(min, max, new QIntValidator(this)));
}

QVariant QPx::IntPropertyBrowserEditor::value() const
{
    return lineEditValue<int>(cache.get<EditCache>().edit->text());
}

QPx::FloatPropertyBrowserEditor::FloatPropertyBrowserEditor(const QVariant &min, const QVariant &max, QWidget *parent) : StringPropertyBrowserEditor(parent)
{
    cache.get<EditCache>().edit->setValidator(setupValidator<float>(min, max, new QDoubleValidator(this)));
}

QVariant QPx::FloatPropertyBrowserEditor::value() const
{
    return lineEditValue<float>(cache.get<EditCache>().edit->text());
}

void QPx::FloatPropertyBrowserEditor::setValue(const QVariant &value)
{
    cache.get<EditCache>().edit->setText(QString::number(value.toFloat()));
}

QPx::EnumPropertyBrowserEditor::EnumPropertyBrowserEditor(const AbstractEnumPropertyBrowserType *type, const QMap<int, QString> &values, QWidget *parent) : PropertyBrowserEditor(parent)
{
    auto &c = cache.alloc<EnumCache>(type, this);

    auto layout = new QPx::VBoxLayout(this);

    foreach(int i, values.keys())
    {
        c.combo->addItem(values[i], i);
    }

    layout->addWidget(c.combo);
    setFocusProxy(c.combo);

    connect(c.combo, SIGNAL(currentIndexChanged(int)), SIGNAL(commit()));

    QTimer::singleShot(0, static_cast<qpx_combo_box*>(c.combo), SLOT(open()));
}

QVariant QPx::EnumPropertyBrowserEditor::value() const
{
    auto &c = cache.get<EnumCache>();
    return c.type->toEnumValue(c.combo->itemData(c.combo->currentIndex()));
}

void QPx::EnumPropertyBrowserEditor::setValue(const QVariant &value)
{
    auto &c = cache.get<EnumCache>();

    for(int i = 0; i < c.combo->count(); ++i)
    {
        if(c.combo->itemData(i) == value.toInt())
        {
            c.combo->setCurrentIndex(i);
            return;
        }
    }

    c.combo->setCurrentIndex(-1);
}

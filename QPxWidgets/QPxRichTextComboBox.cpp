#include "QPxWidgets/QPxRichTextComboBox.h"

#include <QtWidgets/QStylePainter>
#include <QtWidgets/QStyleOptionComboBox>

#include <QtWidgets/QAbstractItemView>
#include <QtWidgets/QStyledItemDelegate>

#include <QtGui/QTextDocument>
#include <QtGui/QAbstractTextDocumentLayout>

#include <QtWidgets/QApplication>

namespace
{

QRect makeTextRect(const QRect &rect, const QFontMetrics &metrics)
{
    QRect r = rect;

    int margin = (rect.height() - metrics.height()) / 2;

    r.setTop(r.top() + margin);
    r.setBottom((r.top() + metrics.height()));

    return r;
}

void drawRichText(QPainter &painter, const QString &text, const QRect &rect, const QColor &color)
{
    QTextDocument doc;
    doc.setHtml(QString("<span style=\"color:rgb(%1,%2,%3)\">%4</span>").arg(color.red()).arg(color.green()).arg(color.blue()).arg(text));
    doc.setDocumentMargin(0);

    QAbstractTextDocumentLayout::PaintContext ctx;

    painter.save();

    painter.setClipRect(rect);
    painter.translate(rect.topLeft());

    doc.documentLayout()->draw(&painter, ctx);

    painter.restore();
}

class ComboDelegate : public QStyledItemDelegate
{
public:
    ComboDelegate(QObject *parent = 0) : QStyledItemDelegate(parent) { }

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

void ComboDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 opt = option;
    initStyleOption(&opt, index);

    QStyle *style = opt.widget? opt.widget->style() : QApplication::style();

    opt.text = QString();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter);

    drawRichText(*painter, index.data(Qt::DisplayRole).toString(), makeTextRect(opt.rect.adjusted(2, 0, -2, 0), opt.fontMetrics), qvariant_cast<QColor>(index.data(Qt::TextColorRole)));
}

}

QPx::RichTextComboBox::RichTextComboBox(QWidget *parent) : QComboBox(parent)
{
    view()->setItemDelegate(new ComboDelegate(view()));
}

void QPx::RichTextComboBox::paintEvent(QPaintEvent *event)
{
    QStylePainter painter(this);

    QStyleOptionComboBox opt;
    initStyleOption(&opt);
    painter.drawComplexControl(QStyle::CC_ComboBox, opt);

    drawRichText(painter, currentText(), makeTextRect(opt.rect.adjusted(4, 0, -14, 0), opt.fontMetrics), palette().color(QPalette::Text));
}

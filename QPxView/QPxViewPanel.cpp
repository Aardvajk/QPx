#include "QPxView/QPxViewPanel.h"

#include "QPxView/QPxViewBar.h"
#include "QPxView/QPxViewBarButton.h"
#include "QPxView/QPxViewBarButtonGroup.h"
#include "QPxView/QPxViewSeparator.h"

#include "QPxWidgets/QPxLayouts.h"
#include "QPxWidgets/QPxPalette.h"

#include <QtCore/QTimer>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMenu>

QPx::ViewPanel::ViewPanel(QWidget *parent) : QPx::SplitterPanel(parent)
{
    setPaletteColor(this, QPalette::Background, qvariant_cast<QColor>(QApplication::instance()->property("qpx-view-ui-border")));

    auto layout = new VBoxLayout(this);

    bar = layout->addTypedWidget(new ViewBar(Qt::Horizontal, ViewBar::Type::Small));

    auto icon = QPixmap(":/resources/images/ark.png").scaledToHeight(16, Qt::SmoothTransformation);

    auto group = new QPx::ViewBarButtonGroup(bar);

    bar->addWidget(group->addButton(new ViewBarButton(icon, bar)));
    bar->addWidget(group->addButton(new ViewBarButton(icon, bar)));
    bar->addWidget(group->addButton(new ViewBarButton(icon, bar)));

    bar->addStretch();
    auto button = bar->addTypedWidget(new ViewBarButton(icon, bar));

    auto menu = new QMenu(button);
    button->setMenu(menu);

    menu->addAction(QIcon(":/resources/images/splitvert.png"), "Split Vertical", this, SLOT(splitVertical()));
    menu->addAction(QIcon(":/resources/images/splithorz.png"), "Split Horizontal", this, SLOT(splitHorizontal()));
    menu->addSeparator();
    menu->addAction("Close", this, SLOT(closePanel()));
}

QPx::ViewBar *QPx::ViewPanel::viewBar() const
{
    return bar;
}

void QPx::ViewPanel::splitVertical()
{
    split(Qt::Vertical, clone());
}

void QPx::ViewPanel::splitHorizontal()
{
    split(Qt::Horizontal, clone());
}

void QPx::ViewPanel::closePanel()
{
    QTimer::singleShot(0, this, SLOT(close()));
}

#include "QPxWidgets/QPxSplitterContainer.h"

#include "QPxWidgets/QPxLayouts.h"
#include "QPxWidgets/QPxSplitterPanel.h"

namespace
{

class Cache
{
public:

};

}

QPx::SplitterContainer::SplitterContainer(SplitterPanel *panel, QWidget *parent) : QWidget(parent)
{
    new VBoxLayout(this);
    layout()->addWidget(panel);
}

#include "QPxView/QPxViewBarButtonGroup.h"

#include "QPxView/QPxViewBarButton.h"

#include <algorithm>

namespace
{

class Cache
{
public:
    QList<QPx::ViewBarButton*> buttons;
};

}

QPx::ViewBarButtonGroup::ViewBarButtonGroup(QObject *parent) : QObject(parent)
{
    cache.alloc<Cache>();
}

QPx::ViewBarButton *QPx::ViewBarButtonGroup::addButton(ViewBarButton *button)
{
    cache.get<Cache>().buttons.append(button);
    button->setCheckable(true);

    connect(button, SIGNAL(destroyed(QObject*)), SLOT(buttonDestroyed(QObject*)));
    connect(button, SIGNAL(toggled(bool)), SLOT(buttonToggled(bool)));

    return button;
}

void QPx::ViewBarButtonGroup::buttonDestroyed(QObject *button)
{
    auto &b = cache.get<Cache>().buttons;
    b.erase(std::remove(b.begin(), b.end(), static_cast<ViewBarButton*>(button)), b.end());
}

void QPx::ViewBarButtonGroup::buttonToggled(bool state)
{
    if(state)
    {
        for(auto &button: cache.get<Cache>().buttons)
        {
            if(button != sender())
            {
                button->setChecked(false);
            }
        }
    }
}

#include "QPxCore/QPxMainWindow.h"

#include "QPxCore/QPxSettings.h"

#include "QPxActions/QPxAction.h"
#include "QPxActions/QPxActionList.h"

#include <QtCore/QHash>

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtWidgets/QToolBar>

namespace
{

enum class Special { Menu, Separator, Unknown };

Special special(const QString &key)
{
    static const QHash<QString, Special> m = { { "[Menu]", Special::Menu }, { "[Separator]", Special::Separator } };
    return m.value(key, Special::Unknown);
}

template<typename T> void loadMenus(const QPx::Settings &root, QPx::ActionList *actions, T *parent, QPx::MainWindow *window, void(QPx::MainWindow::*custom)(const QString&,QWidget*))
{
    for(auto &menu: root)
    {
        switch(special(menu.key()))
        {
            case Special::Menu: loadMenus(menu, actions, parent->addMenu(menu.value().toString()), window, custom); break;
            case Special::Separator: parent->addSeparator(); break;

            case Special::Unknown:
            {
                if(auto a = actions->find(menu.key()))
                {
                    parent->addAction(a);
                }
                else
                {
                    (window->*custom)(menu.key(), parent);
                }
            }
        }
    }
}

void loadToolBars(const QPx::Settings &root, QPx::ActionList *actions, QPx::MainWindow *window, void(QPx::MainWindow::*custom)(const QString&,QWidget*))
{
    for(auto &section: root)
    {
        auto toolbar = window->addToolBar(section.value().toString());

        toolbar->setIconSize(QSize(16, 16));
        toolbar->setMovable(false);

        for(auto &action: section)
        {
            switch(special(action.key()))
            {
                case Special::Menu: break;
                case Special::Separator: toolbar->addSeparator(); break;

                case Special::Unknown:
                {
                    if(auto a = actions->find(action.key()))
                    {
                        toolbar->addAction(a);
                    }
                    else
                    {
                        (window->*custom)(action.key(), toolbar);
                    }
                }
            }
        }
    }
}

}

QPx::MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
}

void QPx::MainWindow::loadInterface(const QString &path, ActionList *actions)
{
    SettingsMap settings(path);

    loadMenus(settings["Menus"], actions, menuBar(), this, &customInterfaceAction);
    loadToolBars(settings["Toolbars"], actions, this, &customInterfaceAction);
}

void QPx::MainWindow::customInterfaceAction(const QString &key, QWidget *parent)
{
}


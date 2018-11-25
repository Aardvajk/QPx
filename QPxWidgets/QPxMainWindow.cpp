#include "QPxWidgets/QPxMainWindow.h"

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

template<typename T> void loadMenus(const QPx::Settings &menus, QPx::ActionList *actions, T *parent, QPx::MainWindow *window, void(QPx::MainWindow::*custom)(const QString&,QWidget*))
{
    for(int i = 0; i < menus.count(); ++i)
    {
        auto key = menus[i].key();

        switch(special(key))
        {
            case Special::Menu: loadMenus(menus[i], actions, parent->addMenu(menus[i].value().toString()), window, custom); break;
            case Special::Separator: parent->addSeparator(); break;

            case Special::Unknown:
            {
                if(auto a = actions->find(key))
                {
                    parent->addAction(a);
                }
                else
                {
                    (window->*custom)(key, parent);
                }
            }
        }
    }
}

void loadToolBars(const QPx::Settings &toolBars, QPx::ActionList *actions, QPx::MainWindow *window, void(QPx::MainWindow::*custom)(const QString&,QWidget*))
{
    for(int i = 0; i < toolBars.count(); ++i)
    {
        auto toolbar = window->addToolBar(toolBars[i].value().toString());

        toolbar->setIconSize(QSize(16, 16));
        toolbar->setMovable(false);

        for(int j = 0; j < toolBars[i].count(); ++j)
        {
            auto key = toolBars[i][j].key();

            switch(special(key))
            {
                case Special::Menu: break;
                case Special::Separator: toolbar->addSeparator(); break;

                case Special::Unknown:
                {
                    if(auto a = actions->find(key))
                    {
                        toolbar->addAction(a);
                    }
                    else
                    {
                        (window->*custom)(key, toolbar);
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
    Settings settings;

    if(settings.load(path))
    {
        loadMenus(settings["Menus"], actions, menuBar(), this, &customInterfaceAction);
        loadToolBars(settings["ToolBars"], actions, this, &customInterfaceAction);
    }
}

void QPx::MainWindow::customInterfaceAction(const QString &key, QWidget *parent)
{
}


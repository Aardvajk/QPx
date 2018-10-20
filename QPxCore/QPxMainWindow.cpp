#include "QPxCore/QPxMainWindow.h"

#include "QPxCore/QPxSettings.h"

#include "QPxActions/QPxAction.h"
#include "QPxActions/QPxActionList.h"

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtWidgets/QToolBar>

namespace
{

enum class Special { Menu, Separator, Unknown };

Special special(const QString &key)
{
    static const QMap<QString, Special> m = { { "[Menu]", Special::Menu }, { "[Separator]", Special::Separator } };
    return m.value(key, Special::Unknown);
}

template<typename T> void loadMenus(QPx::Settings &root, QPx::ActionList *actions, T *parent, QPx::MainWindow *window, void(QPx::MainWindow::*custom)(const QString&,QWidget*))
{
    for(int i = 0; i < root.count(); ++i)
    {
        auto key = root[i].key();

        switch(special(key))
        {
            case Special::Menu: loadMenus(root[i], actions, parent->addMenu(root[i].value().toString()), window, custom); break;
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

void loadToolBars(QPx::Settings &root, QPx::ActionList *actions, QPx::MainWindow *window, void(QPx::MainWindow::*custom)(const QString&,QWidget*))
{
    for(int i = 0; i < root.count(); ++i)
    {
        auto toolbar = window->addToolBar(root[i].value().toString());

        toolbar->setIconSize(QSize(16, 16));
        toolbar->setMovable(false);

        for(int j = 0; j < root[i].count(); ++j)
        {
            auto key = root[i][j].key();

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
    SettingsMap settings(path);

    loadMenus(settings["Menus"], actions, menuBar(), this, &customInterfaceAction);
    loadToolBars(settings["Toolbars"], actions, this, &customInterfaceAction);
}

void QPx::MainWindow::customInterfaceAction(const QString &key, QWidget *parent)
{
}


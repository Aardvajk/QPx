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

template<typename T> void loadMenus(QPx::Settings &root, QPx::ActionList *actions, T *parent, QPx::MainWindow *window, void(QPx::MainWindow::*custom)(const QString&,QWidget*))
{
    for(int i = 0; i < root.count(); ++i)
    {
        auto key = root[i].key();

        if(key == "[Menu]")
        {
            loadMenus(root[i], actions, parent->addMenu(root[i].value().toString()), window, custom);
        }
        else if(key == "[Separator]")
        {
            parent->addSeparator();
        }
        else
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

void loadToolBars(QPx::Settings &root, QPx::ActionList *actions, QPx::MainWindow *window, void(QPx::MainWindow::*custom)(const QString&,QWidget*))
{
    for(int i = 0; i < root.count(); ++i)
    {
        auto toolbar = window->addToolBar("");

        toolbar->setIconSize(QSize(16, 16));
        toolbar->setMovable(false);

        for(int j = 0; j < root[i].count(); ++j)
        {
            auto key = root[i][j].key();

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


#ifndef QPX_MAINWINDOW_H
#define QPX_MAINWINDOW_H

#include <QtWidgets/QMainWindow>

namespace QPx
{

class ActionList;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    void loadInterface(const QString &path, QPx::ActionList *actions);

protected:
    virtual void customInterfaceAction(const QString &key, QWidget *parent);
};

}
#endif // QPX_MAINWINDOW_H

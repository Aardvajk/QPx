#include "QPxEditor/QPxEditorFileActions.h"

#include "QPxEditor/QPxAbstractEditorModel.h"

#include <QtCore/QStringList>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenu>

namespace
{

class NotSavedDialog : public QMessageBox
{
public:
    NotSavedDialog(QWidget *parent);
};

NotSavedDialog::NotSavedDialog(QWidget *parent) : QMessageBox(parent)
{
    setWindowTitle(QCoreApplication::applicationName());
    setIcon(QMessageBox::Question);

    setText("The current world has not been saved.");
    setInformativeText("Do you want to save it?");

    setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
}

class RecentFileNotOpenedDialog : public QMessageBox
{
public:
    RecentFileNotOpenedDialog(const QString &path, QWidget *parent);
};

RecentFileNotOpenedDialog::RecentFileNotOpenedDialog(const QString &path, QWidget *parent) : QMessageBox(parent)
{
    setWindowTitle(QCoreApplication::applicationName());
    setIcon(QMessageBox::Question);

    setText(QString("<b>%1</b> could not be found.").arg(QFileInfo(path).fileName()));
    setInformativeText("Do you want to remove it from the Recent Files list?");

    setStandardButtons(QMessageBox::Yes | QMessageBox::No);
}

void failDialog(const QString &operation, const QString &path, QWidget *widget)
{
    QMessageBox mb(QMessageBox::Critical, QApplication::applicationName(), QString("Unable to %1 the file: %2").arg(operation).arg(path), QMessageBox::Close, widget);
    mb.exec();
}

class Cache
{
public:
    Cache(QPx::AbstractEditorModel *model, QWidget *parent) : model(model), recentMenu(new QMenu("Recent &Files", parent)) { }

    QPx::AbstractEditorModel *model;
    QMenu *recentMenu;
    QStringList recents;
};

}

QPx::EditorFileActions::EditorFileActions(AbstractEditorModel *model, QWidget *parent) : QObject(parent)
{
    cache.alloc<Cache>(model, parent);
    updateRecentMenu();
}

bool QPx::EditorFileActions::canClose()
{
    if(cache.get<Cache>().model->isModified())
    {
        NotSavedDialog dialog(static_cast<QWidget*>(parent()));
        
        switch(dialog.exec())
        {
            case QMessageBox::Yes: return save();
            case QMessageBox::Cancel: return false;

            default: break;
        }
    }

    return true;
}

QMenu *QPx::EditorFileActions::recentFilesMenu()
{
    return cache.get<Cache>().recentMenu;
}

void QPx::EditorFileActions::setRecentFiles(const QStringList &files)
{
    cache.get<Cache>().recents = files;
    updateRecentMenu();
}

bool QPx::EditorFileActions::clear()
{
    if(!canClose())
    {
        return false;
    }

    auto m = cache.get<Cache>().model;

    if(!m->clear())
    {
        return false;
    }

    m->setPath(QString());
    m->setSavePoint();

    return true;
}

bool QPx::EditorFileActions::open()
{
    QString path = QFileDialog::getOpenFileName(static_cast<QWidget*>(parent()), QApplication::applicationName());
    if(path.isEmpty() || !canClose())
    {
        return false;
    }

    auto m = cache.get<Cache>().model;

    if(!m->open(path))
    {
        failDialog("open", path, static_cast<QWidget*>(parent()));
        return false;
    }

    m->setPath(path);
    m->setSavePoint();
    
    fileUsed(path);

    return true;
}

bool QPx::EditorFileActions::save()
{
    auto m = cache.get<Cache>().model;

    if(m->path().isEmpty())
    {
        return saveAs();
    }

    if(!m->save(m->path()))
    {
        failDialog("save", m->path(), static_cast<QWidget*>(parent()));
        return false;
    }

    m->setSavePoint();
    return true;
}

bool QPx::EditorFileActions::saveAs()
{
    QString path = QFileDialog::getSaveFileName(static_cast<QWidget*>(parent()), QApplication::applicationName());
    if(path.isEmpty())
    {
        return false;
    }

    auto m = cache.get<Cache>().model;

    if(!m->save(path))
    {
        failDialog("save", path, static_cast<QWidget*>(parent()));
        return false;
    }

    m->setPath(path);
    m->setSavePoint();
    
    fileUsed(path);

    return true;
}

void QPx::EditorFileActions::recentFileRequested()
{
    auto &c = cache.get<Cache>();

    QString path = static_cast<QAction*>(sender())->text();
    if(path == c.model->path())
    {
        return;
    }

    if(!QFile::exists(path))
    {
        RecentFileNotOpenedDialog dialog(path, static_cast<QWidget*>(parent()));
        if(dialog.exec() == QMessageBox::Yes)
        {
            c.recents.removeAll(path);
            updateRecentMenu();
        }

        return;
    }

    if(!canClose())
    {
        return;
    }

    if(!c.model->open(path))
    {
        failDialog("open", path, static_cast<QWidget*>(parent()));
        return;
    }

    c.model->setPath(path);
    c.model->setSavePoint();

    fileUsed(path);
}

void QPx::EditorFileActions::recentClearClicked()
{
    cache.get<Cache>().recents.clear();
    updateRecentMenu();
}

void QPx::EditorFileActions::updateRecentMenu()
{
    auto &c = cache.get<Cache>();

    c.recentMenu->clear();

    if(c.recents.isEmpty())
    {
        QAction *noneAction = new QAction("None", c.recentMenu);
        noneAction->setEnabled(false);

        c.recentMenu->addAction(noneAction);
    }
    else
    {
        foreach(const QString &file, c.recents)
        {
            QAction *action = new QAction(file, c.recentMenu);
            c.recentMenu->addAction(action);

            connect(action, SIGNAL(triggered()), SLOT(recentFileRequested()));
        }

        c.recentMenu->addSeparator();

        QAction *clearAction = new QAction("Clear Menu", c.recentMenu);
        connect(clearAction, SIGNAL(triggered()), SLOT(recentClearClicked()));

        c.recentMenu->addAction(clearAction);
    }
}

void QPx::EditorFileActions::fileUsed(const QString &path)
{
    auto &c = cache.get<Cache>();

    c.recents.removeAll(path);
    c.recents.insert(0, path);

    while(c.recents.count() > 10)
    {
        c.recents.removeLast();
    }

    updateRecentMenu();
}

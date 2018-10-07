#include "QPxEditor/QPxAbstractEditorModel.h"

#include "QPxEditor/QPxAbstractEditorCommand.h"

#include "internal/qpx_undolist.h"

namespace
{

class Cache
{
public:
    Cache(){ }
    
    qpx_undolist undo;
};

}

#include <QtCore/QDebug>

QPx::AbstractEditorModel::AbstractEditorModel(QObject *parent) : QObject(parent)
{
    cache.alloc<Cache>();

qDebug() << sizeof(Cache);
    
    auto &u = cache.get<Cache>().undo;
    connect(&u, SIGNAL(modifiedStateChanged(bool)), SIGNAL(modifiedStateChanged(bool)));
    connect(&u, SIGNAL(undoStateChanged()), SIGNAL(undoStateChanged()));
}

void QPx::AbstractEditorModel::endCommand(QPx::AbstractEditorCommand *command)
{
    if(command->isValid())
    {
        cache.get<Cache>().undo.addCommand(command);
    }
    else
    {
        delete command;
    }
}

void QPx::AbstractEditorModel::setSavePoint()
{
    cache.get<Cache>().undo.setSavePoint();
}

bool QPx::AbstractEditorModel::isModified() const
{
    return cache.get<Cache>().undo.isModified();
}

bool QPx::AbstractEditorModel::canUndo() const
{
    return cache.get<Cache>().undo.canUndo();
}

bool QPx::AbstractEditorModel::canRedo() const
{
    return cache.get<Cache>().undo.canRedo();
}

QString QPx::AbstractEditorModel::lastCommandName() const
{
    return cache.get<Cache>().undo.lastCommandName();
}

QString QPx::AbstractEditorModel::nextCommandName() const
{
    return cache.get<Cache>().undo.nextCommandName();
}

void QPx::AbstractEditorModel::undo()
{
    cache.get<Cache>().undo.undo();
}

void QPx::AbstractEditorModel::redo()
{
    cache.get<Cache>().undo.redo();
}

void QPx::AbstractEditorModel::clearUndoList()
{
    cache.get<Cache>().undo.clear();
}

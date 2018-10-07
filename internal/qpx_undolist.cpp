#include "internal/qpx_undolist.h"

#include "QPxEditor/QPxAbstractEditorCommand.h"

#include <QtCore/QVector>

namespace
{

class Cache
{
public:
    Cache() : pos(-1), save(-1) { }

    QVector<QPx::AbstractEditorCommand*> data;
    
    int pos;
    int save;    
};
    
}

qpx_undolist::qpx_undolist(QObject *parent) : QObject(parent)
{
    cache.alloc<Cache>();
}

void qpx_undolist::addCommand(QPx::AbstractEditorCommand *command)
{
    auto &c = cache.get<Cache>();

    bool last = isModified();

    if(c.data.count() > c.pos + 1)
    {
        for(int i = c.pos + 1; i < c.data.count(); ++i)
        {
            delete c.data[i];
        }

        c.data.remove(c.pos + 1, c.data.count() - (c.pos + 1));
    }

    command->setParent(this);

    c.data.append(command);
    c.pos = c.data.count() - 1;

    emit undoStateChanged();

    if(last != isModified())
    {
        emit modifiedStateChanged(!last);
    }
}

void qpx_undolist::clear()
{
    auto &c = cache.get<Cache>();

    qDeleteAll(c.data);
    c = { };

    emit undoStateChanged();
    emit modifiedStateChanged(false);
}

bool qpx_undolist::canUndo() const
{
    return cache.get<Cache>().pos >= 0;
}

bool qpx_undolist::canRedo() const
{
    return cache.get<Cache>().pos < cache.get<Cache>().data.count() - 1;
}

bool qpx_undolist::isModified() const
{
    auto &c = cache.get<Cache>();

    if(c.save < c.pos)
    {
        for(int i = c.save + 1; i <= c.pos; ++i)
        {
            if(i >= 0 && c.data[i]->modifiesPersistentState()) return true;
        }
    }
    else
    {
        for(int i = c.save; i > c.pos; --i)
        {
            if(i >= 0 && c.data[i]->modifiesPersistentState()) return true;
        }
    }

    return false;
}

QString qpx_undolist::lastCommandName() const
{
    auto &c = cache.get<Cache>();
    return canUndo() ? c.data[c.pos]->name() : QString();
}

QString qpx_undolist::nextCommandName() const
{
    auto &c = cache.get<Cache>();
    return canRedo() ? c.data[c.pos + 1]->name() : QString();
}

void qpx_undolist::undo()
{
    auto &c = cache.get<Cache>();
    bool last = isModified();

    c.data[c.pos--]->undo();

    emit undoStateChanged();

    if(last != isModified())
    {
        emit modifiedStateChanged(!last);
    }
}

void qpx_undolist::redo()
{
    auto &c = cache.get<Cache>();
    bool last = isModified();

    c.data[++c.pos]->redo();

    emit undoStateChanged();

    if(last != isModified())
    {
        emit modifiedStateChanged(!last);
    }
}

void qpx_undolist::setSavePoint()
{
    auto &c = cache.get<Cache>();
    bool last = isModified();

    c.save = c.pos;

    if(last != isModified())
    {
        emit modifiedStateChanged(!last);
    }
}

#include "QPxAbstractEditorCommand.h"

QPx::AbstractEditorCommand::AbstractEditorCommand(QString name, QObject *parent) : QObject(parent)
{
    cache.alloc<QString>(name);
}

QString QPx::AbstractEditorCommand::name() const
{
    return cache.get<QString>();
}

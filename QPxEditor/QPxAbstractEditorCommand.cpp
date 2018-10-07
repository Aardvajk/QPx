#include "QPxAbstractEditorCommand.h"

QPx::AbstractEditorCommand::AbstractEditorCommand(QString name, QObject *parent) : QObject(parent), n(std::move(name))
{
}

QString QPx::AbstractEditorCommand::name() const
{
    return n;
}

void QPx::AbstractEditorCommand::setName(const QString &name)
{
    if(n != name)
    {
        n = name;
        emit changed();
    }
}

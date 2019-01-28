#include "QPxCore/QPxAction.h"

namespace
{

const char *id_name = "qpx_action_id";

}

QPx::Action::Action(const QString &id, QObject *parent) : QAction(parent)
{
    setProperty(id_name, id);
}

QString QPx::Action::id() const
{
    return property(id_name).toString();
}

void QPx::Action::setId(const QString &id)
{
    if(property(id_name).toString() != id)
    {
        setProperty(id_name, id);
        emit changed();    
    }
}

#ifndef QPXABSTRACTEDITORCOMMAND_H
#define QPXABSTRACTEDITORCOMMAND_H

#include <QtCore/QObject>

#include <pcx/aligned_store.h>

namespace QPx
{

class AbstractEditorCommand : public QObject
{
    Q_OBJECT

public:
    explicit AbstractEditorCommand(QString name, QObject *parent = nullptr);

    QString name() const;

    virtual bool isValid() const = 0;
    virtual bool modifiesPersistentState() const = 0;

    virtual void undo() = 0;
    virtual void redo() = 0;

signals:
    void changed();

public slots:
    void setName(const QString &name);

private:
    pcx::aligned_store<8> cache;
};

}

#endif // QPXABSTRACTEDITORCOMMAND_H

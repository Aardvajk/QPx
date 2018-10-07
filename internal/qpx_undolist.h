#ifndef QPX_UNDOLIST_H
#define QPX_UNDOLIST_H

#include <QtCore/QObject>

#include <pcx/aligned_store.h>

namespace QPx
{

class AbstractEditorCommand;

}

class qpx_undolist : public QObject
{
    Q_OBJECT

public:
    qpx_undolist(QObject *parent = nullptr);

    void addCommand(QPx::AbstractEditorCommand *command);
    void clear();

    bool canUndo() const;
    bool canRedo() const;
    
    bool isModified() const;
    
    QString lastCommandName() const;
    QString nextCommandName() const;

signals:
    void undoStateChanged();
    void modifiedStateChanged(bool state);

public slots:
    void undo();
    void redo();

    void setSavePoint();

private:
    pcx::aligned_store<16> cache;
};

#endif // QPX_UNDOLIST_H

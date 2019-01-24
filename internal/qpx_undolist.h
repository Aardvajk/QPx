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
    explicit qpx_undolist(QObject *parent = nullptr);

    void addCommand(QPx::AbstractEditorCommand *command);
    void clear();

    bool canUndo() const;
    bool canRedo() const;
    
    bool isModified() const;

    QString lastCommandName() const;
    QString nextCommandName() const;

    int limit() const;

signals:
    void undoStateChanged();
    void modifiedStateChanged(bool state);

public slots:
    void undo();
    void redo();

    void setSavePoint();
    void setLimit(int value);

private:
    pcx::aligned_store<24> cache;
};

#endif // QPX_UNDOLIST_H

#ifndef QPX_ABSTRACTEDITORMODEL_H
#define QPX_ABSTRACTEDITORMODEL_H

#include <QtCore/QObject>

#include <pcx/aligned_store.h>

namespace QPx
{

class AbstractEditorCommand;

class AbstractEditorModel : public QObject
{
    Q_OBJECT
    
public:
    AbstractEditorModel(QObject *parent = nullptr);

    virtual void beginCommand(AbstractEditorCommand *command) = 0;
    void endCommand(AbstractEditorCommand *command);

    bool isModified() const;

    bool canUndo() const;
    bool canRedo() const;

    QString lastCommandName() const;
    QString nextCommandName() const;
    
signals:    
    void undoStateChanged();
    void modifiedStateChanged(bool state);

public slots:
    void setSavePoint();

    void undo();
    void redo();

protected:
    void clearUndoList();
    
private:
    pcx::aligned_store<48> cache;
};

}

#endif // QPX_ABSTRACTEDITORMODEL_H

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
    explicit AbstractEditorModel(QObject *parent = nullptr);

    void endCommand(AbstractEditorCommand *command);

    bool isModified() const;

    bool canUndo() const;
    bool canRedo() const;

    QString lastCommandName() const;
    QString nextCommandName() const;

    QString path() const;
    
    virtual bool clear() = 0;
    virtual bool open(const QString &path) = 0;
    virtual bool save(const QString &path) const = 0;

    virtual QString filter() const = 0;
    
signals:    
    void undoStateChanged();
    void modifiedStateChanged(bool state);
    void pathChanged(const QString &path);

public slots:
    void setPath(const QString &path);
    void setSavePoint();
    void setUndoLimit(int value);

    void undo();
    void redo();

protected:
    void clearUndoList();
    
private:
    pcx::aligned_store<80> cache;
};

}

#endif // QPX_ABSTRACTEDITORMODEL_H

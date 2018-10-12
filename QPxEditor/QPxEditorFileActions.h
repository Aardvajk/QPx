#ifndef QPX_EDITORFILEACTIONS_H
#define QPX_EDITORFILEACTIONS_H

#include <QtCore/QObject>

#include <pcx/aligned_store.h>

class QWidget;
class QMenu;

namespace QPx
{

class AbstractEditorModel;

class EditorFileActions : public QObject
{
    Q_OBJECT

public:
    EditorFileActions(AbstractEditorModel *model, QWidget *parent);

    bool canClose();
    
    QMenu *recentFilesMenu();

public slots:
    void setRecentFiles(const QStringList &files);

protected slots:
    bool clear();
    bool open();
    bool save();
    bool saveAs();
    
private slots:
    void recentFileRequested();
    void recentClearClicked();

private:
    void updateRecentMenu();
    void fileUsed(const QString &path);

    pcx::aligned_store<24> cache;
};

}

#endif // QPX_EDITORFILEACTIONS_H

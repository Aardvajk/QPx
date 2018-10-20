#ifndef QPXKEYSEQUENCEEDIT_H
#define QPXKEYSEQUENCEEDIT_H

#include <QtWidgets/QLineEdit>

#include <pcx/aligned_store.h>

namespace QPx
{

class KeySequenceEdit : public QLineEdit
{
    Q_OBJECT
    
public:
    explicit KeySequenceEdit(QWidget *parent = nullptr);

    QKeySequence KeySequence() const;

signals:
    void keySequenceChanged(const QKeySequence &sequence);

public slots:
    void setKeySequence(const QKeySequence &sequence);

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

private slots:
    void textModified(const QString &text);

private:
    pcx::aligned_store<16> cache;
};

}

#endif // QPXKEYSEQUENCEEDIT_H

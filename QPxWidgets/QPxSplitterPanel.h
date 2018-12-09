#ifndef QPX_SPLITTERPANEL_H
#define QPX_SPLITTERPANEL_H

#include <QtWidgets/QWidget>

class QSplitter;

namespace QPx
{

class SplitterPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SplitterPanel(QWidget *parent = nullptr);

public slots:
    void splitVertical();
    void splitHorizontal();

protected:
    virtual void closeEvent(QCloseEvent *event) override;

    virtual QSplitter *createSplitter(Qt::Orientation orientation);
    virtual SplitterPanel *clone() const = 0;

private:
    bool closeFlag;
};

}

#endif // QPX_SPLITTERPANEL_H

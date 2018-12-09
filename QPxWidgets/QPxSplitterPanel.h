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

    void split(Qt::Orientation orientation, QWidget *widget);

protected:
    virtual void closeEvent(QCloseEvent *event) override;
};

}

#endif // QPX_SPLITTERPANEL_H

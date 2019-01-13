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

    SplitterPanel *split(Qt::Orientation orientation, SplitterPanel *panel);

protected:
    virtual QSplitter *createSplitter(Qt::Orientation orientation) const;

    virtual void closeEvent(QCloseEvent *event) override;
};

}

#endif // QPX_SPLITTERPANEL_H

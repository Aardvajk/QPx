#ifndef QPX_VIEWPANEL_H
#define QPX_VIEWPANEL_H

#include <QPxWidgets/QPxSplitterPanel.h>

namespace QPx
{

class ViewBar;

class ViewPanel : public QPx::SplitterPanel
{
    Q_OBJECT

public:
    explicit ViewPanel(QWidget *parent = nullptr);

    ViewBar *viewBar() const;

protected:
    virtual ViewPanel *clone() const = 0;

private slots:
    void splitVertical();
    void splitHorizontal();
    void closePanel();

private:
    ViewBar *bar;
};

}

#endif // QPX_VIEWPANEL_H

#ifndef QPX_LINESPLITTER_H
#define QPX_LINESPLITTER_H

#include <QPxWidgets/QPxLayouts.h>

namespace QPx
{

class LineSplitter : public QPx::Splitter
{
    Q_OBJECT

public:
    LineSplitter(Qt::Orientation orientation, QWidget *parent = nullptr);

protected:
    virtual QSplitterHandle *createHandle() override;
};

}

#endif // QPX_LINESPLITTER_H

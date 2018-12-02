#ifndef QPX_LINESPLITTER_H
#define QPX_LINESPLITTER_H

#include <QtWidgets/QSplitter>

namespace QPx
{

class LineSplitter : public QSplitter
{
    Q_OBJECT

public:
    LineSplitter(Qt::Orientation orientation, QWidget *parent = nullptr);

protected:
    virtual QSplitterHandle *createHandle() override;
};

}

#endif // QPX_LINESPLITTER_H

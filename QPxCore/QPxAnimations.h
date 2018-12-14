#ifndef QPX_ANIMATIONS_H
#define QPX_ANIMATIONS_H

#include <QtCore/QVariantAnimation>

namespace QPx
{

class UnitAnimation : public QVariantAnimation
{
    Q_OBJECT

public:
    UnitAnimation(int durationIn, int durationOut, QObject *parent = nullptr);

public slots:
    void activate();
    void deactivate();

    void reset();

private:
    int dIn;
    int dOut;
};

}

#endif // QPX_ANIMATIONS_H

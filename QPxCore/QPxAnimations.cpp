#include "QPxCore/QPxAnimations.h"

namespace
{

void updateAnimation(QPx::UnitAnimation *anim, float target, int duration)
{
    anim->setStartValue(anim->currentValue().toFloat());
    anim->setEndValue(target);
    anim->setDuration(duration);

    anim->start();
}

}

QPx::UnitAnimation::UnitAnimation(int durationIn, int durationOut, QObject *parent) : QVariantAnimation(parent), dIn(durationIn), dOut(durationOut)
{
}

void QPx::UnitAnimation::activate()
{
    updateAnimation(this, 1, dIn);
}

void QPx::UnitAnimation::deactivate()
{
    updateAnimation(this, 0, dOut);
}

void QPx::UnitAnimation::reset()
{
    setStartValue(0.0f);
    setEndValue(0.0f);

    stop();
}

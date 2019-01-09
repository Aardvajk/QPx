#include "QPxCore/QPxUnitAnimation.h"

namespace
{

class Cache
{
public:
    explicit Cache(int duration) : duration(duration), value(0) { }

    int duration;
    float value;
};

}

QPx::UnitAnimation::UnitAnimation(int duration, QObject *parent) : QAbstractAnimation(parent)
{
    cache.alloc<Cache>(duration);
}

int QPx::UnitAnimation::duration() const
{
    return cache.get<Cache>().duration;
}

float QPx::UnitAnimation::currentValue() const
{
    return cache.get<Cache>().value;
}

void QPx::UnitAnimation::activate(bool state)
{
    setDirection(state ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
    start();
}

void QPx::UnitAnimation::reset()
{
    stop();
    cache.get<Cache>().value = 0;
}

void QPx::UnitAnimation::updateCurrentTime(int currentTime)
{
    auto &c = cache.get<Cache>();

    c.value = float(currentTime) / float(c.duration);
    emit currentValueChanged(c.value);
}

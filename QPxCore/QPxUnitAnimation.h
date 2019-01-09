#ifndef QPX_UNITANIMATION_H
#define QPX_UNITANIMATION_H

#include <QtCore/QAbstractAnimation>

#include <pcx/aligned_store.h>

namespace QPx
{

class UnitAnimation : public QAbstractAnimation
{
    Q_OBJECT

public:
    UnitAnimation(int duration, QObject *parent = nullptr);

    virtual int duration() const override;

    float currentValue() const;

signals:
    void currentValueChanged(float value);

public slots:
    void activate(bool state);
    void reset();

protected:
    virtual void updateCurrentTime(int currentTime) override;

private:
    pcx::aligned_store<8> cache;
};

}

#endif // QPX_UNITANIMATION_H

#ifndef QPXINDEXEDPAIR_H
#define QPXINDEXEDPAIR_H

#include <QtCore/QPair>

namespace QPx
{

template<typename T> class IndexedPair : public QPair<T, T>
{
public:
    using QPair<T, T>::QPair;

    T &operator[](int index){ return index == 0 ? QPair<T, T>::first : QPair<T, T>::second; }
    const T &operator[](int index) const { return index == 0 ? QPair<T, T>::first : QPair<T, T>::second; }
};

}

#endif // QPXINDEXEDPAIR_H

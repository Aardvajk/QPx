#ifndef QPX_STDHASH_H
#define QPX_STDHASH_H

#include <cstring>

namespace QPx
{

template<typename T> struct StdHash
{
    std::size_t operator()(const T &v) const { return static_cast<std::size_t>(qHash(v)); }
};

}

#endif // QPX_STDHASH_H

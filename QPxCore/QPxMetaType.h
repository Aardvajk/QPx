#ifndef QPX_METATYPE_H
#define QPX_METATYPE_H

#include <QtCore/QHash>
#include <QtCore/QDataStream>

#include <type_traits>

template<typename T> struct qpx_is_meta_enum : public std::false_type { };

template <typename T, typename U=typename std::enable_if<qpx_is_meta_enum<T>::value, T>::type> uint qHash(T type)
{
    return ::qHash(static_cast<uint>(type));
}

template <typename T, typename U=typename std::enable_if<qpx_is_meta_enum<T>::value, T>::type> QDataStream &operator<<(QDataStream &ds, T v)
{
    return ds << static_cast<typename std::underlying_type<T>::type>(v);
}

template <typename T, typename U=typename std::enable_if<qpx_is_meta_enum<T>::value, T>::type> QDataStream &operator>>(QDataStream &ds, T &v)
{
    typename std::underlying_type<T>::type i;

    ds >> i;
    v = static_cast<T>(i);

    return ds;
}

#endif // QPX_METATYPE_H

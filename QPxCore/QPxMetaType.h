#ifndef QPX_METATYPE_H
#define QPX_METATYPE_H

#include <QtCore/QMetaType>

#define QPX_DECLARE_COMPARABLE_METATYPE(TYPE) \
Q_DECLARE_METATYPE(TYPE) \
static int TYPE##_static_init_value = [](){ QMetaType::registerComparators<TYPE>(); return 0; }();

#endif // QPX_METATYPE_H

#ifndef LIBENGINES_GLOBAL_H
#define LIBENGINES_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBENGINES_LIBRARY)
#  define LIBENGINESSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBENGINESSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBENGINES_GLOBAL_H

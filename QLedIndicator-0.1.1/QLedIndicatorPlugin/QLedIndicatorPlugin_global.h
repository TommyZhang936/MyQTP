#ifndef QLEDINDICATORPLUGIN_GLOBAL_H
#define QLEDINDICATORPLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QLEDINDICATORPLUGIN_LIBRARY)
#  define QLEDINDICATORPLUGINSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QLEDINDICATORPLUGINSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QLEDINDICATORPLUGIN_GLOBAL_H

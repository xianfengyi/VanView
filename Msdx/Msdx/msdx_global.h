#ifndef MSDX_GLOBAL_H
#define MSDX_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef MSDX_LIB
# define MSDXAPI Q_DECL_EXPORT
#else
# define MSDXAPI Q_DECL_IMPORT
#endif

#endif // MSDX_GLOBAL_H

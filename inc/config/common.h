#ifndef __COMMON_H__
#define __COMMON_H__

#if ENABLE_NLS
#include <libintl.h>
#define _(String) gettext(String)
#define N_(String) gettext_noop(String)
#else
#define _(String) (String)
#define N_(String) (String)
#endif /* ENABLE_NLS */

#include <config/basetypes.h>
#include <config/config.h>
#include <config/util.h>

#include <Exception.h>

#endif /* __COMMON_H__ */

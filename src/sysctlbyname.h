#include "compat.h"
#if ZB_BSD
# ifndef ZB_SYSCTLBYNAME_H_GUARD
#  define ZB_SYSCTLBYNAME_H_GUARD 1
/* PARAMS is a macro used to wrap function prototypes, so that
  compilers that don't understand ANSI C prototypes still work,
  and ANSI C compilers can issue warnings about type mismatches. */
#  undef PARAMS
#  if defined (__STDC__) || defined (_AIX)		\
     || (defined (__mips) && defined (_SYSTYPE_SVR4))	\
     || defined(WIN32) || defined(__cplusplus)
#   define PARAMS(protos) protos
#  else
#   define PARAMS(protos) ()
#  endif

#  include <stdlib.h>

int sysctlbyname PARAMS((const char *name,
			 void *oldp,
			 size_t *oldlenp,
			 void *newp,
			 size_t newlen));

# endif /* ZB_SYSCTLBYNAME_H_GUARD */
#endif

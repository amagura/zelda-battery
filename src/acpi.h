#ifndef ZB_ACPI_H_GUARD
#define ZB_ACPI_H_GUARD 1
/* BEGIN_C_DECLS should be used at the beginning of your declarations,
so that C++ compilers don't mangle their names.  Use END_C_DECLS at
the end of C declarations. */
#undef BEGIN_C_DECLS
#undef END_C_DECLS
#ifdef __cplusplus
# define BEGIN_C_DECLS extern "C" {
# define END_C_DECLS }
#else
# define BEGIN_C_DECLS /* empty */
# define END_C_DECLS /* empty */
#endif

/* PARAMS is a macro used to wrap function prototypes, so that
  compilers that don't understand ANSI C prototypes still work,
  and ANSI C compilers can issue warnings about type mismatches. */
#undef PARAMS
#if defined (__STDC__) || defined (_AIX) \
       || (defined (__mips) && defined (_SYSTYPE_SVR4)) \
       || defined(WIN32) || defined(__cplusplus)
# define PARAMS(protos) protos
#else
# define PARAMS(protos) ()
#endif

BEGIN_C_DECLS

void
find_battpath PARAMS((char *path, char *buf, size_t bufsize, int *batnum));

void
bcapcity PARAMS((char *battpath, int *dst));

void
find_acpath PARAMS((char *pth, char *buf, size_t bufsize));

void
actat PARAMS((char *pth, bool *dst));

END_C_DECLS

#endif /* ZBATT_ACPI_H_GUARD */

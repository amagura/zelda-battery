/****
Copyright 2015, 2016, 2017 Alexej Magura

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
****/
#ifndef COMMON_CONCAT_C_GUARD
# define COMMON_CONCAT_C_GUARD 1

# if defined(HAVE_CONFIG_H)
#  include <config.h>
# endif

# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include <stdbool.h>

# if defined(HAVE_MEMPCPY) || defined(HAVE_BZERO)
#  ifndef _GNU_SOURCE
#   define _GNU_SOURCE 1
#  endif
# endif
# include <string.h>
# include <limits.h>

# if !defined(NO_COMMON_H) && defined(HAVE_LIBCOMMONER)
#  include "common.h"
# else
#  include "main.h"
# endif

# if COM_DEBUG
#  include "cpeek.c"
# endif

# if !defined(HAVE_LIBCOMMONER)

// XXX OBSOLETE
#  if 0
char *concat(const char *s1, ...) __attribute__((sentinel))
     __attribute__((warn_unused_result));

// XXX returned value needs free
char *concat(const char *s1, ...)
{
	va_list args;
	const char *s;
	char *p, *result;
	unsigned long l, m, n;
	m = n = strlen(s1);
	va_start(args, s1);
	while ((s = va_arg(args, char *))) {
		l = strlen(s);
		if ((m += l) < l) break;
	}
	va_end(args);
	if (s || m >= INT_MAX) return NULL;
	com_mtrace;

#  if defined(__cplusplus)
	result = (char *)malloc(m + 1);
#  else
	result = malloc(m + 1);
#  endif
	if (!result) return NULL;

	memcpy(p = result, s1, n);
	p += n;
	va_start(args, s1);
	while ((s = va_arg(args, char *))) {
		l = strlen(s);
		if ((n += l) < l || n > m) break;
		memcpy(p, s, l);
		p += l;
	}
	va_end(args);
	if (s || m != n || p != result + n) {
		free(result);
		return NULL;
	}
	*p = '\0';
	com_muntrace;
	return result;
}
#  endif

/* unlike `concat', which returns a
 * new pointer that must then be copied
 * or acted upon in some meaningfully meaningless
 * manner, `catl' returns the number of bytes belonging
 * to `buf', which could _NOT_ be filled, always copying
 * no more than `bufsiz` of data into `buf'
 *
 * If the return value is an integral value, which
 * we'll call `y', that is less than 0,
 * then the resulting catenation has been truncated by `!y'
 * many bytes.  Similarlly, if a positive value is returned:
 * `y' many bytes is how much of `buf', which was _NOT_ used.
 *
 * XXX A failure is indicated by a return value _equal_ to
 * the destination buffers size_, which may make errors somewhat
 * harder to spot! */

size_t concatl(char *dst, size_t sz, const char *s1, ...) __attribute__((sentinel));

size_t concatl(char *dst, size_t sz, const char *s1, ...)
{
     va_list args;
     const char *s = NULL;
     char *p, *tmp;
     unsigned long ldx, mdx, ndx;
     size_t used = 0;

     mdx = ndx = strlen(s1);
     va_start(args, s1);
     while ((s = va_arg(args, char *))) {
	  ldx = strlen(s);
	  if ((mdx += ldx) < ldx) break;
     }
     va_end(args);
     if (s || mdx >= INT_MAX) return sz;
     com_mtrace;
#  if defined(__cplusplus)
     tmp = (char *)malloc(mdx + 1);
#  else
     tmp = malloc(mdx + 1);
#  endif
     if (!tmp) return sz;
     bzero(tmp, mdx + 1);
     bzero(dst, mdx + 1);

     p = tmp;
     p = mempcpy(p, s1, ndx);

     used += ndx;
     COM_DBG("p: `%s`\n", p);
     COM_DBG("used: %lu\n", used - 0);

     va_start(args, s1);
     while ((s = va_arg(args, char *))) {
	  ldx = strlen(s);
	  if ((ndx += ldx) < ldx || ndx > mdx) break;
	  p = mempcpy(p, s, ldx);
	  used += ldx;
     }
     va_end(args);
     if (s || mdx != ndx || p != tmp + ndx) {
	  free(tmp);
	  return sz;
     }

     COM_DBG("tmp: `%s'\n", tmp);
     p = mempcpy(dst, tmp, (used > sz ? sz : used));
     free(tmp);
     *p = '\0';
     ++used;

     COM_DBG("dst: `%s'\n", dst);
     COM_DBG("*p: `%c'\n", *p);
     COM_DBG("*--p: `%c'\n", cpeek(p, dst, 0));
     COM_DBG("strlen(dst): %lu\n", strlen(dst));
     COM_DBG("used#2: %lu\n", used - 0);
     com_muntrace;
     return sz - used;
}

/* concatm is a little different:
 * unlike `concatl' or `concat', concatm _moves_ memory: that is, the destination
 * pointer can be passed as an argument. */
size_t concatm(char *dst, size_t sz, const char *s1, ...) __attribute__((sentinel));

size_t concatm(char *dst, size_t sz, const char *s1, ...)
{
     va_list args;
     const char *s = NULL;
     char *p, *tmp;
     unsigned long ldx, mdx, ndx;
     size_t used = 0;

     mdx = ndx = strlen(s1);
     va_start(args, s1);
     while ((s = va_arg(args, char *))) {
	  ldx = strlen(s);
	  if ((mdx += ldx) < ldx) break;
     }
     va_end(args);
     if (s || mdx >= INT_MAX) return sz;

     com_mtrace;

#  if defined(__cplusplus)
     tmp = (char *)malloc(mdx + 1);
#  else
     tmp = malloc(mdx + 1);
#  endif
     if (!tmp) return sz;
     bzero(tmp, mdx + 1);

     p = tmp;
     p = mempcpy(p, s1, ndx);

     used += ndx;
     COM_DBG("p: `%s`\n", p);
     COM_DBG("used: %lu\n", used - 0);

     va_start(args, s1);
     while ((s = va_arg(args, char *))) {
	  ldx = strlen(s);
	  if ((ndx += ldx) < ldx || ndx > mdx) break;
	  p = mempcpy(p, s, ldx);
	  used += ldx;
     }
     va_end(args);
     if (s || mdx != ndx || p != tmp + ndx) {
	  free(tmp);
	  return sz;
     }
     COM_DBG("tmp: `%s'\n", tmp);
#  if defined(mempmove) && COM_USE_MEMPMOVE
     p = mempmove(dst, tmp, (used > sz ? sz : used));
#  else
     memmove(dst, tmp, (used > sz ? sz : used));
     p = &dst[(used > sz ? sz : used)];
#  endif
     free(tmp);
     *p = '\0';
     ++used;

     COM_DBG("dst: `%s'\n", dst);
     COM_DBG("*p: `%c'\n", *p);
     COM_DBG("*--p: `%c'\n", cpeek(p, dst, 0));
     COM_DBG("strlen(dst): %lu\n", strlen(dst));
     COM_DBG("used#2: %lu\n", used - 0);
     com_muntrace;

     return sz - used;
}

#  undef catl
#  define catl(...) (concatl(__VA_ARGS__, (void *)NULL))

#  undef catm
#  define catm(...) (concatm(__VA_ARGS__, (void *)NULL))

# endif

#endif

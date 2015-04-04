/****
Copyright 2014-2015 Alexej Magura

This file is a part of ZBatt

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
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include "main.h"
#include "compat.h"

#if 0
int stoi(int *dst, const char *src)
{
     errno = 0;
#if HAVE_LIBBSD
     *dst = strtonum(src, INT_MIN, INT_MAX, NULL);
#endif
     return 0;
}
#endif

#define zb_intlen(ZB_IDX)			\
     do {					\
	  int ZB_hdx = !(ZB_IDX);		\
	  while (ZB_IDX) {			\
	       ++ZB_hdx;			\
	       (ZB_IDX) /= 10;			\
	  }					\
     } while(0)

#define zb_small_intlen(ZB_IDX)			\
     do {					\
	  int ZB_hdx = abs((ZB_IDX));		\
	  (ZB_IDX) = (ZB_hdx < 100		\
		      ? 2			\
		      : (ZB_hdx < 10		\
			 ? 1			\
			 : 3));			\
     } while(0)

#define zb_safesub(ZB_IDX, ZB_HDX)			\
     (((ZB_IDX) > (ZB_HDX) ? (ZB_IDX) : (ZB_HDX))	\
      - ((ZB_IDX) > (ZB_HDX) ? (ZB_HDX) : (ZB_IDX)))

void rev(char *s)
{
     int idx, hdx;
     char tmp;
     hdx = strlen(s) - 1;

     for (idx = 0; idx < hdx; ++idx, --hdx) {
	  tmp = s[idx];
	  s[idx] = s[hdx];
	  s[hdx] = tmp;
     }
}

void itoa(char *dst, int idx)
{
     int len = idx;
     zb_small_intlen(len);
     char tmp[len+1];
     char *wp = tmp;

     for (; idx != 0; ++wp, idx /= 10) {
	  if (idx >= 0)
	       *wp = '0' + (idx % 10);
	  else
	       *wp = '0' - (idx % 10);
	  ZB_DBG("wp: `%c`\n", *wp);
     }
     *wp++ = '\0';
     rev(tmp);
     ZB_DBG("wp: `%s`\n", tmp);
     memcpy(dst, tmp, len+1);
}

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
 * XXX A failure is indicated by a return value _equal to
 * the destination buffers size_, which may make errors somewhat
 * harder to spot! */
size_t catl(size_t dstsiz, char *dst, const char *s1, ...)
{
     va_list args;
     const char *s;
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
     if (s || mdx >= INT_MAX) return dstsiz;

     tmp = malloc(mdx + 1);
     if (!tmp) return dstsiz;
     bzero(tmp, mdx + 1);

     p = mempcpy(p = tmp, s1, ndx);
     used += ndx;
     ZB_DBG("p: `%s`\n", p);

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
	  return dstsiz;
     }

     *p = 0;
     ZB_DBG("used > dstsiz: %d\n", used > dstsiz);
     ZB_DBG("used == strlen(p): %d\n", used == strlen(p));
     ZB_DBG("used: %d\n", used);
     ZB_DBG("strlen(p): %d\n", strlen(p));
     ZB_DBG("p: `%s`\n", p);

     memcpy(dst, tmp, (used > dstsiz ? dstsiz : used));
     free(tmp);
     return dstsiz - used;
}

char *concat(size_t *siz, const char *s1, ...)
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

	result = malloc(m + 1);
	if (!result) return NULL;
	*siz = m + 1;

	memcpy(p = result, s1, n);
	p += n; /* moves memory address to next addressable space */
	va_start(args, s1);
	while ((s = va_arg(args, char *))) {
		l = strlen(s);
		if ((n += l) < l || n > m) break;
		memcpy(p, s, l);
		p += l; /* moves memory address to next empty address */
	}
	va_end(args);
	if (s || m != n || p != result + n) {
		free(result);
		return NULL;
	}

	*p = 0;
	return result;
}

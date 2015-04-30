/****
Copyright 2015 Alexej Magura

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
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "common.h"

int intlen(int idx)
{
     int result = 0;
     while (idx) {
	  ++result;
	  idx /= 10;
     }
     return result;
}

int intlenc(const int idx)
{
     int copy = idx;
     int result = 0;
     while (copy) {
	  ++result;
	  copy /= 10;
     }
     return result;
}

size_t intlenm(int src)
{
     size_t dst = 1; /* XXX adds 1 for null-terminator */
     while (src) {
	  ++dst;
	  src /= 10;
     }
     return dst;
}

# if 0
** Reverse functions **
#endif
void rev(char *s)
{
     int idx = 0;
     int hdx = (int)strlen(s) - 1;

     for (char c; idx < hdx; ++idx, --hdx) {
	  c = s[idx];
	  s[idx] = s[hdx];
	  s[hdx] = c;
     }
}

char *revp(const char *s)
{
     int idx = 0;
     int hdx = (int)strlen(s) - 1;
     char *copy = strdup(s);

     for (char c; idx < hdx; ++idx, --hdx) {
	  c = copy[idx];
	  copy[idx] = copy[hdx];
	  copy[hdx] = c;
     }
     return copy;
}

void revn(char *s, size_t n)
{
     --n;
     if (strchr(s, '\0'))
	  --n;
     int idx = 0;
     for (char tmp; idx < (int)n; ++idx, --n) {
	  tmp = s[idx];
	  s[idx] = s[n];
	  s[n] = tmp;
     }
}

char *revnp(char *s, size_t n)
{
     const size_t len = --n;
     if (strchr(s, '\0'))
	  --n;
     int idx = 0;
     for (char tmp; idx < (int)n; ++idx, --n) {
	  tmp = s[idx];
	  s[idx] = s[n];
	  s[n] = tmp;
     }
     return &s[0] + len;
}

#if 0
** itoa functions **
#endif
void itoa(char *dst, int src)
{
     size_t len = intlenm(src);
     char tmp[len];
     char *wp = tmp;

     for (; src != 0; ++wp, src /= 10) {
	  if (src >= 0)
	       *wp = '0' + (src % 10);
	  else
	       *wp = '0' - (src % 10);
#if COM_DLVL > 1
	  COM_DBG("*wp: `%c`\n", *wp);
#endif
     }
     *wp++ = '\0';
#if COM_DLVL > 1
     COM_DBG("len: %lu\n", len);
     COM_DBG("strlen(tmp): %lu\n", strlen(tmp));
     COM_DBG("sizeof(tmp): %lu\n", sizeof(tmp));
     COM_DBG("tmp: `%s'\n", tmp);
#endif
     rev(tmp);
#if COM_DLVL > 1
     COM_DBG("strlen(tmp)#2: %lu\n", strlen(tmp));
     COM_DBG("sizeof(tmp)#2: %lu\n", sizeof(tmp));
     COM_DBG("tmp#2: `%s'\n", tmp);
#endif
     COM_DBG("tmp#3: `%s'\n", tmp);
     memcpy(dst, tmp, len);
}

char *itoap(const int src)
{
     COM_DBG("src: %d\n", src);
     size_t len = intlenm(src);
     int idx = src;
     char *dst = malloc(len);
     bzero(dst, len);
     char *wp = dst;

     for (; idx != 0; idx /= 10) {
	  if (idx >= 0)
	       *wp++ = '0' + (idx % 10);
	  else
	       *wp++ = '0' - (idx % 10);
     }
     wp = revnp(dst, len);
     *wp = '\0';
     COM_DBG("dst: `%s'\n", dst);

     return dst;
}

char cpeek(const char *c, const char *s, const short fwd)
{
     if (fwd > 0) {
	  if (*c == '\0'
# if defined(_GNU_SOURCE)
	      || c == strchr(s, '\0') - 1
# else
	      || c == &s[strlen(s)]
# endif
	       )
	       return *c;
	  else
	       return *(c + 1);
     }
     return (c == s) ? *c : *(c - 1);
}

#if 0
** Concat functions **
#endif
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

#if defined(__cplusplus)
	result = (char *)malloc(m + 1);
#else
	result = malloc(m + 1);
#endif
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
	return result;
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

#if defined(__cplusplus)
     tmp = (char *)malloc(mdx + 1);
#else
     tmp = malloc(mdx + 1);
#endif
     if (!tmp) return sz;
     bzero(tmp, mdx + 1);
     bzero(dst, mdx + 1);

     p = tmp;
     p = mempcpy(p, (char *)s1, ndx);

     used += ndx;
     COM_DBG("p: `%s`\n", p);
     COM_DBG("used: %lu\n", used - 0);

     va_start(args, s1);
     while ((s = va_arg(args, char *))) {
	  ldx = strlen(s);
	  if ((ndx += ldx) < ldx || ndx > mdx) break;
	  p = mempcpy(p, (char *)s, ldx);
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

     return (used > sz ? 0 : sz - used);
}

/* concatm is a little different:
 * unlike `concatl' or `concat', concatm _moves_ memory: that is, the destination
 * pointer can be passed as an argument. */
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

#if defined(__cplusplus)
     tmp = (char *)malloc(mdx + 1);
#else
     tmp = malloc(mdx + 1);
#endif
     if (!tmp) return sz;
     bzero(tmp, mdx + 1);

     p = tmp;
     p = mempcpy(p, (char *)s1, ndx);

     used += ndx;
     COM_DBG("p: `%s`\n", p);
     COM_DBG("used: %lu\n", used - 0);

     va_start(args, s1);
     while ((s = va_arg(args, char *))) {
	  ldx = strlen(s);
	  if ((ndx += ldx) < ldx || ndx > mdx) break;
	  p = mempcpy(p, (char *)s, ldx);
	  used += ldx;
     }
     va_end(args);
     if (s || mdx != ndx || p != tmp + ndx) {
	  free(tmp);
	  return sz;
     }
     COM_DBG("tmp: `%s'\n", tmp);
#if defined(mempmove) && 0
     p = mempmove(dst, tmp, (used > sz ? sz : used));
#else
     memmove(dst, tmp, (used > sz ? sz : used));
     p = &dst[(used > sz ? sz : used)];
#endif
     free(tmp);
     *p = '\0';
     ++used;

     COM_DBG("dst: `%s'\n", dst);
     COM_DBG("*p: `%c'\n", *p);
     COM_DBG("*--p: `%c'\n", cpeek(p, dst, 0));
     COM_DBG("strlen(dst): %lu\n", strlen(dst));
     COM_DBG("used#2: %lu\n", used - 0);

     return (used > sz ? 0 : sz - used);
}

#if 0
** Repeat functions **
#endif
void repeat(char *dst, const char s, size_t n)
{
     bzero(dst, n);
     char *wp = dst;

     do {
	  *wp = s;
     } while(++wp != &dst[n - 1]);

     dst[n] = '\0';
     COM_DBG("dst: `%s'\n", dst);
     COM_DBG("strlen(dst): %lu\n", strlen(dst));
     COM_DBG("sizeof(dst): %lu\n", sizeof(dst));
     COM_DBG("sizeof(void *): %lu\n", sizeof(void *));
}

int strrep(char *dst, const char *s, size_t n)
{
     bzero(dst, n);
     char *wp = dst;
     int r = 0;

     size_t ssiz = strlen(s), tsiz;
     tsiz = ssiz;

     if (ssiz > n-1)
	  goto end;
     tsiz += ssiz;

     do {
	  wp = mempcpy(wp, (char *)s, ssiz);
     } while (tsiz += ssiz, tsiz < n-1);

     *wp++ = '\0';
end:
     return r;
}

char *strprep(const char *s, int times)
{
     size_t ssiz = strlen(s);
     char *copy = malloc(ssiz * times + 1);
     char *wp = copy;

     for (; times > 0; --times)
	  wp = mempcpy(wp, (char *)s, ssiz);

     *wp++ = '\0';
     return copy;
}

# if 0
** strndelim **
#endif
int *strndelim(const char *s, const char od, const char cd, int count[2])
{
     memset(count, 0, sizeof(*count)*2);
     char *c = strchr(s, '\0');

     if (c == s)
	  return NULL;

     do {
	  if (c != s && cpeek(c, s, 0) == '\\')
	       continue;
	  if (*c == cd)
	       ++count[1];
	  else if (*c == od)
	       ++count[0];
     } while (c-- != s);

     if (od == cd && count[1] > 0) {
	  if (count[1] % 2 == 1)
	       while (count[0]++ < --count[1]);
	  else {
	       count[0] = count[1] * 0.5;
	       count[1] *= 0.5;
	  }
     }

     return count;
}

#if 0
** strwoq **
#endif

char *strwodqp(const char *src)
{
     size_t n = strlen(src) + 1;
     int c[2] = {0, 0}, even = 0;
     char *tmp, *token, *rest, *newp;
     tmp = token = rest = newp = NULL;

     if (!strndelim(src, '"', '"', c))
	  return NULL;

     if (c[0] == 0)
	  return NULL;

     tmp = strdup(src);
     newp = malloc(n);
     even = c[0] - abs(c[0] - c[1]);

     token = strtok_r(tmp, "\"", &rest);

     if (token == NULL) {
	  free(newp);
	  return NULL;
     }

     catl(newp, n, token);
     while ((token = strtok_r(NULL, "\"", &rest)) != NULL) {
	  if (even % 2 == 1) {
	       catm(newp, n, newp, token);
	       --even;
	  } else {
	       ++even;
	  }
     }

     free(tmp);
     return newp;
}

int strwodq(char *dst, const char *src, size_t n)
{
     int c[2] = {0, 0};
     int even = 0;
     int r = 0;
     char *tmp = NULL;
     char *token = NULL;
     char *rest = NULL;
     char *newp = NULL;

     if (!strndelim(src, '"', '"', c)) {
	  r += 1;
	  goto end;
     }

     if (c[0] == 0) {
	  r += 2;
	  goto end;
     }

     tmp = strdup(src);
     newp = malloc(n);
     even = c[0] - abs(c[0] - c[1]);

     token = strtok_r(tmp, "\"", &rest);
     if (token == NULL) {
	  r += 3;
	  goto free;
     }
     catl(newp, n, token);

     while ((token = strtok_r(NULL, "\"", &rest)) != NULL) {
	  if (even % 2 == 1) {
	       catm(newp, n, newp, token);
	       --even;
	  } else {
	       ++even;
	  }
     }

     memcpy(dst, newp, n);

free:
     free(tmp);
     free(newp);
end:
     return r;
}

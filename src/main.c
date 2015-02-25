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

inline int intlen(int idx)
{
     int hdx = !idx;
     while (idx) {
	  ++hdx;
	  idx /= 10;
     }
     return idx;
}

inline int small_intlen(int idx)
{
     int hdx = abs(idx);
     return (hdx < 100
	     ? 2
	     : (hdx < 10
		? 1
		: 3));
}

void rev(char *s)
{
     int idx, hdx;
     idx = 0;
     while (s[idx] != '\0')
	  ++idx;

     hdx = idx - 1;
     char tmp;
     for (idx = 0; idx < hdx; ++idx) {
	  tmp = s[idx];
	  s[idx] = s[hdx];
	  s[hdx] = tmp;
	  --hdx;
     }
}

void itoa(char *dst, int idx)
{
     const int len = small_intlen(idx);
     char tmp[len+1];
     char *wp = tmp;

     for (; idx != 0; ++wp) {
	  if (idx >= 0)
	       *wp = '0' + (idx % 10);
	  else
	       *wp = '0' - (idx % 10);
	  ZB_DBG("wp: `%c`\n", *wp);
	  idx /= 10;
     }
     *wp++ = '\0';
     rev(tmp);
     ZB_DBG("wp: `%s`\n", tmp);
     memcpy(dst, tmp, len+1);
}

# if 1
/* this one uses less memory */
char *concat(const char *s1, ...)
{
  va_list args;
  const char *s;
  char *p, *result;
  unsigned long l, m, n;

  m = n = strlen(s1);
  va_start(args, s1);
  while ((s = va_arg(args, const char *))) {
    l = strlen(s);
    if ((m += l) < l) break;
  }
  va_end(args);
  if (s || m >= INT_MAX) return NULL;

  result = (char *)malloc(m + 1);
  if (!result) return NULL;

  memcpy(p = result, s1, n);
  p += n;
  va_start(args, s1);
  while ((s = va_arg(args, const char *))) {
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

  *p = 0;
  return result;
}
# else
char *concat(const char *s1, ...)
{
     va_list vv; // variable arg vector
     size_t allocd = 8;
     char *dst = (char *)malloc(allocd);

     if (dst) {
	  char *newp, *tmp;
	  const char *s;
	  va_start(vv, s1);
	  tmp = dst;

	  for (s = s1; s != NULL; s = va_arg(vv, const char *)) {
	       size_t len = strlen(s);

	       /* Does more memory need to be alloc'd? */
	       if (tmp + len + 1 > dst + allocd) {
		    allocd += len + 1;
		    if ((newp = (char *)realloc(dst, allocd)) == NULL) {
			 free(dst);
			 /* XXX, if you call `va_end(vv)' here
			  * it will cause problems such as memory leaks */
			 return NULL;

		    }
		    tmp = newp + (tmp - dst);
		    dst = newp;
	       }
	       tmp = mempcpy(tmp, s, len);
	  }

	  /* XXX, `++' gets handled first; not the dereference op */
	  *tmp++ = '\0'; // terminates the string accordingly.

	  /* optimizes amount of memory used */
	  if ((newp = realloc(dst, tmp - dst)) != NULL)
	       dst = newp;

	  va_end(vv);
     }
     return dst;
}
# endif

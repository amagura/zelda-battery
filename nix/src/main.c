/****
Copyright 2014, 2015 Alexej Magura

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

int intlen(int idx)
{
     int result = 0;
     while (idx) {
	  ++result;
	  idx /= 10;
     }
     return result;
}

# if ZB_DEBUG
char cpeek(char *c, char *s, short fwd)
{
     char tmp = 0;
     switch (fwd) {
     case 1:
	  if (c == &s[strlen(s) - 1] || c == &s[strlen(s)])
	       return *c;
	  else
	       tmp = *++c;
	  --c;
	  break;
     case 0:
	  if (c == s)
	       return *c;
	  else
	       tmp = *--c;
	  ++c;
	  break;
     }
     return tmp;
}
# endif

void rev(char *s)
{
     int hdx;
     char tmp;
     hdx = strlen(s) - 1;

     for (int idx = 0; idx < hdx; ++idx, --hdx) {
	  tmp = s[idx];
	  s[idx] = s[hdx];
	  s[hdx] = tmp;
     }
}

# if 0
void posrev(char *s, int start, int end)
{
     char tmp;
     for (; start < end; ++start, --end) {
	  tmp = s[start];
	  s[start] = s[end];
	  s[end] = tmp;
     }
}
# endif

void itoa(char *dst, int src)
{
     int len = intlen(src);
     char tmp[++len];
     char *wp = tmp;

     for (; src != 0; ++wp, src /= 10) {
	  if (src >= 0)
	       *wp = '0' + (src % 10);
	  else
	       *wp = '0' - (src % 10);
# if ZB_DLEVEL > 1
	  ZB_DBG("wp: `%c`\n", *wp);
# endif
     }
     *wp = 0;
# if ZB_DLEVEL > 1
     ZB_DBG("len: %d\n", len);
     ZB_DBG("strlen(tmp): %lu\n", strlen(tmp));
     ZB_DBG("sizeof(tmp): %lu\n", sizeof(tmp));
# endif
     rev(tmp);

     ZB_DBG("strlen(tmp)#1: %lu\n", strlen(tmp));
     ZB_DBG("sizeof(tmp)#1: %lu\n", sizeof(tmp));
     //   ZB_DBG("sizeof(\"4096\"): %lu\n", sizeof("4096"));

     ZB_DBG("wp: `%s`\n", tmp);
     memcpy(dst, tmp, len);
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
size_t catl(size_t bufsiz, char *buf, const char *s1, ...)
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
     if (s || mdx >= INT_MAX) return bufsiz;

     tmp = malloc(mdx + 1);
     if (!tmp) return bufsiz;
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
	  return bufsiz;
     }

     ZB_DBG("*p: `%c'\n", *p);
     ZB_DBG("*p--: `%c'\n", cpeek(p, tmp, 0));
     *p = 0;
     ++used;
# if ZB_DLEVEL > 1
     ZB_DBG("used > bufsiz: %d\n", used > bufsiz);
     ZB_DBG("used == strlen(p): %d\n", used == strlen(p));
     ZB_DBG("used: %lu\n", used - 0);
     ZB_DBG("strlen(p): %lu\n", strlen(p));
     ZB_DBG("strlen(tmp): %lu\n", strlen(tmp));
     ZB_DBG("mdx + 1: %lu\n", mdx + 1);
     ZB_DBG("p: `%s`\n", p);
     ZB_DBG("p == &tmp[%lu + 1]: %d\n", mdx, p == &tmp[mdx + 1]);
     ZB_DBG("bufsiz - used: %lu\n", bufsiz - used);
     ZB_DBG("p == &tmp[%lu]: %d\n", mdx - 0, p == &tmp[mdx]);
# endif

     memcpy(buf, tmp, (used > bufsiz ? bufsiz : used));
     free(tmp);
     return bufsiz - used;
}

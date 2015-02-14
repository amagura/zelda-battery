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
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include "main.h"
#include "compat.h"

#if !defined(_GNU_SOURCE)
inline void *mempcpy(void *dest, const void *src, size_t len)
{
     return memcpy (dest, src, len) + len;
}
#endif

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

char *kcat(const char *s1, ...)
{
     va_list vav; // variable arg vector
     size_t base_alloc = 8;
     char *dst = (char *)malloc(base_alloc);

     if (dst) {
	  char *tp, *tmp;
	  const char *s;

	  va_start(vav, s1);
	  tmp = dst;

	  for (s = s1; s != NULL; s = va_arg(vav, const char *)) {
	       size_t len = strlen(s);

	       /* Does more memory need to be alloc'd? */
	       if ((tmp + len + 1) > (dst + base_alloc)) {
		    base_alloc = (base_alloc + len) * 2;
		    if ((tp = (char *)realloc(dst, base_alloc)) == NULL) {
			 free(dst);
			 va_end(vav);
			 return NULL;

		    }
		    tmp = tp + (tmp - dst);
		    dst = tp;
	       }
	       tmp = mempcpy(tmp, s, len);
	  }

	  /* XXX, `++' gets handled first; not the dereference op */
	  *tmp++ = '\0'; // terminates the string accordingly.

	  /* optimizes amount of memory used */
	  if ((tp = realloc(dst, tmp - dst)) != NULL)
	       dst = tp;

	  va_end(vav);
     }
     return dst;
}

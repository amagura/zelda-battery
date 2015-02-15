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

char *neko(const char *s1, ...)
{
     va_list vargv; // variable arg vector
     size_t allocd = 8;
     char *dst = (char *)malloc(allocd);

     if (dst) {
	  char *newp, *tmp;
	  const char *s;

	  va_start(vargv, s1);
	  tmp = dst;

	  for (s = s1; s != NULL; s = va_arg(vargv, const char *)) {
	       size_t len = strlen(s);

	       /* Does more memory need to be alloc'd? */
	       if (tmp + len + 1 > dst + allocd) {
		    allocd = (allocd + len) * 2;
		    if ((newp = (char *)realloc(dst, allocd)) == NULL) {
			 free(dst);
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

	  va_end(vargv);
     }
     return dst;
}

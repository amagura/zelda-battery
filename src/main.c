/****
Copyright 2014 Alexej Magura

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

#if !defined(mempcpy)
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

char *concat(const char *s1, ...)
{
     va_list ap;
     size_t allocated = 8;
     char *result = (char *) malloc (allocated);

     if (result != NULL) {
	  char *newp;
	  char *tmp;
	  const char *s;

	  va_start (ap, s1);

	  tmp = result;
	  for (s = s1; s != NULL; s = va_arg (ap, const char *)) {
	       size_t len = strlen (s);

	       /* Resize the allocated memory if necessary. */
	       if (tmp + len + 1 > result + allocated) {
		    allocated = (allocated + len) * 2;
		    newp = (char *) realloc (result, allocated);
		    if (newp == NULL) {
			 free (result);
			 va_end (ap);
			 return NULL;
		    }
		    tmp = newp + (tmp - result);
		    result = newp;
	       }
	       tmp = mempcpy (tmp, s, len);
	  }

	  /* Terminate the result string. */
	  *tmp++ = '\0';

	  /* Resize memory to optimal size. */
	  newp = realloc (result, tmp - result);
	  if (newp != NULL)
	       result = newp;

	  va_end (ap);
     }
     return result;
}

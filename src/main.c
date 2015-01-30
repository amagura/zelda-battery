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
#include "main.h"


/* Alloca crap */
#ifdef STDC_HEADERS
# include <stdlib.h>
# include <stddef.h>
#else
# ifdef HAVE_STDLIB_H
#  include <stdlib.h>
# endif
#endif
#ifdef HAVE_ALLOCA_H
# include <alloca.h>
#elif !defined alloca
# ifdef __GNUC__
#  define alloca __builtin_alloca
# elif defined _AIX
#  define alloca __alloca
# elif defined _MSC_VER
#  include <malloc.h>
#  define alloca _alloca
# elif !defined HAVE_ALLOCA
#  ifdef  __cplusplus
extern "C"
#  endif
void *alloca (size_t);
# endif
#endif

#ifndef mempcpy
inline void *mempcpy(void *dest, const void *src, size_t len)
{
     return memcpy (dest, src, len) + len;
}
#endif

/* TODO, make sure that we are allowed to use the following function.
 * It is __slightly__ modified, but it is pretty much the same function
 * used as an example in the Glibc manual. */
char *concat(const char *str, ...)
{
     va_list ap;
     size_t allocated = 100;
     char *result = (char *) malloc (allocated);

     if (result != NULL) {
	  char *newp;
	  char *wp;
	  const char *s;

	  va_start (ap, str);

	  wp = result;
	  for (s = str; s != NULL; s = va_arg (ap, const char *)) {
	       size_t len = strlen (s);

	       /* Resize the allocated memory if necessary. */
	       if (wp + len + 1 > result + allocated) {
		    allocated = (allocated + len) * 2;
		    newp = (char *) realloc (result, allocated);
		    if (newp == NULL) {
			 free (result);
			 va_end (ap);
			 return NULL;
		    }
		    wp = newp + (wp - result);
		    result = newp;
	       }
	       wp = mempcpy (wp, s, len);
	  }

	  /* Terminate the result string. */
	  *wp++ = '\0';

	  /* Resize memory to optimal size. */
	  newp = realloc (result, wp - result);
	  if (newp != NULL)
	       result = newp;

	  va_end (ap);
     }
     return result;
}

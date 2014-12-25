#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#include <unistd.h>
#include <sysctl.h> /* needs -lc */

#if _ZB_MAKING_ZB_COLOR
#define _ZB_PROGNAME "zbac"
#else
#define _ZB_PROGNAME "zbat"
#endif

#define 

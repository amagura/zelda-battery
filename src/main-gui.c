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
#include <string.h>
#include "main.h"
#include "compat.h"
#include "main-gui.h"

#ifndef GZB_SCALEDIR
# define GZB_SCALEDIR ""
#endif

#ifndef GZB_PNGDIR
# define GZB_PNGDIR ""
#endif

#ifndef GZB_PROGNAME
# ifndef ZB_PROGNAME
#  define ZB_PROGNAME ""
# endif
# define GZB_PROGNAME ZB_PROGNAME
#endif

char *scaledir()
{
     return GZB_SCALEDIR;
}

char *pngdir()
{
     return GZB_PNGDIR;
}

char *progname()
{
     return GZB_PROGNAME;
}

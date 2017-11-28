/****
Copyright 2014, 2015, 2016 Alexej Magura

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
#ifndef ZB_MAIN_HXX_GUARD
# define ZB_MAIN_HXX_GUARD 1

# if HAVE_CONFIG_H
#  include <config.h>
# endif

/* ZBatt only supports Windows XP and later, for now */
# define _WIN32_WINNT NTDDI_WINXP

/**
# if !defined(NTDDI_VERSION)
#  define NTDDI_VERSION NTDDI_WINXP
# endif
**/
# if ZB_x86
#  if defined(HAVE__USR_I686_W64_MINGW32_INCLUDE_WINDOWS_H)
#   include "/usr/i686-w64-mingw32/include/windows.h"
#  else
#   if defined(HAVE_WINDOWS_H)
#    include <Windows.h>
#   elif defined(HAVE__WINDOWS_H)
#    include <windows.h>
#   elif !ZB_WINDOWS_BUILD
#    include <windows.h>
#   else
#    include <Windows.h>
#   endif
#  endif
#  if defined(HAVE__USR_I686_W64_MINGW32_INCLUDE_SHELLAPI_H)
#   include "/usr/i686-w64-mingw32/include/shellapi.h"
#  else
#   if defined(HAVE_SHELLAPI_H)
#    include <Shellapi.h>
#   elif defined(HAVE__SHELLAPI_H)
#    include <shellapi.h>
#   elif !ZB_WINDOWS_BUILD
#    include <shellapi.h>
#   else
#    include <Shellapi.h>
#   endif
#  endif
# elif ZB_amd64
#  if defined(HAVE__USR_X86_64_w64_MINGW32_INCLUDE_WINDOWS_H)
#   include "/usr/x86_64-w64-mingw32/include/windows.h"
#  else
#   if defined(HAVE_WINDOWS_H)
#    include <Windows.h>
#   elif defined(HAVE__WINDOWS_H)
#    include <windows.h>
#   elif !ZB_WINDOWS_BUILD
#    include <windows.h>
#   else
#    include <Windows.h>
#   endif
#  endif
#  if defined(HAVE__USR_X86_64_w64_MINGW32_INCLUDE_SHELLAPI_H)
#   include "/usr/x86_64-w64-mingw32/include/shellapi.h"
#  else
#   if defined(HAVE_SHELLAPI_H)
#    include <Shellapi.h>
#   elif defined(HAVE__SHELLAPI_H)
#    include <shellapi.h>
#   elif !ZB_WINDOWS_BUILD
#    include <shellapi.h>
#   else
#    include <Shellapi.h>
#   endif
#  endif
# endif

# define _WIN32_WINNT NTDDI_WINXP

struct hearts {
     char *full;
     char *empty;
};

#endif /* ZB_MAIN_HXX_GUARD */

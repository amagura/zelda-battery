/****
Copyright 2015 Alexej Magura

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

/* ZBatt only supports Windows XP and later, for now */
# define _WIN32_WINNT NTDDI_WINXP

# if !defined(NTDDI_VERSION)
#  define NTDDI_VERSION NTDDI_WINXP
# endif

#if defined(__MINGW32__) || defined(MINGW32_HOST)
# include "/usr/i686-w64-mingw32/include/windows.h"
# include "/usr/i686-w64-mingw32/include/shellapi.h"
#elif defined(__MINGW64__) || defined(MINGW64_HOST)
# include "/usr/x86_64-w64-mingw32/include/windows.h"
# include "/usr/x86_64-w64-mingw32/include/shellapi.h"
#else
# include <Windows.h>
# include <Shellapi.h>
#endif

#endif /* ZB_MAIN_HXX_GUARD */

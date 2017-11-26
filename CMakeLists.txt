#if GET_CMAKE_HEADER == 1
cmake_minimum_required (VERSION 2.6)
project (ZBatt)
#else
add_subdirectory (src/mswin)
#endif

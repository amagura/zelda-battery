cmake_minimum_required (VERSION 2.6)
project (ZBatt)

add_executable(zbatt text.c main.c acpi.c power.c concat.c)
add_executable(zbatc color.c main.c acpi.c power.c concat.c)

CHECK_INCLUDE_FILE(machine/apmvar.h)

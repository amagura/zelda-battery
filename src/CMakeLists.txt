cmake_minimum_required (VERSION 2.6)
project (ZBatt)

add_executable(zbatt text.c main.c acpi.c power.c concat.c)
add_executable(zbatc color.c main.c acpi.c power.c concat.c)
add_definitions(-DNO_COMMON_H)

set_property(TARGET zbatt PROPERTY C_STANDARD 11)
set_property(TARGET zbatc PROPERTY C_STANDARD 11)

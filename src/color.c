/****
Copyright 2014 Alexej Magura

This file is a part of Zelda Battery

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
#include <stdbool.h>

#include "headers/color.h"

struct pwr_src_t {
  bool batt;
  bool ac;
};

struct power_t {
  int charge;
  struct pwr_src_t source;
} power;

inline void
disp_pwr_info(struct color_disp_options_t opts)
{
  if (!opts.blink) {
    printf("%s", _ZB_COLOR_RED);
  } else {
    if (power.charge <= opts.blink_threshold) {
      if (power.source.ac)
        printf("%s", opts.acblink ? _ZB_COLOR_RED_BLINK : _ZB_COLOR_RED);
      else
        printf("%s", _ZB_COLOR_RED_BLINK);
    } else {
      printf("%s", _ZB_COLOR_RED);
    }
  }
}

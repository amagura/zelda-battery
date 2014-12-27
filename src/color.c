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
#include "header/main.h"
#include "header/color.h"
#include "header/power.h"

void
disp_pwr_info(struct color_disp_options_t opts, struct power_t power)
{
  printf("%s", _ZB_COLOR_RED);
  if (opts.blink) {
    if (power.charge <= opts.blink_threshold) {
      power.source.ac \
        && printf("%s", opts.acblink ? _ZB_COLOR_BLINK : _ZB_NULL) \
        || printf("%s", _ZB_COLOR_BLINK);
    }
  }
}
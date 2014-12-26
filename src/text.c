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

#include "header/main.h"
#include "header/text.h"
#include "header/power.h"

inline void
disp_pwr_info(struct txt_disp_options_t opts)
{
  for (int hdx = 1; hdx <= power.charge; ++hdx) {
    printf("%s", opts.full_heart);
  }

  for (; power.charge < 10; ++power.charge) {
    printf("%s", opts.empty_heart);
  }
}

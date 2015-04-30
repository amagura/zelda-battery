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
#include "main.hxx"
#include <iostream>

void disp(LPSYSTEM_POWER_STATUS pwr)
{
     char *norm = "31";
     int thold = 30;
     char *blnk = "5";
     std::cout << "\033[" << norm << "m";
     if (pwr->BatteryLifePercent > thold)
	  return;
     std::cout << "\033[" << blnk << "m";
     return;
}

int main(int argc, char **argv)
{
     SYSTEM_POWER_STATUS pwr;
     if (!GetSystemPowerStatus(&pwr))
	  goto fail;
     disp(&pwr);
win:
     return 0;
fail:
     return 1;
}

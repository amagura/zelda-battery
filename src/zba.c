#include <stdio.h>
#include <stdlib.h>

#include "zbb.h"

int
main(void)
{
  struct power_t power;
  power = zbb__init();

  /* print the appropriate number of full hearts */
  for (int idx = 0; idx < power.charge; ++idx)
    printf("%s", "\u2665");

  /* print the appropriate number of empty hearts */
  for (int idx = 10; idx > power.charge; --idx)
    printf("%s", "\u2661");
  return EXIT_SUCCESS;
}

#include "startup.h"

void startuptext(int i) {
  printf(BOLD "         +--------------+\n        /|             /|\n       / |            / |\n      *--*-----------*  |\n      |              |  |\n      | pickupbox    |  |\n      |              |  |\n   ...|  by Cesar M. |  +......\n  ....|    & Alex S. | /......\n .....|              |/......\n......*--------------*......\n" COLOR_RESET);
  if (i) {
    printf("Pickupbox server loading");
  }
  else {
    printf("Pickupbox client loading");
  }
  fflush(stdout);
  usleep(300000);
  printf(".");
  fflush(stdout);
  usleep(300000);
  printf(".");
  fflush(stdout);
  usleep(300000);
  printf(".");
  fflush(stdout);
  usleep(300000);
  if (i) {
    printf("Ready!\n");
  }
  usleep(300000);
}
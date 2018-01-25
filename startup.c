#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void startuptext(int i) {
  printf("         +--------------+\n        /|             /|\n       / |            / |\n      *--*-----------*  |\n      |              |  |\n      | pickupbox    |  |\n      |              |  |\n   ...|  by Cesar M. |  +......\n  ....|    & Alex S. | /......\n .....|              |/......\n......*--------------*......\n");
  if (i) {
    printf("Pickupbox server loading");
  }
  else {
    printf("Pickupbox client loading");
  }
  fflush(stdout);
  usleep(500000);
  printf(".");
  fflush(stdout);
  usleep(500000);
  printf(".");
  fflush(stdout);
  usleep(500000);
  printf(".");
  fflush(stdout);
  usleep(500000);
  if (i) {
    printf("Ready!\n");
    usleep(500000);
  }
}
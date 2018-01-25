#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void startuptext() {
  printf("         +--------------+\n        /|             /|\n       / |            / |\n      *--*-----------*  |\n      |              |  |\n      | pickupbox    |  |\n      |              |  |\n   ...|  by Cesar M. |  +......\n  ....|    & Alex S. | /......\n .....|              |/......\n......*--------------*......\n");
  printf("Pickupbox server loading");
  fflush(stdout);
  usleep(700000);
  printf(".");
  fflush(stdout);
  usleep(700000);
  printf(".");
  fflush(stdout);
  usleep(700000);
  printf(".");
  fflush(stdout);
  usleep(700000);
  printf("Ready!\n");
}
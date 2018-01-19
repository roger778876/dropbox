#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  printf("         +--------------+\n        /|             /|\n       / |            / |\n      *--*-----------*  |\n      |              |  |\n      | pickupbox    |  |\n      |              |  |\n   ...|  by Cesar M. |  +......\n  ....|    & Alex S. | /......\n .....|              |/......\n......*--------------*......\n");
  
  while (1) {
    printf("\b|");
    fflush(stdout);
    usleep(1000000);
    printf("\b/");
    fflush(stdout);
    usleep(1000000);
    printf("\b--");
    fflush(stdout);
    usleep(1000000);
    printf("\b\b\\");
    fflush(stdout);
    usleep(1000000);
  }
}

#include <stdio.h>
#include "display.h"

void display(Othero O[8][8]){
  int i, j;
  printf(" ０１２３４５６７\n");
  for (i = 0; i < 8; i++) {
    printf("%d", i);
    for (j = 0; j < 8; j++) {
      if (O[i][j].color == WHITE)
        printf("〇");
      else if (O[i][j].color == BLACK)
        printf("Ｘ");
      else
        printf("・");
    }
    printf("\n");
  }
}
#include "display.h"
#define BLACK 1
#define WHITE -1


//(1)盤面を表示する。
void display(int array[8][8]){
  int i,j;
   for(i=0;i<8;i++){
    for(j=0;j<8;j++){
      //printf(array[i][j]);
      if(array[i][j]==WHITE) printf("〇");
      else if(array[i][j]==BLACK) printf("Ｘ");
      else printf(" ");
    }
    printf("\n");
  }
}
#include <stdlib.h>
#include <stdio.h>

void main(){
  int source_row, source_col, dest_row, dest_col;
  for(int i = 0; i < 100; i++){
    printf("Enter a crane command <row,col row,col>:\n");
    scanf("%d,%d %d,%d", &source_row, &source_col, &dest_row, &dest_col);
    printf("%d,%d %d,%d\n", source_row, source_col, dest_row, dest_col);
  }

}

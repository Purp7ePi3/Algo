#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define rows 3
#define cols 3

typedef struct start {
  unsigned int xValue;
  unsigned int yValue;
} start;

start checkstart(unsigned int active[][cols]){
  unsigned int found = 0,i,j;
  start startPoint = {0,0};
  for (i = 0; i < 3; ++i) {
    for (j = 0; j < 3; ++j) {
        
        if (active[i][j] == 1) {
            /* Memorizza le coordinate nel punto con valore 1 nella struttura*/
            startPoint.xValue = i;
            startPoint.yValue = j;

            found = 1;  /* Imposta il flag a true*/
            break;      /* Esci dal ciclo interno*/
        }
    }

    if (found) {
        break;  /* Esci dal ciclo esterno se il punto è stato trovato*/
    }
}
return startPoint;
}

void PrintTable(unsigned int grid[][cols],unsigned int active[][cols]) {
  
  unsigned int i, j,k,m;
  system("cls");
  for (i = 0; i < rows; ++i) {
    for (m = 0; m < cols; ++m) {
      if(active[i][m]){
        printf("*\t");
        /*printf("%d\t",grid[i][j]);*/
      }else{
        printf(".\t");
      }
    }
    printf("\t");
    for (j = 0; j < cols; ++j) {
      if(active[i][j]){
        printf("%d\t",grid[i][j]);
      }else{
        printf(".\t");
      }
    }
    printf("\t");
    for (k = 0; k < cols; ++k) {
      printf("%d ",active[i][k]);
    }
    printf("\n");
  }
  printf("\n\n");
}

int checkmove(int move) {
  return move >= 0 && move <= rows*cols-1;  /* Esempio di verifica se la mossa è compresa tra 1 e 9*/
}

void changeState(unsigned int active[rows][cols], unsigned short x, unsigned short y, start startPoint) {
    if (active[x][y] != 0) {
       /*cella sopra*/
        if (x > 0 && (x - 1 != startPoint.xValue || y != startPoint.yValue)) {
            active[x - 1][y] = !active[x - 1][y];
        }
        /*cella sotto*/
        if (x < rows - 1 && (x + 1 != startPoint.xValue || y != startPoint.yValue)) {
            active[x + 1][y] = !active[x + 1][y];
        }
       /*cella sx*/
        if (y > 0 && (x != startPoint.xValue || y - 1 != startPoint.yValue)) {
            active[x][y - 1] = !active[x][y - 1];
        }
        /*cella dx*/
        if (y < cols - 1 && (x != startPoint.xValue || y + 1 != startPoint.yValue)) {
            active[x][y + 1] = !active[x][y + 1];
        }
        if(rows == 3 && cols == 3){
        if((x==0 && y==0 ) || (x==0 && y==cols-1) || (x==rows-1 && y==0) || (x==rows-1 && y==cols-1)){
          active[startPoint.xValue][startPoint.yValue] = !active[startPoint.xValue][startPoint.yValue];
        }
        active[x][y] = !active[x][y];
        }
    }
}


int checklose(unsigned int active[rows][cols], start startPoint) {
  int i, j, k = 0;
  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      if (active[i][j] == 1 && !(i == startPoint.xValue && j == startPoint.yValue)) {
        k++;
      }
    }
  }
  return k;
}


int main( void )
{ 
  int move,x,y,score,cmove=0;
  unsigned int grid[rows][cols] = {{0,1,2},{3,4,5},{6,7,8}};
  unsigned int active[rows][cols] = {{0, 0, 0}, {0, 1, 0}, {0, 0, 0}};

  start startPoint = checkstart(active);
 
  PrintTable(grid,active);
  scanf("%d",&move);
do {
    if (checkmove(move)) {
        cmove++;
        x = (move) / cols; 
        y = (move) % cols; 
        changeState(active, x, y,startPoint);
        PrintTable(grid, active);
        score = checklose(active,startPoint);
        if(score == rows*cols-1){
          printf("Hai Vinto! In %d",cmove); return 0;
        }else if(score == 0){
          printf("Hai perso"); return 0;
        }
    } else {
        printf("Mossa non valida\n");
    }
    scanf("%d", &move);
} while (move >= 0);
  return EXIT_SUCCESS;
}



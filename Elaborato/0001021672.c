/*TODO LEGGI I LE COSE CHE MANCANO DAL FILE DEL PROGETTO, CAMBIA QUALCHE VARIABILE IN QUA E Là E POI CI SIAMO*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define ALOT LONG_MAX

typedef struct {
    int x, y;
} moves;

typedef struct {
    moves moves;
    long int cost;
} nCoda;

typedef struct {
    nCoda *coda;
    int size;
} uCoda;

void swap(nCoda *a, nCoda *b) {
    nCoda temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(uCoda *u, int n) {
    int genitore = (n - 1) / 2;
    while (n > 0 && u->coda[genitore].cost > u->coda[n].cost) {
        swap(&u->coda[genitore], &u->coda[n]);
        n = genitore;
        genitore = (n - 1) / 2;
    }
}

void heapifyDown(uCoda *u, int n) {
    int minore = n;
    int sinistra = 2 * n + 1;
    int destra = 2 * n + 2;

    if (sinistra < u->size && u->coda[sinistra].cost < u->coda[minore].cost)
        minore = sinistra;

    if (destra < u->size && u->coda[destra].cost < u->coda[minore].cost)
        minore = destra;

    if (minore != n) {
        swap(&u->coda[n], &u->coda[minore]);
        heapifyDown(u, minore);
    }
}

uCoda* createuCoda(int size) {
    uCoda* u = (uCoda*)malloc(sizeof(uCoda));
    if(u == NULL ) exit(EXIT_FAILURE);
    u->coda = (nCoda*)malloc(size * sizeof(nCoda));
    if(u->coda == NULL ) exit(EXIT_FAILURE);
    u->size = 0;
    return u;
}

void enqueue(uCoda *u, moves moves, long int cost) {
    u->coda[u->size].moves = moves;
    u->coda[u->size].cost = cost;
    u->size++;
    heapifyUp(u, u->size - 1);
}

nCoda dequeue(uCoda *u) {
    nCoda minNode = u->coda[0];
    u->coda[0] = u->coda[u->size - 1];
    u->size--;
    heapifyDown(u, 0);
    return minNode;
}

void findMinPath(int rows, int cols, int **matrix, int ccell, int cheight) {
    /*Contatori*/
    int i, j, pathLength, newRow, newCol;
    moves **predecessors, printPath, *path, startpoint, temp; /*Modifica questo per far parirtire la ricerca da un'altra poszioni*/
    /*Sono i possibili movimenti, su, giu, destra, sinistra, se voglio farlo muovere in diagonale decommento e il for arriva fino a 6*/
    moves direzione[] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}/*,{-1, -1}, {1, 1}*/};
    uCoda *q;
    nCoda current;
    long int finalCost = 0, currentCost = 0, edgeCost = 0;
    int aRows, aCols;
    long int **distances = (long int **)malloc(rows * sizeof(long int *));
    if(distances == NULL) exit(EXIT_FAILURE);
    for (i = 0; i < rows; i++) {
        distances[i] = (long int *)malloc(cols * sizeof(long int));
        if(distances[i] == NULL) exit(EXIT_FAILURE);
        for (j = 0; j < cols; j++) {
            distances[i][j] = ALOT;
        }
    }
    predecessors = (moves **)malloc(rows * sizeof(moves *));
    if(predecessors == NULL) exit(EXIT_FAILURE);
    for (i = 0; i < rows; i++) {
        predecessors[i] = (moves *)malloc(cols * sizeof(moves));
        if(predecessors[i] == NULL) exit(EXIT_FAILURE);
    }
    q = createuCoda(rows * cols);
    startpoint.x = 0;
    startpoint.y = 0;
    enqueue(q, startpoint, 0); 
    distances[0][0] = 0;

    while (q->size > 0) {
        current = dequeue(q);
        aRows = current.moves.x;
        aCols = current.moves.y;
        currentCost = current.cost;
        for (i = 0; i < 4; ++i) {
            newRow = aRows + direzione[i].x;
            newCol = aCols + direzione[i].y;
            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
                edgeCost = ccell + cheight * ((matrix[aRows][aCols] - matrix[newRow][newCol]) * (matrix[aRows][aCols] - matrix[newRow][newCol]));
                if (currentCost + edgeCost < distances[newRow][newCol]) {
                    distances[newRow][newCol] = currentCost + edgeCost;
                    temp.x = aRows;
                    temp.y = aCols;
                    predecessors[newRow][newCol] = temp;
                    temp.x = newRow;
                    temp.y = newCol;
                    enqueue(q, temp, currentCost + edgeCost);
                }
            }
        }
    }

    finalCost = distances[rows - 1][cols - 1];

    path = (moves *)malloc(rows * cols * sizeof(moves));
    if(path == NULL) exit(EXIT_FAILURE);
    printPath.x = rows -1;
    printPath.y = cols -1;
    pathLength = 0;
    while (printPath.x != 0 || printPath.y != 0) {
        path[pathLength++] = printPath;
        printPath = predecessors[printPath.x][printPath.y];
    }
    startpoint.x = 0;
    startpoint.y = 0;
    path[pathLength++] = startpoint; 
    for (i = pathLength - 1; i >= 0; --i) {
        printf("%d %d\n", path[i].x, path[i].y);
    }
    printf("-1 -1\n%ld\n", finalCost + ccell);
    for (i = 0; i < rows; ++i) {
        free(distances[i]);
        free(predecessors[i]);
    }
    free(path);
    free(distances);
    free(predecessors);
    free(q->coda);
    free(q);
}


int main(int argc, char *argv[]) {
    /*Contatori*/
    int i, j;
    /*salvo la tabella*/
    int **matrix;
    /*variabili che devo prendere in input*/
    int rows, cols, ccell, cheight;
    FILE*file;

    if (argc != 2) return EXIT_FAILURE;

    file = fopen(argv[1], "r");
    if (file == NULL) return EXIT_FAILURE;

    fscanf(file, "%d\n%d\n%d\n%d", &ccell, &cheight, &rows, &cols); 

    matrix = (int **)malloc(rows * sizeof(int *));
    if(matrix == NULL) return EXIT_FAILURE;
    for (i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
        if(matrix[i] == NULL) return EXIT_FAILURE;
        for (j = 0; j < cols; j++) {
            fscanf(file, "%d", &matrix[i][j]); 
        }
    }

    fclose(file);

    findMinPath(rows, cols, matrix, ccell, cheight);
    
    for (i = 0; i < rows; i++) {
        free(matrix[i]);
    }

    free(matrix);

    return 0;
}

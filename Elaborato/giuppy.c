// Giuseppe Cattolico 0001124318 CLASSE B giuseppe.cattolico@studio.unibo.it
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

typedef struct {
    int x, y;
} pos;

typedef struct {
    pos pos;
    int prezzo;
} Nodo;

typedef struct {
    Nodo* coda;
    int dimensione;
} CodaPriority;

/* Funzione che ripristina la propriet� dell'heap (min-heap) spostando un elemento verso il basso */
int down_heap(CodaPriority* u, int n) {
    int min = n;
    int sx = (2 * n) + 1;
    int dx = (2 * n) + 2;

    if (sx < u->dimensione && u->coda[sx].prezzo < u->coda[min].prezzo)
        min = sx;
    if (dx < u->dimensione && u->coda[dx].prezzo < u->coda[min].prezzo)
        min = dx;

    if (min != n) {
        // Scambio diretto degli elementi
        Nodo temp = u->coda[n];
        u->coda[n] = u->coda[min];
        u->coda[min] = temp;
        down_heap(u, min);
    }
}

/* Funzione che ripristina la propriet� dell'heap (min-heap) spostando un elemento verso l'alto */
int riseHeap(CodaPriority* queue, int index) {
    int parentIndex;
    while (index > 0) {
        parentIndex = (index - 1) / 2;
        if (queue->coda[parentIndex].prezzo <= queue->coda[index].prezzo) {
            break;
        }
        // Scambio diretto degli elementi
        Nodo temp = queue->coda[parentIndex];
        queue->coda[parentIndex] = queue->coda[index];
        queue->coda[index] = temp;
        index = parentIndex;
    }
}


/* Funzione che crea e inizializza una coda con priorit� (heap) */
CodaPriority* crea_coda(int dimensione) {
    CodaPriority* u = (CodaPriority*)malloc(sizeof(CodaPriority));
    if (u == NULL) return 0;
    u->coda = (Nodo*)malloc(dimensione * sizeof(Nodo));
    if (u->coda == NULL) return 0;
    u->dimensione = 0;
    return u;
}

/* Funzione che inserisce un elemento nella coda con priorit� (heap) */
int incoda(CodaPriority* queue, pos item, long int priority) {
    queue->coda[queue->dimensione].pos = item;
    queue->coda[queue->dimensione].prezzo = priority;
    queue->dimensione++;
    riseHeap(queue, queue->dimensione - 1);
}

/* Funzione che rimuove e ritorna l'elemento con la minima priorit� dalla coda */
Nodo fuori_coda(CodaPriority* u) {
    Nodo minNode = u->coda[0];
    u->coda[0] = u->coda[u->dimensione - 1];
    u->dimensione--;
    down_heap(u, 0);
    return minNode;
}

/* Funzione che calcola il percorso minimo in una matrice, considerando i prezzoi delle celle e delle variazioni di altitudine */
int calcolaPercorsoMinimo(int** matrice, int prezzooCella, int prezzooAltitudine, int righe, int colonne) {
    int r, c, lunghezzaPercorso, nuovaRiga, nuovaColonna;
    pos** precedenti, posStampata, * percorso, punto, temp;
    pos direzioni[] = { {-1,0}, {1,0}, {0,1}, {0,-1} };
    CodaPriority* coda;
    Nodo nodoCorrente;
    long int costoFinale = 0, costoAttuale = 0, costoArco = 0;
    int rCorrente, cCorrente;
    long int** distanze = (long int**)malloc(righe * sizeof(long int*));
    if (distanze == NULL) return 0;
    for (r = 0; r < righe; r++) {
        distanze[r] = (long int*)malloc(colonne * sizeof(long int));
        if (distanze[r] == NULL) return 0;
        for (c = 0; c < colonne; c++) {
            distanze[r][c] = LONG_MAX;
        }
    }
    precedenti = (pos**)malloc(righe * sizeof(pos*));
    if (precedenti == NULL) return 0;
    for (r = 0; r < righe; r++) {
        precedenti[r] = (pos*)malloc(colonne * sizeof(pos));
        if (precedenti[r] == NULL) return 0;
    }
    coda = crea_coda(righe * colonne);
    punto.x = 0;
    punto.y = 0;
    incoda(coda, punto, 0);
    distanze[0][0] = 0;

    while (coda->dimensione > 0) {
        nodoCorrente = fuori_coda(coda);
        rCorrente = nodoCorrente.pos.x;
        cCorrente = nodoCorrente.pos.y;
        costoAttuale = nodoCorrente.prezzo;
        for (r = 0; r < 4; ++r) {
            nuovaRiga = rCorrente + direzioni[r].x;
            nuovaColonna = cCorrente + direzioni[r].y;
            if (nuovaRiga >= 0 && nuovaRiga < righe && nuovaColonna >= 0 && nuovaColonna < colonne) {
                costoArco = prezzooCella + prezzooAltitudine * ((matrice[rCorrente][cCorrente] - matrice[nuovaRiga][nuovaColonna]) * (matrice[rCorrente][cCorrente] - matrice[nuovaRiga][nuovaColonna]));
                if (costoAttuale + costoArco < distanze[nuovaRiga][nuovaColonna]) {
                    distanze[nuovaRiga][nuovaColonna] = costoAttuale + costoArco;
                    temp.y = cCorrente;
                    temp.x = rCorrente;
                    precedenti[nuovaRiga][nuovaColonna] = temp;
                    temp.y = nuovaColonna;
                    temp.x = nuovaRiga;
                    incoda(coda, temp, costoAttuale + costoArco);
                }
            }
        }
    }

    costoFinale = distanze[righe - 1][colonne - 1];

    percorso = (pos*)malloc(righe * colonne * sizeof(pos));
    if (percorso == NULL) return 0;
    posStampata.x = righe - 1;
    posStampata.y = colonne - 1;
    lunghezzaPercorso = 0;
    while (posStampata.x != 0 || posStampata.y != 0) {
        percorso[lunghezzaPercorso++] = posStampata;
        posStampata = precedenti[posStampata.x][posStampata.y];
    }
    punto.x = 0;
    punto.y = 0;
    percorso[lunghezzaPercorso++] = punto;
    for (r = lunghezzaPercorso - 1; r >= 0; --r) {
        printf("%d %d\n", percorso[r].x, percorso[r].y);
    }
    printf("-1 -1\n%ld\n", costoFinale + prezzooCella);
    for (r = 0; r < righe; ++r) {
        free(distanze[r]);
        free(precedenti[r]);
    }
    free(distanze);
    free(precedenti);
    free(percorso);
    free(coda->coda);
    free(coda);
}

int main(int argc, char* argv[]) {
    FILE* file; // Dichiarazione di un puntatore a FILE per gestire il file di input
    int i, j; // Dichiarazione delle variabili di controllo per i cicli
    int** matrice; // Dichiarazione di un puntatore a puntatore per la matrice
    int ccell, cheight, rows, cols; // Dichiarazione delle variabili per i parametri di input

    // Controllo che il programma riceva l'input necessario per funzionare
    if (argc != 2) return 1;

    // Apertura del file di input in modalit� read
    file = fopen(argv[1], "r");
    if (file == NULL) return 1;

    // Controllo che i parametri in input siano corretti
    if (fscanf(file, "%d %d %d %d", &ccell, &cheight, &rows, &cols) != 4) {
        return 1;
    }
    printf("%d %d %d %d\n",ccell, cheight, rows, cols);

    // Allocazione di spazio per la matrice
    matrice = (int**)malloc(rows * sizeof(int*));
    if (matrice == NULL) return 1;
    for (i = 0; i < rows; i++) {
        matrice[i] = (int*)malloc(cols * sizeof(int));
        if (matrice[i] == NULL) return 1;

        // Lettura dei valori della matrice dal file
        for (j = 0; j < cols; j++) {
            if (fscanf(file, "%d", &matrice[i][j]) != 1) {
                return 1;
            }
        }
    }
    fclose(file); // Chiusura del file di input

    // Calcolo del percorso minimo
    calcolaPercorsoMinimo(matrice, ccell, cheight, rows, cols);

    // Liberazione della memoria allocata per la matrice
    for (i = 0; i < rows; i++) {
        free(matrice[i]);
    }
    free(matrice); // Liberazione della memoria per la matrice
    return 0; 
}

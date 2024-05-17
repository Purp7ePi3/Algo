#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#define TANTO 100000

typedef struct {
    int valore;
    int quantita;
} Valuta;

int n;
int *m;
int **val;
int **path;
int somma;
Valuta *qused;
size_t size;

int minimum(int a, int b) {
    return a < b ? a : b;
}

int dp(int pos, int resto) {
    if (resto == 0) return 0;
    if (resto < 0 || pos >= n) return TANTO;

    if (val[pos][resto] != -1) return val[pos][resto];

    int prendo = dp(pos + 1, resto - m[pos]) + 1;
    int lascio = dp(pos + 1, resto);
    return val[pos][resto] = minimum(prendo, lascio);
}

void print_path(int pos, int resto) {
    int k = 0;
    if (pos >= n || resto <= 0)
        return;
    int prendo = dp(pos + 1, resto - m[pos]) + 1;
    int lascio = dp(pos + 1, resto);
    if (dp(pos, resto) == prendo) {
        while (k < size) {
            if (qused[k].valore == m[pos]) {
                qused[k].quantita++;    
                break;
            }
            k++;
        }
        if (k == size) {
            qused = realloc(qused, (size + 1) * sizeof(Valuta));
            qused[size].valore = m[pos];
            qused[size].quantita = 1;
            size++;
        }
        somma += m[pos];
        printf("%d ", m[pos]);
        print_path(pos + 1, resto - m[pos]);
    } else {
        print_path(pos + 1, resto);
    }
}

int main(int argc, char *argv[]) {
    int R, i;
    FILE *filein = stdin;

    if (argc != 2) {
        fprintf(stderr, "Invocare il programma con: %s input_file\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-") != 0) {
        filein = fopen(argv[1], "r");
        if (filein == NULL) {
            fprintf(stderr, "Can not open %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    if (fscanf(filein, "%d %d", &R, &n) != 2) {
        fprintf(stderr, "Errore durante la lettura di R e n\n");
        return EXIT_FAILURE;
    }

    m = (int *)malloc(n * sizeof(*m));
    assert(m != NULL);
    for (i = 0; i < n; i++) {
        if (fscanf(filein, "%d", &m[i]) != 1) {
            fprintf(stderr, "Errore durante la lettura della moneta %d di %d\n", i + 1, n);
            return EXIT_FAILURE;
        }
    }

    val = (int **)malloc(n * sizeof(int *));
    assert(val != NULL);
    for (i = 0; i < n; i++) {
        val[i] = (int *)malloc((R + 1) * sizeof(int));
        assert(val[i] != NULL);
        memset(val[i], -1, (R + 1) * sizeof(int));
    }
    int result = dp(0, R);

    qused = malloc(sizeof(Valuta)); 
    size = 0; 

    if (result >= TANTO) {
        printf("Impossibile dare il resto con le monete a disposizione");
        return -1;
    }
    printf("\nPercorso: ");
    print_path(0, R);
    printf("\n");
    printf("Ho dato un resto di: %d su %d con %d monete\n\n", somma, R, result);

    printf("Valuta usata:\n");
    for (i = 0; i < size; i++) {
        printf("%d x %d\n", qused[i].valore, qused[i].quantita);
    }

    free(m);
    for (i = 0; i < n; i++) {
        free(val[i]);
    }
    free(val);
    free(qused);

    if (filein != stdin) fclose(filein);

    return EXIT_SUCCESS;
}

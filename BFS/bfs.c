#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include "graph.h"
#include "list.h"

typedef struct QueueNode {
    int data;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* next;
    QueueNode* prec;
} Queue;

Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->next = NULL;
    queue->prec = NULL;
    return queue;
}

bool isEmpty(Queue* queue) {
    return (queue->next == NULL);
}

void enqueue(Queue* queue, int data) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->data = data;
    newNode->next = NULL;
    if (isEmpty(queue)) {
        queue->next = newNode;
        queue->prec = newNode;
    } else {
        queue->prec->next = newNode;
        queue->prec = newNode;
    }
}

int dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        return -1; 
    }
    int data = queue->next->data;
    QueueNode* temp = queue->next;
    queue->next = queue->next->next;
    free(temp);
    if (queue->next == NULL) {
        queue->prec = NULL;
    }

    return data;
}

void freeQueue(Queue* queue) {
    while (!isEmpty(queue)) {
        dequeue(queue);
    }
    free(queue);
}

const int NODE_UNDEF = -1;

typedef enum { WHITE, GREY, BLACK } Color; /* colori dei nodi */

/* Visita il grafo `g` usando l'algoritmo di visita in ampiezza (BFS)
   usando `s` come nodo sorgente. Restituisce il numero di nodi
   raggiungibili da `s`, incluso `s`. */
int bfs(const Graph *graph, int s, int *d, int *p) {
    bool *visited = (bool*)malloc(graph->n * sizeof(bool));
    for (int i = 0; i < graph->n; i++) {
        visited[i] = false;
        d[i] = INT_MAX; 
        p[i] = NODE_UNDEF; 
    }
    Queue* queue = createQueue();
    enqueue(queue, s);
    visited[s] = true;
    d[s] = 0;

    while (!isEmpty(queue)) {
        int u = dequeue(queue);

        Edge *current = graph->edges[u];
        while (current != NULL) {
            int v = current->dst;
            if (!visited[v]) {
                visited[v] = true;
                d[v] = d[u] + 1; 
                p[v] = u;
                enqueue(queue, v); 
            }
            current = current->next;
        }
    }

    freeQueue(queue);
    free(visited);

    int count = 0;
    for (int i = 0; i < graph->n; ++i) {
        if (d[i] != INT_MAX) {
            count++;
        }
    }
    return count;
}



/* Stampa il cammino che da `s` a `d` prodotto dalla visita in
   ampiezza; se `d` non è raggiungibile da `s`, stampa "Non
   raggiungibile". La stampa del cammino deve avere la forma:

   s->n1->n2->...->d

   dove n1, n2... sono gli identificatori (indici) dei nodi
   attraversati. */
void print_path(int s, int d, const int *p)
{
    if (s == d)
        printf("%d", s);
    else if (p[d] < 0)
        printf("Non raggiungibile");
    else {
        print_path(s, p[d], p);
        printf("->%d", d);
    }
}

/* Stampa le distanze dal nodo `src` di tutti gli altri nodi, e i
   cammini per raggiungerli. I cammini sono quelli prodotti dalla
   visita in ampiezza. L'array `p[]` indica l'array dei predecessori,
   cioè `p[i]` è il predecessore del nodo `i` nell'albero
   corrispondente alla visita BFS. Se un nodo non è raggiungibile da
   `src`, la sua distanza viene riportata come "-1" e il cammino viene
   sostituito dalla stringa "Non raggiungibile". */
void print_bfs( const Graph *g, int src, const int *d, const int *p )
{
    const int n = graph_n_nodes(g);
    int v;

    assert(p != NULL);
    assert(d != NULL);

    printf("  src | dest | distanza | path\n");
    printf("------+------+----------+-------------------------\n");
    for (v=0; v<n; v++) {
    printf(" %4d | %4d | %8d | ", src, v, d[v] == INT_MAX ? -1 : d[v]);
        print_path(src, v, p);
        printf("\n");
    }
}

int main( int argc, char *argv[] )
{
    Graph *G;
    int nvisited; /* n. di nodi raggiungibili dalla sorgente */
    int *p, *d;
    FILE *filein = stdin;
    int src = 0, n;

    if (argc != 3) {
        fprintf(stderr, "Uso: %s nodo_sorgente file_grafo\n", argv[0]);
        return EXIT_FAILURE;
    }

    src = atoi(argv[1]);

    if (strcmp(argv[2], "-") != 0) {
        filein = fopen(argv[2], "r");
        if (filein == NULL) {
            fprintf(stderr, "Can not open %s\n", argv[2]);
            return EXIT_FAILURE;
        }
    }

    G = graph_read_from_file(filein);
    n = graph_n_nodes(G);
    assert((src >= 0) && (src < n));
    p = (int*)malloc( n * sizeof(*p) ); assert(p != NULL);
    d = (int*)malloc( n * sizeof(*d) ); assert(d != NULL);
    nvisited = bfs(G, src, d, p);
    print_bfs(G, src, d, p);
    printf("# %d nodi su %d raggiungibili dalla sorgente %d\n", nvisited, n, src);
    graph_destroy(G);
    free(p);
    free(d);
    if (filein != stdin) fclose(filein);

    return EXIT_SUCCESS;
}

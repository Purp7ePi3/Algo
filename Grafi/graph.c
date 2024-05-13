#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph.h"

Graph *graph_create( int n, Graph_type t )
{
    int i;
    Graph *g = (Graph*)malloc(sizeof(*g));
    assert(g != NULL);
    assert(n > 0);

    g->n = n;
    g->m = 0;
    g->t = t;
    g->edges = (Edge**)malloc(n * sizeof(Edge*));
    assert(g->edges != NULL);
    g->in_deg = (int*)malloc(n * sizeof(*(g->in_deg)));
    assert(g->in_deg != NULL);
    g->out_deg = (int*)malloc(n * sizeof(*(g->out_deg)));
    assert(g->out_deg != NULL);
    for (i=0; i<n; i++) {
        g->edges[i] = NULL;
        g->in_deg[i] = g->out_deg[i] = 0;
    }
    return g;
}

void graph_destroy(Graph *g)
{
    int i;

    assert(g != NULL);

    for (i=0; i<g->n; i++) {
        Edge *edge = g->edges[i];
        while (edge != NULL) {
            Edge *next = edge->next;
            free(edge);
            edge = next;
        }
        g->edges[i] = NULL; /* e' superfluo */
    }
    free(g->edges);
    free(g->in_deg);
    free(g->out_deg);
    g->n = 0;
    g->edges = NULL;
    free(g);
}

Graph_type graph_type(const Graph *g)
{
    return g->t;
}


void graph_add_edge(Graph *g, int src, int dst, double weight)
{
    assert((src >= 0) && (src < graph_n_nodes(g)));
    assert((dst >= 0) && (dst < graph_n_nodes(g)));

    if(graph_type(g) == GRAPH_DIRECTED){
        Edge*new_edge = (Edge*)malloc(sizeof(Edge));
        assert(new_edge != NULL);
        new_edge->src = src;
        new_edge->dst = dst;
        new_edge->weight = weight;
        new_edge->next = g->edges[src];

        g->edges[src] = new_edge;

        g->out_deg[src]++;
        g->in_deg[dst]++;
        g->m++;
    }else{

        Edge*new_edge_src = (Edge*)malloc(sizeof(Edge));
        assert(new_edge_src != NULL);
        new_edge_src->src = src;
        new_edge_src->dst = dst;
        new_edge_src->weight = weight;
        new_edge_src->next = g->edges[src];
        g->edges[src] = new_edge_src;

        Edge*new_edge_dst = (Edge*)malloc(sizeof(Edge));
        assert(new_edge_dst != NULL);
        new_edge_dst->src = dst;
        new_edge_dst->dst = src;
        new_edge_dst->weight = weight;
        new_edge_dst->next = g->edges[dst];

        g->edges[dst] = new_edge_dst;

        g->out_deg[src]++;
        g->out_deg[dst]++;

        g->in_deg[dst]++;
        g->in_deg[src]++;
        g->m++;
    }
    /* Inserire un nuovo arco pesato <src, dst, weight> nel grafo.
       Il comportamento di questa funzione sarà leggermente diverso
       a seconda che il grafo sia orientato oppure no.
       Se il grafo è ORIENTATO:
       
       - L'arco (src,dst,weight) viene inserito nella lista di adiacenza
         del nodo `src`; dato che le liste di adiacenza non devono essere
         mantenute ordinate, l'arco può essere inserito all'inizio in modo
         da completare l'operazione in tempo O(1)
       - Si incrementa il grado uscente di `src`;
       - Si incrementa il grado entrante di `dst`;
       - Si incrementa il numero di archi del grafo.
       Se il grafo è NON ORIENTATO:
       - L'arco non orientato (src,dst,weight) viene inserito sia
         nella lista di adiacenza del nodo `src`, che in quella del
         nodo `dst`
       - Si incrementa il grado uscente sia di `src` che di `dst`;
       - Si incrementa il grado entrante sia di `dst` che di `src`;
       - Si incrementa il numero di archi del grafo.
       Per risparmiare tempo è possibile assumere che l'arco non
       esista già. Può comunque essere una buona idea fare il controllo
       per rendere il programma più robusto (è lasciata libertà di decidere
       come comportarsi in caso di tentativo di inserimento di un arco
       duplicato, ad esempio chiamando `abort()` per interrompere il programma
       oppure semplicemente ignorare l'inserimento). Il controllo però
       peggiora il costo asintotico dell'inserimento, che non è più O(1)
       ma diventa proporzionale al grado uscente di `src`. */
    /* [TODO] */
}

/* Funzione ricorsiva ausiliaria che rimuove l'arco che porta al nodo
   `dst` dalla lista di adiacenza `adj`. Se tale arco non è presente,
   non fa nulla. Ritorna la testa della lista modificata. Al termine
   pone `*deleted = 1` se l'arco è stato rimosso. */
static Edge *graph_adj_remove(Edge *adj, int dst, int *deleted)
{
    /* Questo è l'algoritmo ricorsivo standard per la rimozione da una
       lista concatenata singola. */
    if (adj == NULL) { /* caso base 1: lista vuota */
        *deleted = 0;
        return adj;
    } else if (adj->dst == dst) { /* caso base 2: il nodo da cancellare è il primo della lista. */
        Edge *result = adj->next;
        free(adj);
        *deleted = 1;
        return result;
    } else { /* caso ricorsivo */
        adj->next = graph_adj_remove(adj->next, dst, deleted);
        return adj;
    }
}

void graph_del_edge(Graph *g, int src, int dst)
{
    int del_srcdst, del_dstsrc;

    assert((src >= 0) && (src < graph_n_nodes(g)));
    assert((dst >= 0) && (dst < graph_n_nodes(g)));

    /* Rimuovi l'arco src -> dst. */
    g->edges[src] = graph_adj_remove(g->edges[src], dst, &del_srcdst);
    if (del_srcdst) {
        g->out_deg[src]--;
        g->in_deg[dst]--;
        g->m--;
    }
    if (g->t == GRAPH_UNDIRECTED) {
        /* Rimuovi l'arco dst -> src. */
        g->edges[dst] = graph_adj_remove(g->edges[dst], src, &del_dstsrc);
        /* L'asserzione seguente serve per assicurarsi che l'arco
           dst->src venga cancellato se e solo se src->dst lo è.
           Infatti, in un grafo non orientato ogni arco viene
           rappresentato dalla coppia src->dst e dst->src. Pertanto,
           se esiste l'arco "di andata" ma non quello "di ritorno" (o
           viceversa), il grafo non è stato costruito correttamente e
           il programma deve essere abortito. */
        assert(del_srcdst == del_dstsrc);
        if (del_dstsrc) {
            g->out_deg[dst]--;
            g->in_deg[src]--;
            /* Non bisogna decrementare g->m due volte. */
        }
    }
}

int graph_n_nodes(const Graph *g)
{
    assert(g != NULL);

    return g->n;
}

int graph_n_edges(const Graph *g)
{
    assert(g != NULL);

    return g->m;
}

int graph_out_degree(const Graph *g, int v)
{
    assert(g != NULL);
    assert((v >= 0) && (v < graph_n_nodes(g)));
    return g->out_deg[v];
}

int graph_in_degree(const Graph *g, int v)
{
    assert(g != NULL);
    assert((v >= 0) && (v < graph_n_nodes(g)));
    return g->in_deg[v];
}

Edge *graph_adj(const Graph *g, int v)
{
    assert(g != NULL);
    assert((v >= 0) && (v < graph_n_nodes(g)));

    return g->edges[v];
}

void graph_print(const Graph *g)
{
    int i;
    int outg = 1;
    assert(g != NULL);

    if (graph_type(g) == GRAPH_UNDIRECTED) {
        printf("UNDIRECTED\n");
    } else {
        printf("DIRECTED\n");
    }

    for (i=0; i<g->n; i++) {
        const Edge *e;
        int out_deg = 0; /* ne approfittiamo per controllare la
                            correttezza dei gradi uscenti */
        printf("[%2d] -> ", i);
        for (e = graph_adj(g, i); e != NULL; e = e->next) {
            printf("(%d, %d, %f) -> ", e->src, e->dst, e->weight);
            out_deg++;
        }
        if (i == outg) {
            printf("Outgoing %d: %d-> ",outg, out_deg);
        }
        assert(out_deg == graph_out_degree(g, i));
        printf("NULL\n");
    }
}

Graph *graph_read_from_file(FILE *f)
{
    int n, m, t;
    int src, dst;
    int i; /* numero archi letti dal file */
    double weight;
    Graph *g;

    assert(f != NULL);

    if (3 != fscanf(f, "%d %d %d", &n, &m, &t)) {
        fprintf(stderr, "ERRORE durante la lettura dell'intestazione del grafo\n");
        abort();
    };
    assert( n > 0 );
    assert( m >= 0 );
    assert( (t == GRAPH_UNDIRECTED) || (t == GRAPH_DIRECTED) );

    g = graph_create(n, t);
    /* Ciclo di lettura degli archi. Per rendere il programma più
       robusto, meglio non fidarsi del valore `m` nell'intestazione
       dell'input. Leggiamo informazioni sugli archi fino a quando ne
       troviamo, e poi controlliamo che il numero di archi letti (i)
       sia uguale a quello dichiarato (m) */
    i = 0;
    while (3 == fscanf(f, "%d %d %lf", &src, &dst, &weight)) {
        graph_add_edge(g, src, dst, weight);
        i++;
    }
    if (i != m) {
        fprintf(stderr, "WARNING: ho letto %d archi, ma l'intestazione ne dichiara %d\n", i, m);
    }
    /*
    fprintf(stderr, "INFO: Letto grafo %s con %d nodi e %d archi\n",
            (t == GRAPH_UNDIRECTED) ? "non orientato" : "orientato",
            n,
            m);
    */
    return g;
}

void graph_write_to_file( FILE *f, const Graph* g )
{
    int v;
    int n, m, t;

    assert(g != NULL);
    assert(f != NULL);

    n = graph_n_nodes(g);
    m = graph_n_edges(g);
    t = graph_type(g);

    fprintf(f, "%d %d %d\n", n, m, t);
    for (v=0; v<n; v++) {
        const Edge *e;
        for (e = graph_adj(g, v); e != NULL; e = e->next) {
            assert(e->src == v);
            /* Se il grafo è non orientato, dobbiamo ricordarci che
               gli archi compaiono due volte nelle liste di
               adiacenza. Nel file pero' dobbiamo riportare ogni arco
               una sola volta, dato che sarà la procedura di lettura a
               creare le liste di adiacenza in modo corretto. Quindi,
               ogni coppia di archi (u,v), (v,u) deve comparire una
               sola volta nel file. Per comodità, salviamo nel file la
               versione di ciascun arco in cui il nodo sorgente è
               minore del nodo destinazione. */
            if ((graph_type(g) == GRAPH_DIRECTED) || (e->src < e->dst)) {
                fprintf(f, "%d %d %f\n", e->src, e->dst, e->weight);
            }
        }
    }
}

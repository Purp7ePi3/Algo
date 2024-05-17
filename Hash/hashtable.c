
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "hashtable.h"

unsigned long hash_function(const HashTable *table, unsigned long k)
{
    assert(table != NULL);

    return k % table->size;
}

unsigned long encode(const char *key)
{
    int i;
    unsigned long s;

    assert(key != NULL);

    s = 0;
    /* Secondo lo standard C99
       (http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf
       sezione 6.2.5/9) l'aritmetica con tipi unsigned non produce mai
       overflow; non sono riuscito a trovare quale sia il
       comportamento atteso in ANSI C, ma per i fini di questo
       esercizio assumiamo che sia lo stesso. */
    for (i=0; key[i]; i++) {
        s += key[i];
    }
    return s;
}

/* Ritorna true (nonzero) se le chiavi k1 e k2 sono uguali, cioè se le
   stringhe sono uguali carattere per carattere. */
static int keys_equal(const char *k1, const char *k2)
{
    assert(k1 != NULL);
    assert(k2 != NULL);

    return (0 == strcmp(k1, k2));
}

HashTable *ht_create(const int size)
{
    HashTable *h = (HashTable*)malloc(sizeof(*h));
    int i;

    assert(h != NULL);
    h->size = size;
    h->values_count = 0;
    h->items = (HashNode **) malloc(h->size * sizeof(*(h->items)));
    assert(h->items != NULL);
    for (i = 0; i < h->size; i++) {
        h->items[i] = NULL;
    }
    return h;
}

/* Funzione ausiliaria che crea un nuovo nodo per le liste di trabocco
   contenente una copia della chiave `key` con valore associato il
   valore `value`. Il successore del nuovo nodo viene posto a
   `next`. */
static HashNode *hashtable_new_node(const char *key, int value, HashNode *next)
{
    HashNode *item = (HashNode *) malloc(sizeof(HashNode));
    const int keylen = strlen(key);

    assert(item != NULL);
    /* Per duplicare la stringa `key` sarebbe possibile usare la
       funzione `strdup()`. Purtroppo tale funzione non è parte di
       ANSI C né di C99, ma è inclusa nelle estensioni POSIX. Di
       conseguenza, potrebbe non essere disponibile ovunque. Conviene
       quindi realizzare la copia manualmente, allocando un buffer di
       lunghezza opportuna e poi copiando `key` in tale buffer. */
    item->key = (char*)malloc(keylen+1);
    assert(item->key != NULL);
    strcpy(item->key, key);
    item->next = next;
    item->value = value;
    return item;
}

/* Libera la memoria allocata per il nodo n della tabella hash.
   Libera anche la memoria riservata alla chiave. */
static void free_node(HashNode *n)
{
    assert(n != NULL);

    free(n->key);
    free(n);
}

int ht_insert(HashTable *h, const char *key, int value)
{
    unsigned long hash = hash_function(h, encode(key));
    HashNode *current = h->items[hash];
    HashNode *new_node = NULL;

    while (current != NULL) {
        if (keys_equal(current->key, key)) {
            current->value = value;
            return 0; 
        }
        current = current->next;
    }
    new_node = hashtable_new_node(key, value, h->items[hash]);
    h->items[hash] = new_node;
    h->values_count++;

    return 1; 
}


HashNode *ht_search(HashTable *h, const char *key)
{
    unsigned long hash = hash_function(h, encode(key));
    HashNode *current = h->items[hash];

    while (current != NULL) {
        if (keys_equal(current->key, key)) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}


int ht_delete(HashTable *h, const char *key)
{
    unsigned long hash = hash_function(h, encode(key));
    HashNode *current = h->items[hash];
    HashNode *prev = NULL;

   
    while (current != NULL) {
        if (keys_equal(current->key, key)) {
            if (prev == NULL) {
                h->items[hash] = current->next;
            } else {
                prev->next = current->next;
            }
            free_node(current);
            h->values_count--;
            return 1; 
        }
        prev = current;
        current = current->next;
    }

    return 0;
}


void ht_clear(HashTable *h)
{
    int i;

    assert(h != NULL);

    for (i = 0; i < h->size; i++) {
        HashNode *current = h->items[i];
        while (current != NULL) {
            HashNode *next = current->next;
            free_node(current);
            current = next;
        }
        h->items[i] = NULL;
    }
    h->values_count = 0;
}

void ht_destroy(HashTable *h)
{
    assert(h != NULL);

    ht_clear(h);
    free(h->items);
    h->items = NULL; /* non serve */
    h->size = h->values_count = 0;
    free(h);
}

int ht_count(const HashTable *h)
{
    assert(h != NULL);
    return (h->values_count);
}

void ht_print(const HashTable *h)
{
    int i;
    assert(h != NULL);
    for (i=0; i<h->size; i++) {
        const HashNode* iter;
        printf("[%3d] ", i);
        for (iter = h->items[i]; iter != NULL; iter = iter->next) {
            printf("->(%s, %d)", iter->key, iter->value);
        }
        printf("\n");
    }
}

#include <stdio.h>
#include <stdlib.h>

// Funzione per contare i modi in cui è possibile fare un ammontare 'm' con le monete fornite
int how_many_ways(int m, int* coins, int coins_size) {
    // Array per memorizzare il numero di modi per ottenere ogni importo da 0 a m
    int* memo = (int*)calloc(m + 1, sizeof(int));
    
    // Inizializzare il caso base
    memo[0] = 1;

    // Calcolare il numero di modi per ogni importo da 1 a m
    for (int i = 1; i <= m; i++) {
        memo[i] = 0;
    }

    // Considerare ogni moneta
    for (int j = 0; j < coins_size; j++) {
        int coin = coins[j];
        for (int i = coin; i <= m; i++) {
            memo[i] += memo[i - coin];
        }
    }

    int result = memo[m];
    free(memo);
    return result;
}

int main() {
    int coins[] = {2,7,17}; // Esempio di monete
    int m = 33; // Esempio di importo

    int result = how_many_ways(m, coins, sizeof(coins)/sizeof(coins[0]));
    printf("Numero di modi per ottenere %d: %d\n", m, result);

    return 0;
}

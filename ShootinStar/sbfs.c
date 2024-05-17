#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define WIN 0b111101111
#define LOSS 0
#define START 0b000010000

int moves[9] = {
    0b000011011,    // Esplosione 0
    0b000000111,    // Esplosione 1
    0b000110110,    // Esplosione 2
    0b001001001,    // Esplosione 3
    0b010111010,    // Esplosione 4
    0b100100100,    // Esplosione 5
    0b011011000,    // Esplosione 6
    0b111000000,    // Esplosione 7
    0b110110000     // Esplosione 8
};

int explode(int from, int pos) {
    return from ^ moves[pos];
}

bool get_state(int map, int pos) {
    return map & (1 << pos);
}

void print_map(int map) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int pos = j + i * 3;
            if (get_state(map, pos)) printf("%d", pos);
            else printf(".");
        }
        printf("\n");
    }
}

void solve() {
    int visited[600];
    int from[600];
    int move[600];
    memset(visited, 0, sizeof(visited));

    int queue[600];
    int front = 0, rear = -1;
    visited[START] = 1;
    queue[++rear] = START;

    while (front <= rear) {
        int curr = queue[front++];
        
        if (curr == WIN || curr == LOSS) continue;

        for (int i = 0; i < 9; ++i) {
            int next = explode(curr, i);
            if (get_state(curr, i) && !visited[next]) {
                visited[next] = true;
                from[next] = curr;
                move[next] = i;
                queue[++rear] = next;
            }
        }
    }

    printf("Posizioni non raggiunte:\n");
    for (int i = 0; i < 512; ++i) {
        if (!visited[i]) print_map(i);
    }

    printf("Percorso per la vittoria: ");
    int pos = WIN;
    while (pos != START) {
        printf("%d ", move[pos]);
        pos = from[pos];
    }

    printf("\nPercorso per la sconfitta: ");
    pos = LOSS;
    while (pos != START) {
        printf("%d ", move[pos]);
        pos = from[pos];
    }
    printf("\n");
}

int main() {
    solve();
    return 0;
}

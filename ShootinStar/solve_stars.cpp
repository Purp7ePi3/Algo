// g++ solve_stars.cpp -o solve -g -Wall -Wextra -Wpedantic -fsanitize=address ; ./solve

#include <bits/stdc++.h>

#define WIN 0b111101111
#define LOSS 0
#define START 0b000010000

// Queste sono le maschere da applicare alla mappa per eseguire la relativa mossa
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

// La mappa viene rappresentata da un bitset lungo 9 (uso un int)
// (9 caselle, 2 stati possibili)
// i bit impostati a zero rappresentano buchi neri, a uno le stelle
// 8 7 6 5 4 3 2 1 0 (0 nel LSB)

int explode(int from, int pos) {
    return from ^ moves[pos];
}

// true: stella, false: buco nero
bool get_state(int map, int pos) {
    return map & (1 << pos);
}


void print_map(int map) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int pos = j+i*3;
            if (get_state(map, pos)) std::printf("%d", pos);
            else std::printf(".");
        }
        printf("\n");
    }
}


void solve() {
    int visited[600];
    int from[600];
    int move[600];
    std::memset(visited, 0, sizeof(visited));

    std::queue<int> q;
    visited[START] = 1;
    q.push(START);

    // na bella bieffeesseee
    while(!q.empty()) {
        int curr = q.front();
        q.pop();

       if (curr == WIN || curr == LOSS) continue; //idealmente dopo una vittoria/sconfitta non si va piu avanti nel gioco

        for (int i = 0; i < 9; ++i) {
            int next = explode(curr, i);
            if (get_state(curr, i) && !visited[next]) {
                visited[next] = true;
                from[next] = curr;
                move[next] = i;
                q.push(next);
            }
        }
    }

    // Posizioni non raggiunte
    std::printf("Posizioni non raggiunte:\n");
    for (int i = 0; i < 512; ++i) {
        if (!visited[i]) print_map(i);
    }

    // Allora qua ho fatto un bordello con gli stack per girare in fretta (bugia sono solo pigro) la lista di numeri, fa schifo lo so
    // il cuore di questa parte di codice e' ripercorrere al contrario il percorso trovato dalla bfs    

    // Shortest path to win:
    std::printf("Percorso per la vittoria: ");
    std::stack<int> s;
    int pos = WIN;
    while(pos != START) {
        s.push(move[pos]);
        pos = from[pos];
    }
    while (!s.empty()) {
        std::printf("%d ", s.top());
        s.pop();
    }
    // Shortest path to lose:
    std::printf("\nPercorso per la sconfitta: ");
    pos = LOSS;
    while(pos != START) {
        s.push(move[pos]);
        pos = from[pos];
    }
    while (!s.empty()) {
        std::printf("%d ", s.top());
        s.pop();
    }
    std::printf("\n");
}


int main() {
    solve();

    return 0;
}
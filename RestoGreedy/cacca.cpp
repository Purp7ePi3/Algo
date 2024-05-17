#include <iostream>
#include <fstream> // Aggiunto
#include <vector>
using namespace std;

int n;
vector<int> m;

vector<vector<int>> memo;

int dp(int pos, int resto) {
    
    if (resto == 0) return 0;
    if (resto < 0 || pos >= n) return 10000;

    if (memo[pos][resto] != -1) return memo[pos][resto];

    int prendo = dp(pos+1, resto-m[pos]) + 1;
    int non_prendo = dp(pos+1, resto);

    return memo[pos][resto] = min(prendo, non_prendo);
}


int main() {
    ifstream input("resto-greedy.in"); // Apri il file di input
    int s;
    input >> s;
    input >> n;
    m.resize(n);
    memo.resize(n, vector<int>(s+1, -1)); // Correzione qui
    for (int i = 0; i < n; ++i) {
        input >> m[i];   
    }
    input.close(); // Chiudi il file di input
    cout << dp(0, s) << "\n";
    return 0;
}

#include <cstdio>
#include <vector>
#include <iostream>
#include <list>
#include <string>

using namespace std;

int N,M,R;
vector<vector<int> > matrix;
vector<int> sequence;
vector<vector<vector<vector<int> > > > table;


void readInput() {
    cin >> N >> M;
    matrix = vector<vector<int> >(N, vector<int>(N));
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> matrix[i][j];   
        }   
    }
    sequence.resize(M);

    for (int i = 0; i < M; i++) {
        cin >> sequence[i]; 
    }
    cin >> R; 
}


void printInput() {
    for(size_t i = 0; i < matrix.size(); i++) {
        for(size_t j = 0; j < matrix[i].size(); j++) {
            cout << matrix[i][j] << " ";
        }    
        cout << endl;
    }

    for(size_t i = 0; i < sequence.size(); i++) {
        cout << sequence[i] << " ";
    }
    cout << endl; 
    cout << R << endl;  
}


void printTable() {
    // Iterate over the 2D dimensions of the table
    for (size_t i = 0; i < table.size(); i++) {
        for (size_t j = 0; j < table[i].size(); j++) {
            // Check if the table[i][j] is initialized before accessing its elements
            if (!table[i][j].empty() && !table[i][j][0].empty()) {
                cout << "[";
                for (size_t m = 0; m < table[i][j].size(); m++){
                    cout << table[i][j][m][0] << " ";
                }
                cout << "] ";
            } 
            else {
                cout << "X    ";  // Indicates uninitialized or empty values
            }
        }    
        cout << endl;
    } 
}


vector<vector<int>> B(int i, int j) { 
    vector<vector<int> > results;
    vector<bool> bs = vector<bool>(N, false); 
    if (i == j) {
        vector<int> bola = {sequence[i], 0, 0, 0};
        results.push_back(bola);
    }
    else if (j - i == 1) {
        int result = matrix[sequence[i] - 1][sequence[j] - 1];
        vector<int> bola = {result, sequence[i], i, sequence[j]};
        results.push_back(bola);
    }
    else {
        //percorrer a sequencia metendo os parenteses em todos os k possiveis
        int count = 0; 
        for (int k = j-1; k >= i && count < N; k--) {
            int r, e, d;
            //Testar os resultados possíveis nos dois lados dos parenteses 
            for (size_t m = 0; m < table[i][k].size() && count < N; m++) {
                e = table[i][k][m][0];
                for (size_t n = 0; n < table[k+1][j].size() && count < N; n++) {
                    d = table[k+1][j][n][0];
                    r = matrix[e-1][d-1];  
                    // Verificar se não está vazio, se não estiver, meter bola nova
                    // com resultado diferente e com parênteses mais à esquerda
                    if(!bs[r]) {
                        vector<int> bola = {r, e, k, d};
                        count++;
                        results.push_back(bola);
                        bs[r] = true;
                    }
                }  
            }
        }      
    }
    return results;
 }


void buildTable() {
    table = vector<vector<vector<vector<int>>>>(M, vector<vector<vector<int>>>(M));
    for (int k = 0; k < M; k++) {
        for (int i = 0; i < M - k; i++) {
            int j = i + k;
            table[i][j] = B(i,j);
        }
    }
}


void solve(int i, int j, int r) { 
    if (i == j) {
        cout << sequence[i];
    } else {
        for (size_t m = 0; m < table[i][j].size(); m++) {
            if (r == table[i][j][m][0]) {
                cout << "("; 
                solve(i, table[i][j][m][2], table[i][j][m][1]);
                cout << " ";
                solve(table[i][j][m][2]+1, j, table[i][j][m][3]);
                cout << ")";
            }
        }
    }
}


int main () { 
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);

    readInput();
    //printInput();
    buildTable();
    //printTable();

    for (size_t m = 0; m < table[0][M-1].size(); m++) {
        if (R == table[0][M-1][m][0]) {
            cout << 1 << endl;
            solve(0, M-1, R);
            cout << endl;
            return 0;
        }
    }
    
    cout << 0 << endl;
    return 0;
}
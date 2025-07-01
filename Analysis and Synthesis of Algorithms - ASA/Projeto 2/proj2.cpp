/* 2º Projeto de ASA - Grupo 127 */

#include <cstdio>
#include <vector>
#include <iostream>
#include <list>
#include <string>
#include <queue>
#include <unordered_map>
#include <set>
#include <limits>
#include <algorithm>

using namespace std;


size_t N;
int M, L;


vector<vector<int>> graph;
vector<vector<int>> linesGraph;

void processInput(bool& unconnected, bool& zeroChanges) {
    cin >> N >> M >> L;

    vector<bool> bs = vector<bool>(N + 1, false);

    graph.resize(L + 1);

    for (int i = 0; i < M; ++i) {
        int x, y, line;
        cin >> x >> y >> line;
        
        // Verificar se a estação não está na linha
        if (find(graph[line].begin(), graph[line].end(), x) == graph[line].end()) {
            graph[line].push_back(x);
            bs[x] = true;
        }
        if (find(graph[line].begin(), graph[line].end(), y) == graph[line].end()) {
            graph[line].push_back(y);
            bs[y] = true;
        }
        if (graph[line].size() == N) {
            zeroChanges = true;
        }
    }

    if (zeroChanges) {
        return;
    }

    for(size_t i = 1; i < N + 1; i++) {
        if(!bs[i]) {
            unconnected = true;
            return;
        }
    }       
}

void printGraph(vector<vector<int>> graph) {
    for(size_t i = 1; i < graph.size(); ++i) {
        cout << "Line" << i << ": ";
        for(size_t j = 0; j < graph[i].size(); j++) {
            cout << graph[i][j] << " ";
        }    
        cout << endl;
    }  
}

void buildLinesGraph() {
    linesGraph = vector<vector<int> >(L + 1, vector<int>(L + 1));

    for (int i = 1; i <= L; i++) {
        for (size_t j = 0; j < graph[i].size(); j++) {
            for (int l = i+1; l <= L; l++) {
                for (size_t k = 0; k < graph[l].size(); k++) {
                    if (graph[i][j] == graph[l][k]) {
                        linesGraph[i][l] = 1;
                        linesGraph[l][i] = 1;
                    }
                }        
            }
        }
    }
}

int bfs(int startLine) {
    // Define a fila para BFS e o vetor de distâncias
    queue<int> q;
    vector<int> distance(L + 1, -1); // Inicializamos as distâncias com -1 (não visitado)

    // Inicializa a BFS com a linha inicial
    q.push(startLine);
    distance[startLine] = 0;

    // Executa a BFS
    while (!q.empty()) {
        int currentLine = q.front();
        q.pop();

        // Percorre todas as linhas conectadas à linha atual
        for (int nextLine = 1; nextLine <= L; ++nextLine) {
            if (linesGraph[currentLine][nextLine] == 1 && distance[nextLine] == -1) {
                distance[nextLine] = distance[currentLine] + 1;
                q.push(nextLine);
            }
        }
    }

    // Retorna a distância máxima encontrada (número máximo de mudanças de linha)
    return *max_element(distance.begin(), distance.end());
}


// Calculate the metro connectivity index
int calculate_connectivity_index(int L) {
    int max_changes = 0;

    for (int i = 1; i <= L; i++) {
        int changes = bfs(i);
        max_changes = max(max_changes, changes);
    }

    return max_changes;
}

int main () { 
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);

    bool unconnected = false;
    bool zeroChanges = false;
    processInput(unconnected, zeroChanges);
    if(unconnected){
        cout << -1 << endl;
        return 0;
    }
    if(zeroChanges){
        cout << 0 << endl;
        return 0;
    }

    //printGraph(graph);
    buildLinesGraph();
    //printGraph(linesGraph);

    // Calculate and print the connectivity index
    int result = calculate_connectivity_index(L);
    cout << result << endl;
    

    return 0;
}
#include <cstdio>
#include <vector>
#include <deque>
#include <cstring>
#include <climits>
#define INF INT_MAX
const int MAXN = 10000;

using namespace std;

int n; //numero de vertices do grafo
vector<int> A, B; //particoes A e B
int dist[MAXN];

//retorna true se achou um caminho de aumento e false caso contrario
bool acha_caminhos_de_aumento(vector<int> G[MAXN], int m[]) {
    deque<int> queue; 
 	int k = INF;
  
    for (int u : A) {
   		if (m[u] == u) { //se u é livre adiciona na fila
            dist[u] = 0;
            queue.push_back(u);
        }
        else dist[u] = INF;
    }
 
    while (!queue.empty()) {     
        int u = queue.front();
        queue.pop_front();
 
        if (dist[u] < k) {
            for (int v : G[u]) {
               if (v == m[v] && k == INF)
 					k = dist[u] + 1;
                else if (dist[m[v]] == INF){
                    dist[m[v]] = dist[u] + 1;
                    queue.push_back(m[v]);
                }
            }
        }
    }
 
   	return (k != INF);
}
 
// Retorna true se existe um caminho de aumento comecando no vertice livre u e aumenta o emparelhamento
bool aumenta_emparelhamento(vector<int> G[MAXN], int m[], int u) {
   for (int v : G[u]) {
   		if (m[v] == v || (dist[m[v]] == dist[u] + 1 && aumenta_emparelhamento(G, m, m[v]))) {
   			m[v] = u;
   			m[u] = v;
   			return true;
   		} 
   }
   return false;
}
 

void hopcroft_karp(vector<int> G[MAXN]) {
	int m[MAXN];
	for (int v = 0; v < n; v++)	m[v] = v;

	int max_matching = 0;
	while (acha_caminhos_de_aumento(G, m)) {
		for (int v : A)
			if (m[v] == v && aumenta_emparelhamento(G, m, v))
				max_matching++;
	}
	printf("Tamanho do emparelhamento maximo: %d\n", max_matching);
	for (int v : A) {
		if (m[v] != v) printf("%d está emparelhado com %d\n", v, m[v]);
	}
}

int main() {
	int nA, nB; //numero de vertices da particao A e B
	int e; //numero de arestas do grafo
	vector<int> G[MAXN]; //grafo representado por listas de adjacencia
	scanf("%d %d %d %d", &n, &nA, &nB, &e);

	//Lendo as particoes A e B
	int w;
	while (nA--) 
		scanf("%d", &w), A.push_back(w);

	while (nB--) 
		scanf("%d", &w), B.push_back(w);

	//Leitura do grafo
	while(e--) {
		int u, v;
		scanf("%d %d", &u, &v);
		G[u].push_back(v);
		G[v].push_back(u);
	}

	hopcroft_karp(G);
 
    return 0;
}
#include <cstdio>
#include <vector>
#include <cstring>
const int MAXN = 10000;

using namespace std;

int seen[MAXN];

bool acha_caminho_aumento(int u, vector<int> G[MAXN], int m[]) {
	for (int v : G[u]) {
		if (seen[v]) continue;
		seen[v] = true;
		if (m[v] == v || acha_caminho_aumento(m[v], G, m)) {
			m[u] = v;
			m[v] = u;
			return true;
		}
	}
	return false;
}

int main() {
	int n; //numero de vertices do grafo
	int nA, nB; //numero de vertices da particao A e B
	vector<int> A, B; //particoes A e B
	int e; //numero de arestas do grafo
	vector<int> G[MAXN]; //grafo representado por listas de adjacencia
	int m[MAXN]; //representação do emparelhamento

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

	//Iniciando o emparelhamento
	for (int v = 0; v < n; v++) m[v] = v;

	int max_matching = 0;
	for (int v : A)
		if (m[v] == v) { //se v e livre
			memset(seen, 0, sizeof seen);
			if (acha_caminho_aumento(v, G, m))
				max_matching++;
		}

	printf("O tamanho do emparelhamento maximo = %d\n", max_matching);
	for (int v : A) {
		if (m[v] != v) printf("%d está emparelhado com %d\n", v, m[v]);
	}
	return 0;
}

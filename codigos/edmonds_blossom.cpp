#include <bits/stdc++.h>

using namespace std;
const int NMAX=500;

struct struct_edge{
    int v;
    struct_edge* n;
};

typedef struct_edge* Edge;
struct_edge edges[NMAX * NMAX * 2];
Edge top=edges, adj[NMAX];

int V, E; //Numero de vertices e arestas respectivamente
int match[NMAX]; //Representacao do emparelhamento
int qh, qt, q[NMAX], pai[NMAX], base[NMAX], inQ[NMAX], inB[NMAX]; //Variaveis auxiliares p marcar blossom, construir a arvore e descomprimir o caminho
bool G[NMAX][NMAX]; //Grafo representado por tabela

void add_edge(int u, int v) {
    top->v = v; 
    top->n = adj[u]; 
    adj[u] = top++;

    top->v = u;
    top->n = adj[v];
    adj[v] = top++;
}

int LCA(int root, int u, int v) {
    bool inP[NMAX];
    memset(inP, 0, sizeof(inP));
    while(42) {
        u = base[u];
        inP[u] = true;
        if (u == root) break;
        u = pai[match[u]];
    }

    while(42) {
        v = base[v];
        if (inP[v]) return v;
        else v = pai[match[v]];
    }
}

void marca_blossom(int lca, int u) {
    while (base[u] != lca) {
        int v = match[u];
        inB[base[u]] = inB[base[v]] = true;
        u = pai[v]; 
        if (base[u] != lca) pai[u] = v;
    }
}

void contrai_blossom(int s, int u, int v) {
    int lca = LCA(s, u, v);
    memset(inB, 0, sizeof(inB));
    marca_blossom(lca, u);
    marca_blossom(lca, v);
    
    if (base[u] != lca) pai[u] = v;
    if (base[v] != lca) pai[v] = u;

    for (int u = 0; u < V; u++)
        if (inB[base[u]]) {
            base[u] = lca;
            if (!inQ[u]) { 
                q[++qt] = u;
                inQ[u] = true;
            }
        }
}

int acha_caminho_aumento(int s) {
    memset(inQ, 0, sizeof(inQ));
    memset(pai, -1, sizeof(pai));

    for (int i = 0;i < V; i++) base[i] = i;
    qh = qt = 0;
    q[0] = s;
    inQ[s] = true;
    
    while (qh <= qt) {
        int u = q[qh++];
        for (Edge e = adj[u]; e; e = e->n) {
            int v = e->v;
            if (base[u] != base[v] && match[u] != v) {
                if ((v == s) || (match[v] != -1 && pai[match[v]] != -1))
                    contrai_blossom(s, u, v);
                else if (pai[v] == -1) {
                    pai[v] = u;
                    if (match[v] == -1) return v;
                    else if (!inQ[match[v]]) {
                        q[++qt] = match[v];
                        inQ[match[v]] = true;
                    }
                }
            }
        }
    }
    return -1;
}

int aumenta_emparelhamento(int s, int t) {
    int u = t, v, w;
    while (u != -1) {
        v = pai[u];
        w = match[v];
        match[v] = u;
        match[u] = v;
        u = w;
    }
    return t != -1;
}

int edmonds_blossom() {
    int match_new = 0;
    memset(match, -1, sizeof(match));
    for (int u = 0; u < V; u++)
        if (match[u] == -1)
            match_new += aumenta_emparelhamento(u, acha_caminho_aumento(u));
    return match_new;
}

int main() {
    scanf("%d %d", &V, &E);

    while(E--) {
        int u, v;
        scanf("%d %d", &u, &v);
        u--; v--;
        if (!G[u][v]) {
            add_edge(u, v);
            G[u][v] = G[v][u] = true;
        }
    }

    printf("O tamanho do emparelhamento maximo = %d\n", edmonds_blossom());
    for (int v = 0; v < V; v++) {
        if (v < match[v]) printf("%d está emparelhado com %d\n", v, match[v]);
    }
}
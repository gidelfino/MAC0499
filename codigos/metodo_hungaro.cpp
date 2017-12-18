#include <bits/stdc++.h>
#define MAXN 1000
#define INF INT_MAX 

using namespace std;

int custo[MAXN][MAXN]; //Matriz de custo
int n; //Numero de vertices em cada particao
int max_matching; //Cardinalidade do emparelhamento maaimo
int lA[MAXN], lB[MAXN]; //labels das particoes A e B
int mA[MAXN]; //mA[a] - vertice emparelhado com a
int mB[MAXN]; //mB[b] - vertice emparelhado com b
bool S[MAXN], T[MAXN]; //conjuntos S e T descritos na monografia
int folga[MAXN]; //vetor de folga descrito na monografia
int folgaA[MAXN]; //folgaA[b] equivale a um vertice que l(folgaA[b]) + l(b) - custo(folgaA[b],b) = folga[b]
int alt_path[MAXN]; //vetor que guarda os caminhos alternantes

void inicializa() {
	memset(lA, 0, sizeof(lA));
	memset(lB, 0, sizeof(lB));
	for (int a = 0; a < n; a++)
		for (int b = 0; b < n; b++)
			lA[a] = max(lA[a], custo[a][b]);
}

void atualiza_dual() {
	int a, b, delta = INF; 
	for (b = 0; b < n; b++) { //Calcula o delta usando o vetor de folgas
		if (!T[b])
			delta = min(delta, folga[b]);
	}
	for (a = 0; a < n; a++) //atualiza A
		if (S[a]) lA[a] -= delta;
	for (b = 0; b < n; b++) //atualiza B
		if (T[b]) lB[b] += delta;
	for (b = 0; b < n; b++) //Atualiza vetor de folgas
		if (!T[b]) folga[b] -= delta;
}

//v: vertice atual
//alt_path_a: vertice de A que aparece antes de v no caminho alternante,
//a funcao adiciona as arestas (alt_path_a, mA[a]), (mA[a], a)
void adiciona_na_arvore(int a, int alt_path_a) {
	S[a] = true; 
	alt_path[a] = alt_path_a; 

	//atualiza os vetores de folga pois adicionamos vertice novo em S
	for (int b = 0; b < n; b++) 
		if (lA[a] + lB[b] - custo[a][b] < folga[b]) {
			folga[b] = lA[a] + lB[b] - custo[a][b];
			folgaA[b] = a;
		}
}

void acha_caminho_de_aumento() {
	if (max_matching == n) return; //se o emparelhamento ja e perfeito podemos retornar
	int a, b, root; 
	int q[MAXN], wr = 0, rd = 0; //variaveis que simulam a fila da BFS

	memset(S, false, sizeof(S)); 
	memset(T, false, sizeof(T)); 
	memset(alt_path, -1, sizeof(alt_path)); 
	
	for (a = 0; a < n; a++) //encontrando a raiz da arvore
		if (mA[a] == -1) {
			q[wr++] = root = a;
			alt_path[a] = -2;
			S[a] = true;
			break;
		}

	for (b = 0; b < n; b++) { //inicializa vetor de folga
		folga[b] = lA[root] + lB[b] - custo[root][b];
		folgaA[b] = root;
	}

	while (true) { //ciclo principal da funcao
		while (rd < wr) {//condicao de ciclo da BFS
			a = q[rd++]; 
			for (b = 0; b < n; b++) 
				if (custo[a][b] == lA[a] + lB[b] && !T[b]) {
					if (mB[b] == -1) break; //vertice livre em B encontrado, logo caminho de aumento
					T[b] = true; //caso contrario adiciona b em T
					q[wr++] = mB[b]; //adiciona vertice mB[b] que esta emparelhado com b
					adiciona_na_arvore(mB[b], a); 
				}
				if (b < n) break; //caminho de aumento encontrado
		}
		if (b < n) break; 

		atualiza_dual(); 
		wr = rd = 0; 
		for (b = 0; b < n; b++) {
			if (!T[b] && folga[b] == 0) {	
				if (mB[b] == -1) {
					a = folgaA[b];
					break;
				}
				else {
					T[b] = true; 
					if (!S[mB[b]]) {
						q[wr++] = mB[b]; 
						adiciona_na_arvore(mB[b], folgaA[b]);
					}
				}
			}
			if (b < n) break; 
		}

		if (b < n) {
			max_matching++; 
			//aplicando a operacao de diferenca simetrica no caminho encontrado
			for (int ca = a, cb = b, tb; ca != -2; ca = alt_path[ca], cb = tb) {
				tb = mA[ca];
				mB[cb] = ca;
				mA[ca] = cb;
			}
			acha_caminho_de_aumento(); 
		}
	}
}

int metodo_hungaro() {
	int peso_matching = 0; //peso do matching otimo
	max_matching = 0; 
	memset(mA, -1, sizeof(mA)); 
	memset(mB, -1, sizeof(mB));
	inicializa(); 
	acha_caminho_de_aumento(); 
	for (int v = 0; v < n; v++) //forming answer there
		peso_matching += custo[v][mA[v]];
	return peso_matching;
}

int main() {
	scanf("%d", &n);
	for (int a = 0; a < n; a++)
		for (int b = 0; b < n; b++)
			scanf("%d", &custo[a][b]);
	int peso_matching = metodo_hungaro();
	printf("Peso do emparelhamento maximo: %d\n", peso_matching);
	for (int a = 0; a < n; a++) {
		if (mA[a] != -1) printf("%d está emparelhado com %d\n", a, mA[a]);
	}
	return 0;
}
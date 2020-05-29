#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct Graph {
	int n;  // nr de noduri
	int m;  // nr de muchii
	int x;  // nr de reprezentanti
	int **adj;  // matricea de adiacenta
} Graph;

// Functia de citire a datelor si creare a matricii de adiacenta
void readAdjMatrix(Graph **g, int *repr, FILE *input, FILE *output) {
	fscanf(input, "%d %d %d", &(*g)->n, &(*g)->m, &(*g)->x);
	int src, dest, weight;

	(*g)->adj = calloc((*g)->n+1, sizeof(int *));
	for (int i = 1; i <= (*g)->n; i++)
		(*g)->adj[i] = (int*)calloc((*g)->n+1, sizeof(int));

	for (int i = 0; i < (*g)->m; i++) {
		fscanf(input, "%d %d %d", &src, &dest, &weight);
		(*g)->adj[src][dest] = weight;
		(*g)->adj[dest][src] = weight;
	}
	for (int i = 0; i < (*g)->x; i++)
		fscanf(input, "%d", &repr[i]);
}
// Functia utilitara de a gasi nodul distanta minima
// dintre nodurile care inca nu au fost incluse in drum
int min_distance(int dist[], int included[], Graph *g) {
	int min = INT_MAX;
	int index;

	for (int i = 1; i <= g->n; i++)
		if (included[i] == 0 && dist[i] <= min) {
			min = dist[i];
			index = i;
		}
	return index;
}
//printare de verificare
void print_final(int repr[], int dist[], Graph *g, FILE *output) {
	int min = dist[repr[0]];
	int nod = repr[0];
	for(int i = 0; i < g->x; i++) {
		if (dist[repr[i]] < min) {
			min = dist[repr[i]];
			nod = repr[i];
		}
	}
	fprintf(output, "%d %d", nod, min);
}
// Functia de printare a reprezentantului si distantei
void printSolution(int dist[], FILE *output, Graph *g, int poz[]) {
	for (int i = 1; i <= g->n; i++) {
		if (dist[i] == INT_MAX)
			fprintf(output, "-1\n");
		else
			fprintf(output, "%d %d\n", poz[i], dist[i]);
	}
}
// Functie utilitara de comparare a distantelor minime intre reprezentanti
void compare(int dist[], Graph *g, int aux[], int poz[], int src) {
	for (int i = 1; i <= g->n; i++) {
		if (dist[i] < aux[i]) {
			aux[i] = dist[i];
			poz[i] = src;
		}
	}
}
// Functia care implementeaza algoritmul dijkstra
// de gasire a drumul minim de la fiecare reprezentant la fiecare nod
void shortest_path(Graph *g, int from, FILE *output,
				int repr[], int aux[], int poz[]) {
	// vectorul care pastreaza cel mai scurt drum
	// de la nodul sursa la nodul i
	int *dist = calloc(g->n, sizeof(int));
	// vector de mentinere a starii 1/0
	// 1- daca nodul i este inclus in drumul cel mai scurt
	int *included = calloc(g->n, sizeof(int));
	// initializam toate distantele cu infinit si included cu 0
	for (int i = 1; i <= g->n; i++) {
		dist[i] = INT_MAX;
		included[i] = 0;
	}
	dist[from] = 0;  // distanta de la sursa la el insusi mereu e 0
	int src;
	int i = 1;
	while (i <= g->n - 1) {
		src = min_distance(dist, included, g);
		// marchez nodul ca verificat deja
		included[src] = 1;
		// parcurg toate nodurile si updatez distanta nodului ales pentru nodurile vecine
		for (int target = 1; target <= g->n; target++)
			if (included[target] == 0 && g->adj[src][target] != 0 && dist[src] != INT_MAX
						&& dist[src] + g->adj[src][target] < dist[target])
				dist[target] = dist[src] + g->adj[src][target];
		i++;
	}
	compare(dist, g, aux, poz, from);
	free(included);
	free(dist);
}
// Functia de dezalocare a memoriei
void freegraf(Graph *g) {
	for (int i = 1; i <= g->n; i++)
		free(g->adj[i]);
	free(g->adj);
	free(g);
}
int main() {
	FILE *input = fopen("reprezentanti.in", "r");
	FILE *output = fopen("reprezentanti.out", "w");
	int repr[1001];
	Graph *g = (Graph *)calloc(1, sizeof(Graph));
	readAdjMatrix(&g, repr, input, output);

	// vectorul pastreaza distantele reprezentantilor
	int *aux = calloc(g->n, sizeof(int));
	// pastreaza reprezentantul pana la care este distanta minima
	int *poz = calloc(g->n, sizeof(int));
	for (int i = 1; i <= g->n; i++) {
		aux[i] = INT_MAX;
		poz[i] = 0;
	}
	// aplicarea algoritmului dijkstra
	for (int i = 0; i < g->x; i++)
		shortest_path(g, repr[i], output, repr, aux, poz);

	printSolution(aux, output, g, poz);
	free(aux);
	free(poz);
	freegraf(g);
	fclose(input);
	fclose(output);

	return 0;
}

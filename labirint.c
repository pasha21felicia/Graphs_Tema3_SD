#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
const int Up = 0;
const int Down = 1;
const int Left = 2;
const int Right = 3;

// Functia de citire a matricei
void read_matrix(FILE *input, int **matrice, int *directions,
				 int N, int M, int K, int *pos_r, int *pos_c) {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++) {
			fscanf(input, "%d", &matrice[i][j]);
			if (matrice[i][j] == 2) {
				*pos_r = i;  // pastrez pozitia initiala a lui David
				*pos_c = j;
			}
		}
	for (int i = 0; i < K; i++)
		fscanf(input, "%d", &directions[i]);
}

void print(FILE *output, int **matrice, int N, int M) {
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++)
			fprintf(output, "%d ", matrice[i][j]);
			fprintf(output, "\n");
		}
	fprintf(output, "\n");
}
// Urmatoarele 4 functii urmeaza acelasi principiu
// Functiile verifica fiecare directie si pozitioneaza nr
// pe pozitiile accesibile
void up(int **matrice, int pos_r, int pos_c, int *nr) {
	int i = pos_r;
	int j = pos_c;

	for (int k = i-1; k >= 0; k--)
		if (matrice[k][j] != 1)
			matrice[k][j] = *nr;
		else
			break;
}

void down(int **matrice, int N, int pos_r, int pos_c, int *nr) {
	int i = pos_r;
	int j = pos_c;

	for (int k = i+1; k < N; k++)
		if (matrice[k][j] != 1)
			matrice[k][j] = *nr;
		else
			break;
}

void left(int **matrice, int M, int pos_r, int pos_c, int *nr) {
	int i = pos_r;
	int j = pos_c;

	for (int k = j-1; k >= 0; k--)
		if (matrice[i][k] != 1)
			matrice[i][k] = *nr;
		else
			break;
}

void right(int **matrice, int M, int pos_r, int pos_c, int *nr) {
	int i = pos_r;
	int j = pos_c;

	for (int k = j+1; k < M; k++)
		if (matrice[i][k] != 1)
			matrice[i][k] = *nr;
		else
			break;
}

// Functia principala creare a drumurilor
void paths(int **matrice, int *directions, int N, int M, int K,
				int pos_r, int pos_c, int *nr, FILE *output) {
	int k = 0;
	while (k < K) {
		if (k == 0) {
			// prima pozitionare pe prima directie
			if (directions[k] == Up)
				up(matrice, pos_r, pos_c, nr);
			else if (directions[k] == Down)
				down(matrice, N, pos_r, pos_c, nr);
			else if (directions[k] == Left)
				left(matrice, M, pos_r, pos_c, nr);
			else if (directions[k] == Right)
				right(matrice, M, pos_r, pos_c, nr);
		} else {
			// se parcurge matricea si se pozitioneaza
			// pe directia cuvenita nr
			for (int i = 0; i < N; i++) {
				for (int j = 0; j< M; j++) {
					if (matrice[i][j] == *nr - 1) {
						if (directions[k] == Up)
							up(matrice, i, j, nr);
						else if (directions[k] == Down)
							down(matrice, N, i, j, nr);
						else if (directions[k] == Left)
							left(matrice, M, i, j, nr);
						else if (directions[k] == Right)
							right(matrice, M, i, j, nr);
					}
				}
			}
		}
		(*nr)++;
		k++;
	}
}
// Functia numara cate pozitii sunt in matrice egale cu ultimul nr
// In acest punct ultimul nr reprezinta ultima celula din drum
int nr_final_positions(int **matrice, int N, int M, int nr) {
	int cnt = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			if (matrice[i][j] == nr-1)
				cnt++;
	return cnt;
}
int main(int argc, char const *argv[]) {
	FILE *input = fopen("labirint.in", "r");
	FILE *output = fopen("labirint.out", "w");
	int N, M, K, pos_c, pos_r;
	fscanf(input, "%d %d %d", &N, &M, &K);
	// alocarea matricei si a vectorului de directii
	int **matrice = calloc(N+1, sizeof(int*));
	for (int i = 0; i < M; i++)
		matrice[i] = calloc(M, sizeof(int));
	int *directions = calloc(K, sizeof(int));

	read_matrix(input, matrice, directions, N, M, K, &pos_r, &pos_c);
	int nr = 3;  // initializez nr cu un numar diferit de 0, 1, 2

	paths(matrice, directions, N, M, K, pos_r, pos_c, &nr, output);
	int cnt = nr_final_positions(matrice, N, M, nr);
	fprintf(output, "%d", cnt);
	fprintf(output, "\n");
	// eliberarea memoriei
	free(directions);
	for (int i = 0; i <= N; i++)
		free(matrice[i]);
	free(matrice);
	fclose(input);
	fclose(output);

	return 0;
}

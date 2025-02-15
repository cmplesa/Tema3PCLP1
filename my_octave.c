//PlesaMarianCosmin311CA
#include <stdio.h>
#include <stdlib.h>

#define M 10007
void freemaxtrix(int ***matrice, int rows)
{
	//functia elibereaza matricea aleasa in alte functii
	for (int i = 0; i < rows; i++)
		free((*matrice)[i]);

	free((*matrice));
}

void inm(int n, int **A, int **B, int ***result)
{
//se realizeaza inmultirea a 2 matrice si matricea result este matricea rez
for (int i = 0; i < n; i++) {
	for (int j = 0; j < n; j++) {
		(*result)[i][j] = 0;
		for (int k = 0; k < n; k++)
			(*result)[i][j] = ((*result)[i][j] + A[i][k] * B[k][j]) % M;
				if ((*result)[i][j] < 0)
					(*result)[i][j] = (*result)[i][j] + M;
		}
	}
}

void puterelog(int **matrix, int ***result, int exponent, int n)
{
	int **auxiliary1;
	auxiliary1 = (int **)malloc(n * sizeof(int *));
	if (!auxiliary1) {
		printf("Eroare la alocarea de memorie pentru matrice.\n");
		return;
	}
	for (int i = 0; i < n; i++) {
		auxiliary1[i] = (int *)malloc(n * sizeof(int));
		if (!auxiliary1[i]) {
			printf("Eroare la alocarea de memorie pentru linia %d.\n", i);
			for (int j = 0; j < i; j++)
				free(auxiliary1[j]);

			free(auxiliary1);
			return;
		}
	}
	if (exponent == 0) { //daca exp=0 se intoarce matricea identitate
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++)
				if (i == j)
					(*result)[i][j] = 1;
				else
					(*result)[i][j] = 0;
		}
	} else if (exponent % 2 == 0) { //se verifica paritatea exponentului
		puterelog(matrix, &auxiliary1, exponent / 2, n);
		inm(n, auxiliary1, auxiliary1, &(*result));
	} else {
		puterelog(matrix, &auxiliary1, exponent - 1, n);
		inm(n, matrix, auxiliary1, &(*result));
	}
	freemaxtrix(&auxiliary1, n);
}

void down(int s, int **A, int **B, int ***C)
{
	int i, j;
	for (i = 0; i < s; i++) {
		for (j = 0; j < s; j++) {
			(*C)[i][j] = (A[i][j] - B[i][j]) % 10007;
				if ((*C)[i][j] < 0)
					(*C)[i][j] = (*C)[i][j] + M;
		}
	}
}

void resize2(int ***msize, int *cm)
{
	*cm = *cm * 2;
	int **temp = (int **)realloc(*msize, *cm * sizeof(int *));
	if (!temp) {
		// Eroare la realocare de memorie
		printf("Eroare la realocarea de memorie.\n");
		return;
	}
	*msize = temp;
}

void resize(int ****lm, int *cm)
{
	*cm = *cm * 2;
	int ***temp = (int ***)realloc(*lm, *cm * sizeof(int **));
	if (!temp) {
		// Eroare la realocare de memorie
		printf("Eroare la realocarea de memorie pentru lm.\n");
		return;
	}
	*lm = temp;
}

int **alocm(int rows, int cols)
{
	int **matrix = (int **)malloc(rows * sizeof(int *));
	if (!matrix) {
		perror("Eroare la alocarea memoriei pentru matrix");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < rows; i++) {
		matrix[i] = (int *)malloc(cols * sizeof(int));
		if (!matrix[i]) {
			perror("Eroare la alocarea memoriei pentru matrix");
			exit(EXIT_FAILURE);
		}
	}
	return matrix;
}

void sum(int s, int **A, int **B, int ***C)
{
	int i, j;
	for (i = 0; i < s; i++) {
		for (j = 0; j < s; j++) {
			(*C)[i][j] = (A[i][j] + B[i][j]) % 10007;
				if ((*C)[i][j] < 0)
					(*C)[i][j] = (*C)[i][j] + M;
		}
	}
}

void print(int n, int **matrix)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			printf("%d ", matrix[i][j]);

		printf("\n");
	}
}

void strassen(int n, int **matr1, int **matr2, int ***result)
{
	if (n == 1) { //se verifica marimea matricei
		(*result)[0][0] = matr1[0][0] * matr2[0][0];
	} else { //se aplica algoritmul strassen
		n = n / 2;
		int **A11, **A12, **A21, **A22, **B11, **B12, **B21, **B22;
		int **P1, **P2, **P3, **P4, **P5, **P6, **P7, **C11, **C12, **C21;
		int **C22, **temp1, **temp2;
		A11 = alocm(n, n);
		A12 = alocm(n, n);
		A21 = alocm(n, n);
		A22 = alocm(n, n);
		B11 = alocm(n, n);
		B12 = alocm(n, n);
		B21 = alocm(n, n);
		B22 = alocm(n, n);
		P1 = alocm(n, n);
		P2 = alocm(n, n);
		P3 = alocm(n, n);
		P4 = alocm(n, n);
		P5 = alocm(n, n);
		P6 = alocm(n, n);
		P7 = alocm(n, n);
		C11 = alocm(n, n);
		C12 = alocm(n, n);
		C21 = alocm(n, n);
		C22 = alocm(n, n);
		temp1 = alocm(n, n);
		temp2 = alocm(n, n);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				A11[i][j] = matr1[i][j];
				A12[i][j] = matr1[i][j + n];
				A21[i][j] = matr1[i + n][j];
				A22[i][j] = matr1[i + n][j + n];
				B11[i][j] = matr2[i][j];
				B12[i][j] = matr2[i][j + n];
				B21[i][j] = matr2[i + n][j];
				B22[i][j] = matr2[i + n][j + n];
			}
		}
		sum(n, B11, B22, &temp2);
		sum(n, A11, A22, &temp1); strassen(n, temp1, temp2, &P1);
		sum(n, A21, A22, &temp1); strassen(n, temp1, B11, &P2);
		down(n, B12, B22, &temp2); strassen(n, A11, temp2, &P3);
		down(n, B21, B11, &temp2); strassen(n, A22, temp2, &P4);
		sum(n, A11, A12, &temp1); strassen(n, temp1, B22, &P5);
		down(n, A21, A11, &temp1); sum(n, B11, B12, &temp2);
		strassen(n, temp1, temp2, &P6); down(n, A12, A22, &temp1);
		sum(n, B21, B22, &temp2); strassen(n, temp1, temp2, &P7);
		sum(n, P1, P4, &temp1); down(n, P7, P5, &temp2);
		sum(n, temp1, temp2, &C11); sum(n, P3, P5, &C12);
		sum(n, P2, P4, &C21); down(n, P1, P2, &temp1);
		sum(n, P3, P6, &temp2); sum(n, temp1, temp2, &C22);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				(*result)[i][j] = C11[i][j];
				(*result)[i][j + n] = C12[i][j];
				(*result)[i + n][j] = C21[i][j];
				(*result)[i + n][j + n] = C22[i][j];
			}
		}
		freemaxtrix(&A11, n); freemaxtrix(&A12, n);
		freemaxtrix(&A21, n); freemaxtrix(&A22, n);
		freemaxtrix(&B11, n); freemaxtrix(&B12, n);
		freemaxtrix(&B21, n); freemaxtrix(&B22, n);
		freemaxtrix(&P1, n);
		freemaxtrix(&P2, n); freemaxtrix(&P3, n);
		freemaxtrix(&P4, n); freemaxtrix(&P5, n);
		freemaxtrix(&P6, n); freemaxtrix(&P7, n);
		freemaxtrix(&temp1, n); freemaxtrix(&temp2, n);
		freemaxtrix(&C11, n); freemaxtrix(&C12, n);
		freemaxtrix(&C21, n); freemaxtrix(&C22, n);
	}
}

void multiplierstrassen(int ****lm, int ***msize, int *cm, int *mn, int ind1,
						int ind2)
{
	int n1 = (*msize)[ind1][0];
	int m1 = (*msize)[ind1][1];
	int n2 = (*msize)[ind2][0];
	int m2 = (*msize)[ind2][1];

	if (m1 != n2) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}

	int **A = (*lm)[ind1];
	int **B = (*lm)[ind2];

	int **C = alocm(n1, m2); //matricea rezultat este C

	strassen(n1, A, B, &C);

	resize(lm, cm);
	(*lm)[*mn] = C;//se salveaza matricea rezultat in lista de matrice

	(*msize) = (int **)realloc(*msize, (*mn + 1) * sizeof(int *));
	(*msize)[*mn] = (int *)malloc(2 * sizeof(int));
	(*msize)[*mn][0] = n1;
	(*msize)[*mn][1] = m2;

	(*mn)++;
}

int smatr(int **matrice, int l, int c)
{
	int s = 0;
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < c; j++)
			s = (s + matrice[i][j]) % M;
	}
	s = s % M;
	if (s < 0)
		s += M;

	return s;
}

void sortare(int ***lm, int **msize, int mn)
{
	int aux, **temp;
	for (int i = 0; i < mn - 1; i++) {
		for (int j = i + 1; j < mn; j++) { //se compara sumele matr
			if (smatr(lm[i], msize[i][0], msize[i][1]) >
					smatr(lm[j], msize[j][0], msize[j][1])) {
				temp = lm[i];
				lm[i] = lm[j];
				lm[j] = temp;

				aux = msize[i][0];
				msize[i][0] = msize[j][0];
				msize[j][0] = aux;

				aux = msize[i][1];
				msize[i][1] = msize[j][1];
				msize[j][1] = aux;
			}
		}
	}
}

void deal(int ****lm, int ***msize, int *mn)
{
	for (int i = 0; i < *mn; i++) {
		for (int j = 0; j < (*msize)[i][0]; j++)
			free((*lm)[i][j]);
				for (int i = 0; i < *mn; i++)
					free((*lm)[i]);
	}
	free(*lm);

	for (int i = 0; i < *mn; i++)
		free((*msize)[i]);

	free(*msize);
	*mn = 0;
}

void elm(int ****lm, int ***msize, int ind5, int *mn)
{
	for (int i = 0; i < (*msize)[ind5][0]; i++)
		free((*lm)[ind5][i]);

	free((*lm)[ind5]);
	free((*msize)[ind5]);

	for (int j = ind5; j < *mn - 1; j++) {
		(*lm)[j] = (*lm)[j + 1];
		(*msize)[j] = (*msize)[j + 1];
	}
	(*mn)--;
}

void transposition(int ****lm, int ***msize, int *mn, int ind4)
{
	if (ind4 >= *mn)
		printf("No matrix with the given index");

	int **trp = alocm((*msize)[ind4][1], (*msize)[ind4][0]);
	for (int i = 0; i < (*msize)[ind4][1]; i++)
		for (int j = 0; j < (*msize)[ind4][0]; j++)
			trp[i][j] = (*lm)[ind4][j][i];

	for (int z = 0; z < (*msize)[ind4][0]; z++)
		free((*lm)[ind4][z]);

	free((*lm)[ind4]);
	(*lm)[ind4] = trp;
	int aux = (*msize)[ind4][0];
	(*msize)[ind4][0] = (*msize)[ind4][1];
	(*msize)[ind4][1] = aux;
}

void rdm(int ****lm, int ***msize, int *mn, int ind3)
{
	int l, r;
	scanf("%d", &l);
	int *ldx = (int *)malloc(l * sizeof(int));// vector pentru linii
	if (!ldx) {
		printf("Eroare la alocarea de memorie pentru ldx.\n");
		return;
	}
	for (int s = 0; s < l; s++)
		scanf("%d", &ldx[s]);

	scanf("%d", &r);
	int *cdx = (int *)malloc(r * sizeof(int)); //vector pentru coloane
	if (!cdx) {
		printf("Eroare la alocarea de memorie pentru ldx.\n");
		return;
	}
	for (int z = 0; z < r; z++)
		scanf("%d", &cdx[z]);

	if (ind3 >= *mn || ind3 < 0) {
		printf("No matrix with the given index\n");
		free(ldx);
		free(cdx);
		return;
	}

	int **rdmatr = alocm(l, r); //alocare matrice cu noi dimensiuni

	for (int m = 0; m < l; m++) {
		for (int y = 0; y < r; y++)
			if (m < (*msize)[ind3][0] && y < (*msize)[ind3][1])
				rdmatr[m][y] = (*lm)[ind3][ldx[m]][cdx[y]];
			else
				rdmatr[m][y] = 0;
	}
	for (int i = 0; i < (*msize)[ind3][0]; i++)
		free((*lm)[ind3][i]);

	free((*lm)[ind3]);

	(*lm)[ind3] = rdmatr;
	(*msize)[ind3][0] = l;
	(*msize)[ind3][1] = r;
	free(ldx);
	free(cdx);
}

void caseM(int ****lm, int ***msize, int *cm, int *mn)
{
	int i1, i2;
	scanf("%d%d", &i1, &i2);

	if (i1 >= *mn || i2 >= *mn) {
		printf("No matrix with the given index\n");
		return;
	}

	if ((*msize)[i1][1] != (*msize)[i2][0]) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}
	//,atrice rezultat pentru inmultire
	int **mr = alocm((*msize)[i1][0], (*msize)[i2][1]);

	for (int i = 0; i < (*msize)[i1][0]; i++) {
		for (int j = 0; j < (*msize)[i2][1]; j++) {
			mr[i][j] = 0;
			for (int y = 0; y < (*msize)[i1][1]; y++)
				mr[i][j] = (mr[i][j] + (*lm)[i1][i][y] * (*lm)[i2][y][j]) % M;
					if (mr[i][j] < 0)
						mr[i][j] = mr[i][j] + M;
		}
	}

	resize(lm, cm);
	(*lm)[*mn] = mr;

	(*msize) = (int **)realloc(*msize, (*mn + 1) * sizeof(int *));
	(*msize)[*mn] = (int *)malloc(2 * sizeof(int));
	(*msize)[*mn][0] = (*msize)[i1][0];
	(*msize)[*mn][1] = (*msize)[i2][1];

	(*mn)++;
}

void readmatrix(int ****lm, int ***msize, int *cm, int *mn)
{
	int n, m;
	scanf("%d%d", &n, &m);
	if (*cm == *mn) {
		resize(lm, cm);
		*cm = *cm / 2;
		resize2(msize, cm);
	}
	int **a = (int **)malloc(n * sizeof(int *));
	if (!a) { // daca nu pot aloca, opresc executia si afisez un mesaj
		fprintf(stderr, "malloc() for matrix failed\n");
		*lm = NULL;
		return;
	}

	(*lm)[*mn] = a;
	(*msize)[*mn] = (int *)malloc(2 * sizeof(int));
	(*msize)[*mn][0] = n;
	(*msize)[*mn][1] = m;
	for (int i = 0; i < n; i++) {
		a[i] = (int *)malloc(m * sizeof(int));
		if (!a[i]) {
			fprintf(stderr, "malloc() for line %d failed\n", i);
			for (int j = 0; j < i; j++)
				free(a[j]);
			free(a);
			*lm = NULL;
			return;
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
			scanf("%d", &a[i][j]);
	}
	(*mn)++;
}

void printmatrix(int ***lm, int **msize, int ind)
{
	for (int i = 0; i < msize[ind][0]; ++i) {
		for (int j = 0; j < msize[ind][1]; ++j)
			printf("%d ", lm[ind][i][j]);

		printf("\n");
	}
}

int ***allocatememoryfor3darray(int cm)
{
	int ***lm = (int ***)malloc(cm * sizeof(int **));
	if (!lm)  {
		printf("Eroare la alocarea de memorie pentru lm.\n");
		return NULL;
	}

	for (int i = 0; i < cm; i++)
		lm[i] = NULL;

	return lm;
}

int **allocatememoryfor2darray(int cm)
{
	int **msize = (int **)malloc(cm * sizeof(int *));
	if (!msize) {
		printf("Eroare la alocarea de memorie pentru msize.\n");
		return NULL;
	}

	for (int i = 0; i < cm; i++)
		msize[i] = NULL;

	return msize;
}

int main(void)
{	//lm=lista de matrice si msize=matricea de dimensiuni
	int ***lm = NULL, **msize = NULL; //cm=capacitare memorie si mn=matrice nr
	int mn = 0, cm = 1, ok = 1, ind, ind2, ind3, ind4, ind5, ind6, ind7;
	int exponent, ind8; char s;
	lm = allocatememoryfor3darray(cm);
	msize = allocatememoryfor2darray(cm);
	while (ok == 1) {
		scanf(" %c", &s);
			switch (s) {
			case 'L':
				readmatrix(&lm, &msize, &cm, &mn); break;
			case 'D':
				scanf("%d", &ind2); //ind2=indicele matricei alese
				if (ind2 >= mn)
					printf("No matrix with the given index\n");
				else
					printf("%d %d\n", msize[ind2][0], msize[ind2][1]);
				break;
			case 'M':
				caseM(&lm, &msize, &cm, &mn); break;
			case 'P':
				scanf("%d", &ind); //ind=indicele matricei alese
				if (ind >= mn || ind < 0)
					printf("No matrix with the given index\n");
				else
					printmatrix(lm, msize, ind);
				break;
			case 'C':
				scanf("%d", &ind3); rdm(&lm, &msize, &mn, ind3); break;
			case 'T':
				scanf("%d", &ind4);
				if (ind4 >= mn)
					printf("No matrix with the given index\n");
				else
					transposition(&lm, &msize, &mn, ind4);
				break;
			case 'F':
				scanf("%d", &ind5);
				if (ind5 >= mn || ind5 < 0)
					printf("No matrix with the given index\n");
				else
					elm(&lm, &msize, ind5, &mn);
				break;
			case 'S':
				scanf("%d%d", &ind6, &ind7);
				multiplierstrassen(&lm, &msize, &cm, &mn, ind6, ind7); break;
			case 'R':
				scanf("%d%d", &ind8, &exponent);
					if (ind8 >= mn || ind8 < 0) {
						printf("No matrix with the given index\n");
					} else if (exponent < 0) {
						printf("Power should be positive\n");
					} else if (msize[ind8][0] != msize[ind8][1]) {
						printf("Cannot perform matrix multiplication\n");
				} else {
					int y = msize[ind8][0];
					int **result;
					result = (int **)malloc(y * sizeof(int *));
					for (int i = 0; i < y; i++)
						result[i] = (int *)malloc(y * sizeof(int));
					puterelog(lm[ind8], &result, exponent, y);
					freemaxtrix(&lm[ind8], y);
					lm[ind8] = result; msize[ind8][0] = y; msize[ind8][1] = y;
					}
				break;
			case 'O':
				sortare(lm, msize, mn); break;
			case 'Q':
				for (int i = 0; i < mn; i++)
					freemaxtrix(&lm[i], msize[i][0]);
				free(lm);
				freemaxtrix(&msize, mn); ok = 0; break;
			default:
				printf("Unrecognized command\n"); break;
		}
	}
	return 0;
}

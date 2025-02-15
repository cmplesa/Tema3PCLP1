//PlesaMarianCosmin311CA
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int r, g, b;
} rgb;

typedef struct {
	int type;
	int height, width;
	int valmax;
	int loaded;
	int **gpix;
	int coord[4];
	rgb **cpix;
} image;

int clampul_meu(double x)
{
	if (x > 255)
		x = 255;
	else if (x < 0)
		x = 0;
	return (int)x;
}

void freematrix(int **image, int n)
{
	for (int i = 0; i < n; i++)
		free(image[i]);

	free(image);
}

void freeimgcolor(rgb **v, int n)
{
	for (int i = 0; i < n; i++)
		free(v[i]);
	free(v);
}

rgb **allocatematrixcolor(int lines, int columns, image *image)
{
	image->type = 1;
	rgb **matrice;
	matrice = (rgb **)malloc(lines * sizeof(rgb *));
	if (!matrice) {
		if (image->type == 0)
			freematrix((*image).gpix,  image->height);
		else if (image->type == 1)
			freeimgcolor(image->cpix, image->height);
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < lines; i++) {
		matrice[i] = (rgb *)malloc(columns * sizeof(rgb));
		if (!matrice[i]) {
			for (int j = 0; j <= i; j = j + 1)
				free(matrice[j]);
			if (image->type == 0)
				freematrix((*image).gpix,  image->height);
			else if (image->type == 1)
				freeimgcolor((*image).cpix, image->height);
			exit(EXIT_FAILURE);
		}
	}
	return matrice;
}

int **allocarematrix(int nheight, int nwidth, image *img)
{
	int **image;
	image = (int **)malloc(nheight * sizeof(int *));
	if (!image) { // daca nu pot aloca, opresc executia si afisez un mesaj
		fprintf(stderr, "malloc() for matrix failed\n");
		if (img->type == 0)
			freematrix((*img).gpix,  img->height);
		else if (img->type == 1)
			freeimgcolor(img->cpix, img->height);
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < nheight; i++) {
		image[i] = (int *)malloc(nwidth * sizeof(int));
			if (!image[i]) { // daca nu pot aloca, opresc executia si afisez
				fprintf(stderr, "malloc() for matrix failed\n");
				exit(EXIT_FAILURE);
				free(img);
				exit(EXIT_FAILURE);
		}
	}
	return image;
}

double **allocatematrixdo(int height, int width)
{
	double **kernel;
	kernel = (double **)malloc(height * sizeof(double *));
	if (!kernel) { // daca nu pot aloca, opresc executia si afisez un mesaj
		fprintf(stderr, "malloc() for matrix failed\n");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < height; i++) {
		kernel[i] = (double *)malloc(width * sizeof(double));
			if (!kernel[i]) { // daca nu pot aloca, opresc executia si afisez
				fprintf(stderr, "malloc() for matrix failed\n");
				exit(EXIT_FAILURE);
			}
	}
	return kernel;
}

int verifypower2(int a)
{
	while (a % 2 == 0)
		a = a / 2;

	return a;
}

void eliminare_comentarii(FILE *file)
{
	int c;
	while ((c = fgetc(file)) == '#')
		while ((c = fgetc(file)) != '\n' && c != EOF)
			;
	ungetc(c, file);
}

void loadgrey(image *img, char filename[])
{
	char tip[3];
	int latime2, inaltime2, max_v;
	int c, s, f, z;
	FILE *in = fopen(filename, "rt");//deschidere fisier
	if (!in) {
		printf("Failed to load %s\n", filename);
		free(img);
		return;
	}
	while ((c = fgetc(in)) == '#')
		while ((c = fgetc(in)) != '\n' && c != EOF)
			;
	ungetc(c, in);
	fscanf(in, "%s", tip);
	while ((s = fgetc(in)) == '#')
		while ((s = fgetc(in)) != '\n' && c != EOF)
			;
	ungetc(s, in);
	fscanf(in, "%d%d", &latime2, &inaltime2);
	while ((f = fgetc(in)) == '#')
		while ((f = fgetc(in)) != '\n' && c != EOF)
			;
	ungetc(f, in);
	fscanf(in, "%d", &max_v);
	while ((z = fgetc(in)) == '#')
		while ((z = fgetc(in)) != '\n' && c != EOF)
			;
	ungetc(z, in);
	img->width = latime2;
	img->height = inaltime2;
	img->valmax = max_v;
	//salvez in vectorul de coordonate dimensiunile pozei
	img->coord[0] = 0; //x1=0;
	img->coord[1] = 0; // y1=0;
	 //ambele coordonate x1 si y1 se initializeaza cu 0
	img->coord[3] = img->height; // y2=nr de linii
	img->coord[2] = img->width;  // x2=nr de coloane
	//se aloca matricea cu dimensiunile citite
	(*img).gpix = allocarematrix(inaltime2, latime2, img);
	if (strcmp(tip, "P2") == 0) {
		for (int i = 0; i < inaltime2; i++)
			for (int j = 0; j < latime2; j++)
				fscanf(in, "%d", &(*img).gpix[i][j]);
	} else if (strcmp(tip, "P5") == 0) {
		int aux = ftell(in);//se retine pozitia in fisier
		fclose(in);
		in = fopen(filename, "rb");
		// se deschide ca fisier binar pt citire
		fseek(in, aux + 1, SEEK_SET); // se pozitioneaza in fisier unde a ramas
		for (int i = 0; i < inaltime2; i++)
			for (int j = 0; j < latime2; j++) {
				// se citesc valorile pixelilor pentru imaginea
				// in tonuri gri
				unsigned char a;
				fread(&a, sizeof(unsigned char), 1, in);
				(*img).gpix[i][j] = (int)a;
				}
		}
	img->loaded = 1;
	img->type = 0;  //color
	fclose(in);
}

void loadcolor(image *img, char filename[])
{
	char tip[3];
	int latime2, inaltime2, max_v;
	int c, s, f, z;
	FILE *in = fopen(filename, "rt");//deschidere fisier
	if (!in) {
		printf("Failed to load %s\n", filename);
		free(img);
		return;
	}
	while ((c = fgetc(in)) == '#')
		while ((c = fgetc(in)) != '\n' && c != EOF)
			;
	ungetc(c, in);
	fscanf(in, "%s", tip);
	while ((s = fgetc(in)) == '#')
		while ((s = fgetc(in)) != '\n' && c != EOF)
			;
	ungetc(s, in);
	fscanf(in, "%d%d", &latime2, &inaltime2);
	while ((f = fgetc(in)) == '#')
		while ((f = fgetc(in)) != '\n' && c != EOF)
			;
	ungetc(f, in);
	fscanf(in, "%d", &max_v);
	while ((z = fgetc(in)) == '#')
		while ((z = fgetc(in)) != '\n' && c != EOF)
			;
	ungetc(z, in);
	img->width = latime2;
	img->height = inaltime2;
	img->valmax = max_v;
	img->loaded = 1;
	img->coord[0] = 0;
	img->coord[2] = img->width;
	img->coord[1] = 0;
	img->coord[3] = img->height;
	img->type = 1;  //color
	img->cpix = allocatematrixcolor(inaltime2, latime2, img);
	if (strcmp(tip, "P3") == 0) {
		for (int i = 0; i < img->height; i++)
			for (int j = 0; j < img->width; j++) {
				// se citesc valorile pixelilor pentru imaginea
				 // color
				int r;
				int g;
				int b;
				fscanf(in, "%d", &r);
				(*img).cpix[i][j].r = r;
				fscanf(in, "%d", &g);
				(*img).cpix[i][j].g = g;
				fscanf(in, "%d", &b);
				(*img).cpix[i][j].b = b;
			}
	} else if (strcmp(tip, "P6") == 0) {
		int aux = ftell(in);//se retine pozitia in fisier
		fclose(in);
		in = fopen(filename, "rb");//se deschide ca fisier binar pt citire
		fseek(in, aux + 1, SEEK_SET); // se pozitioneaza in fisier unde a ramas
		//se citesc valorile pixelilor pentru imaginea color
		unsigned char r;
		unsigned char g;
		unsigned char b;
		for (int i = 0; i < img->height; i++)
			for (int j = 0; j < img->width; j++) {
				fread(&r, sizeof(unsigned char), 1, in);
				(*img).cpix[i][j].r = (int)r;
				fread(&g, sizeof(unsigned char), 1, in);
				(*img).cpix[i][j].g = (int)g;
				fread(&b, sizeof(unsigned char), 1, in);
				(*img).cpix[i][j].b = (int)b;
			}
		}
	fclose(in);
}

void load(image *img, char filename[])
{
	if (img->loaded == 1) {//daca exista vreo imagine incarcata se sterge
		if (img->type == 0)
			freematrix((*img).gpix,  img->height);
		else if (img->type == 1)
			freeimgcolor(img->cpix, img->height);
	img->height = 0;
	img->width = 0;
	img->loaded = 0;
		for (int i = 0; i < 4; i++)
			img->coord[i] = 0;
	}
	FILE *in = fopen(filename, "rt");//deschidere fisier
	if (!in) {
		printf("Failed to load %s\n", filename);
		return;
	}

	char tip[3];
	eliminare_comentarii(in);
	fscanf(in, "%s", tip);
	if (strcmp(tip, "P2") == 0 || strcmp(tip, "P5") == 0) {//tonuri gri
		loadgrey(img, filename);
	} else if (strcmp(tip, "P3") == 0 || strcmp(tip, "P6") == 0) {//color
		loadcolor(img, filename);
	}
	fclose(in);
	printf("Loaded %s\n", filename);
}

void selectall(image *img)
{
	if (!img->loaded) {
		printf("No image loaded\n");
		return;
	}

	img->coord[0] = 0;
	img->coord[2] = img->width;
	img->coord[1] = 0;
	img->coord[3] = img->height;

	printf("Selected ALL\n");
}

void save(image *img, char s[])
{
	char cuvant3[150];
	char nume_fisier[100];
	char scpy[100];
	int rezultat = sscanf(s, "%s %s %s", scpy, nume_fisier, cuvant3);
	int ok = 0;//ok ne spune cum sa salvam fisierul 0=binar, 1=text
	if (rezultat == 2)
		ok = 0;
	else if (rezultat == 3)
		ok = 1;
	if (img->loaded == 0) {
		printf("No image loaded\n");
		return;
	}

	if (img->loaded == 1 && ok == 1) {
		FILE *fptr = fopen(nume_fisier, "w");
		if (img->type == 0) {
			fprintf(fptr, "P2\n");
			fprintf(fptr, "%d %d\n", img->width, img->height);
			fprintf(fptr, "%d\n", img->valmax);
				for (int i = 0; i < img->height; i++) {
					for (int j = 0; j < img->width; j++)
						fprintf(fptr, "%d ", (*img).gpix[i][j]);

		fprintf(fptr, "\n");
				}
	} else if (img->type == 1) {
		fprintf(fptr, "P3\n");
		fprintf(fptr, "%d %d\n", img->width, img->height);
		fprintf(fptr, "%d\n", img->valmax);
		for (int i = 0; i < img->height; i++) {
			for (int j = 0; j < img->width; j++) {
				fprintf(fptr, "%d ", (*img).cpix[i][j].r);
				fprintf(fptr, "%d ", (*img).cpix[i][j].g);
				fprintf(fptr, "%d ", (*img).cpix[i][j].b);
		}
		fprintf(fptr, "\n");
		}
		}
	fclose(fptr);
	} else if (img->loaded == 1 && ok == 0) {
		if (img->type == 0) {
			FILE *fptr = fopen(nume_fisier, "wb");
			fprintf(fptr, "%s\n", "P5");
			fprintf(fptr, "%d %d\n", img->width, img->height);
			fprintf(fptr, "%d\n", img->valmax);
			for (int i = 0; i < img->height; i++)
				for (int j = 0; j < img->width; j++) {
					unsigned char a;
					a = (unsigned char)(*img).gpix[i][j];
					fwrite(&a, sizeof(unsigned char), 1, fptr);
				}
		fclose(fptr);
		} else if (img->type == 1) {
			FILE *fptr = fopen(nume_fisier, "wb");
			fprintf(fptr, "P6\n");
			fprintf(fptr, "%d %d\n", img->width, img->height);
			fprintf(fptr, "%d\n", img->valmax);

	// unsigned char r, g, b;
	for (int i = 0; i < img->height; i++) {
		for (int j = 0; j < img->width; j++) {
			unsigned char r, g, b;
			r = (unsigned char)(*img).cpix[i][j].r;
			fwrite(&r, sizeof(unsigned char), 1, fptr);
			g = (unsigned char)(*img).cpix[i][j].g;
			fwrite(&g, sizeof(unsigned char), 1, fptr);
			b = (unsigned char)(*img).cpix[i][j].b;
			fwrite(&b, sizeof(unsigned char), 1, fptr);
		}
		}
	fclose(fptr);
	}
	}
	printf("Saved %s\n", nume_fisier);
}

void interschimbare(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void selectioncoordinates(image *img, int x1, int y1, int x2, int y2)
{
	if (!img->loaded) {
		printf("No image loaded\n");
		return;
	}

	if (y1 > y2)
		interschimbare(&y1, &y2);

	if (x1 > x2)
		interschimbare(&x1, &x2);

	if (x1 < 0 || x1 >= img->width ||
		x2 <= 0 || x2 > img->width ||
		y1 < 0 || y1 >= img->height ||
		y2 <= 0 || y2 > img->height ||
		x1 == x2 || y1 == y2) {
		printf("Invalid set of coordinates\n");
		return;
	}

	printf("Selected %d %d %d %d\n", x1, y1, x2, y2);

	img->coord[0] = x1, img->coord[1] = y1, img->coord[2] = x2,
	img->coord[3] = y2;
}

void gracefulexit(image *image)
{
	if (image->loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	if (image->loaded == 1) {
		if (image->type == 0)
			freematrix((*image).gpix,  image->height);
		else if (image->type == 1)
			freeimgcolor((*image).cpix, image->height);

	image->height = 0;
	image->width = 0;
	image->loaded = 0;
		for (int i = 0; i < 4; i++)
			image->coord[i] = 0;
	}
}

void equalizer(int frecventa[], int nr, int nr_bin)
{
	int x = nr / nr_bin;

	int frecventa2[256] = {0};  // Initialize the array to 0
	// No need for dynamic allocation

	int nr2 = 0;
	for (int i = 0; i < nr; i += x) {
		int sum = 0;
		for (int j = i; j < i + x; j++)
			sum += frecventa[j];
		frecventa2[nr2++] = sum;
	}

	for (int i = 0; i < nr_bin; i++)
		frecventa[i] = frecventa2[i];
}

void histogram(image *image, int x, int y, int z,
			   char s[])
{
	if (image->loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	int result = sscanf(s, "%*s %d %d %d", &x, &y, &z);
	if (result != 2) {
		printf("Invalid command\n");
		return;
	}
	int tip = 0;
	tip = image->type;
	if (tip == 1) {
		printf("Black and white image needed\n");
		return;
	}
	if (result != 2) {
		printf("Invalid command\n");
		return;
	}

	if (image->loaded == 1) {
		if (y > 256 || verifypower2(y) != 1) {
			printf("Invalid set of parameters\n");
			return;
		}
		int histogram[256] = {0};
		for (int i = 0; i < image->height; i++) {
			for (int j = 0; j < image->width; j++)
				histogram[(*image).gpix[i][j]]++;
		}

		equalizer(histogram, 256, y);//se aduce la numarul de binuri
		int maxi = 0;
		for (int i = 0; i < 256; i++) {
			if (histogram[i] > maxi)
				maxi = histogram[i];
		}

		for (int i = 0; i < y; i++) {
			int nr_stelute = (double)histogram[i] / maxi * x;
			printf("%d	|	", nr_stelute);
			for (int j = 0; j < nr_stelute; j++)
				printf("*");

			printf("\n");
		}
	}
}

void crop(image *image)
{
	if (image->loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	int x1 = image->coord[2];
	int x2 = image->coord[0];
	int nwidth = x1 - x2;
	int y1 = image->coord[3];
	int y2 = image->coord[1];
	int nheight = y1 - y2;
if (image->type == 1) {
	rgb **ncpixels = allocatematrixcolor(nheight, nwidth, image);
		for (int i = y2; i < nheight + y2; i++) {
			for (int j = x2; j < nwidth + x2; j++) {
				ncpixels[i - y2][j - x2].r = (*image).cpix[i][j].r;
				ncpixels[i - y2][j - x2].g = (*image).cpix[i][j].g;
				ncpixels[i - y2][j - x2].b = (*image).cpix[i][j].b;
	}
	}
	freeimgcolor(image->cpix, image->height);
	//se aloca o noua matrice cu noile dimensiuni
	image->cpix = allocatematrixcolor(nheight, nwidth, image);
	// Copiază valorile pixelilor din matricea 'ncpixels' în matricea 'image'
	for (int i = 0; i < nheight; i++) {
		for (int j = 0; j < nwidth; j++) {
			// Copiază componenta roșie
			(*image).cpix[i][j].r = ncpixels[i][j].r;
			// Copiază componenta verde
			(*image).cpix[i][j].g = ncpixels[i][j].g;
			// Copiază componenta albastră
			(*image).cpix[i][j].b = ncpixels[i][j].b;
		}
	}
	freeimgcolor(ncpixels, nheight);
	printf("Image cropped\n");
		} else if (image->type == 0) {
			int **newgpix = allocarematrix(nheight, nwidth, image);
				for (int i = y2; i < nheight + y2; i++) {
					for (int j = x2; j < nwidth + x2; j++) {
						newgpix[i - y2][j - x2] =
							(*image).gpix[i][j];
			}
		}
	for (int i = 0; i < nheight; i++)
		free((*image).gpix[i]);

	free((*image).gpix);
	(*image).gpix = allocarematrix(nheight, nwidth, image);
	for (int i = 0; i < nheight; i++) {
		for (int j = 0; j < nwidth; j++)
			(*image).gpix[i][j] = newgpix[i][j];
	}
	for (int i = 0; i < nheight; i++)
		free(newgpix[i]);

	free(newgpix);
	printf("Image cropped\n");
	}
	image->width = nwidth;
	image->height = nheight;
	image->coord[0] = 0;
	image->coord[1] = 0;
	image->coord[2] = image->width;
	image->coord[3] = image->height;
}

void equalize(image *image)
{
	if (image->loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	if (image->type == 1) {
		printf("Black and white image needed\n");
		return;
	}
	if (image->type == 0) {
		int histogram[256] = {0};
		int cumulativehistogram[256] = {0};
		for (int i = 0; i < image->height; i++)
			for (int j = 0; j < image->width; j++)
				histogram[(*image).gpix[i][j]]++;
		int i = 1;
		for (i = 1; i < 256; i++) {
			cumulativehistogram[i] =
				cumulativehistogram[i - 1] + histogram[i];
		}
		for (int i = 0; i < image->height; i++) {
			for (int j = 0; j < image->width; j++) {
				(*image).gpix[i][j] =
					(cumulativehistogram[(*image).gpix[i][j]] * 255) /
					(image->width * image->height);
				(*image).gpix[i][j] = clampul_meu((*image).gpix[i][j]);
		}
	}
	}
	printf("Equalize done\n");
}

void setgaussianblurkernel(double **kernel)
{
		double values[] = {0.0625, 0.125,  0.0625, 0.125, 0.25,
						0.125,  0.0625, 0.125,  0.0625};
	int index = 0;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			kernel[i][j] = values[index++];
}

void setblurkernel(double **kernel)
{
	double value = 0.11111111111;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			kernel[i][j] = value;
}

void setsharpenkernel(double **kernel)
{
	int values[] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			kernel[i][j] = values[i * 3 + j];
}

void setedgekernel(double **kernel)
{
	int values[] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			kernel[i][j] = values[i * 3 + j];
}

void initializekernel(double **kernel, char filter[])
{
	if (strcmp(filter, "GAUSSIAN_BLUR") == 0)
		setgaussianblurkernel(kernel);
	else if (strcmp(filter, "BLUR") == 0)
		setblurkernel(kernel);
	else if (strcmp(filter, "SHARPEN") == 0)
		setsharpenkernel(kernel);
	else if (strcmp(filter, "EDGE") == 0)
		setedgekernel(kernel);
}

void copycpix(rgb **aux, image *img)
{
	for (int i = img->height - 1; i >= 0; i--)
		for (int j = img->width - 1; j >= 0; j--) {
			aux[i][j].r = (*img).cpix[i][j].r;
			aux[i][j].g = (*img).cpix[i][j].g;
			aux[i][j].b = (*img).cpix[i][j].b;
		}
}

void copycpixback(rgb **aux, image *img)
{
				for (int i = 0; i < img->coord[3] - img->coord[1]; i++)
					for (int j = 0; j < img->coord[2] - img->coord[0]; j++) {
						(*img).cpix[i + img->coord[1]][j + img->coord[0]].r =
							aux[i + img->coord[1]][j + img->coord[0]].r;
						(*img).cpix[i + img->coord[1]][j + img->coord[0]].g =
							aux[i + img->coord[1]][j + img->coord[0]].g;
						(*img).cpix[i + img->coord[1]][j + img->coord[0]].b =
							aux[i + img->coord[1]][j + img->coord[0]].b;
		}
}

void freekernel(double **kernel)
{
	for (int i = 0; i < 3; i++)
		free(kernel[i]);
	free(kernel);
}

void applyeffect(image *img, char filter[])
{
	rgb **aux;//se realizeaza o copie
	aux = allocatematrixcolor(img->height, img->width, img);
	copycpix(aux, img);
	double **mat_filtru;
	mat_filtru = allocatematrixdo(3, 3);
	initializekernel(mat_filtru, filter);// se initializeaza matricea
	copycpix(aux, img);
	for (int i = img->coord[1]; i < img->coord[3]; i++) {
		for (int j = img->coord[0]; j < img->coord[2]; j++) {
			double aux_r = 0, aux_b = 0, aux_g = 0;
			if (i >= 1 && i <= img->height - 2 && j >= 1 &&
				j <= img->width - 2) {
				for (int k = 0; k <= 2; k++) {
					for (int l = 0; l <= 2; l++) {
						int nr_l = l - 1, nr_c = k - 1;
										aux_r +=
											(*img).cpix[i + nr_c][j + nr_l].r *
											mat_filtru[l][k];

										aux_g +=
											(*img).cpix[i + nr_c][j + nr_l].g *
											mat_filtru[l][k];

										aux_b +=
											(*img).cpix[i + nr_c][j + nr_l].b *
											mat_filtru[l][k];
				}
			}
			aux[i][j].r = clampul_meu(round(aux_r));
			aux[i][j].g = clampul_meu(round(aux_g));
			aux[i][j].b = clampul_meu(round(aux_b));
		}
	}
	}
	//se retin valorile in matricea img->color
	copycpixback(aux, img);
	freeimgcolor(aux, img->height); //se elibereaza matricea auxiliara
	freekernel(mat_filtru); //se elibereaza kernelul de efect
	printf("APPLY %s done\n", filter);
}

void apply(image *image, char c2[])
{
	if (strcmp(c2, "EDGE") != 0 && strcmp(c2, "SHARPEN") != 0 &&
		strcmp(c2, "BLUR") != 0 && strcmp(c2, "GAUSSIAN_BLUR") != 0) {
		printf("APPLY parameter invalid\n");
		return;
	}

	if (image->type == 0) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}

	if (image->loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	applyeffect(image, c2);
}

int onlynumbers(const char *str)
{
	if (*str == '-' && *(str + 1) != '\0') {
		// Avem un minus la început, verificăm restul șirului
		str++;
	}

	while (*str) {
		if (*str < '0' || *str > '9')
			return 0;  // Nu conține doar cifre

	str++;
	}

	return 1;  // Conține doar cifre sau un singur minus la început
}

int main(void)
{
	char c1[150], c2[150], c3[150], c4[150], c5[150];
	char s[300];
	int activ = 1;
	image images;
	images.loaded = 0;
	images.type = -1;
	while (activ) {
		fgets(s, sizeof(s), stdin);
		char scpy[150];
		strcpy(scpy, s);
		int rezultat = sscanf(s, "%s %s %s %s %s", c1, c2, c3, c4, c5);
	if (strcmp(c1, "LOAD") == 0) {
		load(&images, c2);
		} else if (strcmp(c1, "SELECT") == 0) {
			if (rezultat == 2) {
				selectall(&images);
			} else {
				int x1, y1, x2, y2;
				x1 = atoi(c2);
				y1 = atoi(c3);
				x2 = atoi(c4);
				y2 = atoi(c5);
				int y = onlynumbers(c5);
				int a = onlynumbers(c4);
				int z = onlynumbers(c3);
				int b = onlynumbers(c2);
					if (images.loaded == 0)
						printf("No image loaded\n");
					else if (y == 0 || z == 0 || a == 0 || b == 0)
						printf("Invalid command\n");
					else if (y == 1 && z == 1 && b == 1 && a == 1)
						selectioncoordinates(&images, x1, y1, x2, y2);
			}
			} else if (strcmp(c1, "HISTOGRAM") == 0) {
				int y = 0;
				int x = 0;
				int z = 0;
				histogram(&images, x, y, z, s);
			} else if (strcmp(c1, "ROTATE") == 0) {
			} else if (strcmp(c1, "CROP") == 0) {
				crop(&images);
			} else if (strcmp(c1, "APPLY") == 0) {
				if (images.loaded == 0)
					printf("No image loaded\n");
				else if (rezultat == 1)
					printf("Invalid command\n");
				else
					apply(&images, c2);
		} else if (strcmp(c1, "SAVE") == 0) {
			save(&images, scpy);
		} else if (strcmp(c1, "EXIT") == 0) {
			activ = 0;
			gracefulexit(&images);
		} else if (strcmp(c1, "EQUALIZE") == 0) {
			equalize(&images);
		} else {
			printf("Invalid command\n");
	}
	}
	return 0;
}

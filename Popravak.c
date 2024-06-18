#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Header.h"

// Funkcija koja provjerava postoji li vec uneseni ID popravka
int postojiIDPopravka(int id) {
	FILE* file = fopen("popravci.txt", "r+");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za popravke");
		exit(1);
	}

	int postojeciId;
	int idVozila;
	int idKlijenta;
	char opis[DESCRIBE];
	double cijena;
	char datum[DATE];
	char dijelovi[PARTS];

	while (fscanf(file, "%d %d %d %254[^0-9] %lf %10s %254[^\n]", &postojeciId, &idVozila, &idKlijenta, opis, &cijena, datum, dijelovi) == 7) {
		if (postojeciId == id) {
			fclose(file);
			return 1;  // ID popravka postoji
		}
	}

	fclose(file);
	return 0;  // ID popravka ne postoji
}

void noviPopravak() {

	FILE* file = fopen("popravci.txt", "a");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za popravke");
		exit(1);
	}

	Popravak* popravak = (Popravak*)malloc(sizeof(Popravak));
	if (popravak == NULL) {
		perror("Neuspjesno zauzimanje memorije za popravak");
		fclose(file);
		exit(1);
	}

	printf("Unesite ID popravka: ");
	if (scanf("%d", &popravak->id) != 1 || popravak->id <= 0) {
		printf("Neispravan unos ID-a popravka.\n");
		fclose(file);
		free(popravak);
		return;
	}

	// Provjera postoji li ID popravka
	if (postojiIDPopravka(popravak->id)) {
		printf("\nPopravak s tim ID-om vec postoji!\n");
		fclose(file);
		free(popravak);
		return;
	}

	printf("Unesite ID vozila: ");
	if (scanf("%d", &popravak->idVozila) != 1 || popravak->idVozila <= 0) {
		printf("Neispravan unos ID-a vozila.\n");
		fclose(file);
		free(popravak);
		return;
	}

	printf("Unesite ID klijenta: ");
	if (scanf("%d", &popravak->idKlijenta) != 1 || popravak->idKlijenta <= 0) {
		printf("Neispravan unos ID-a klijenta.\n");
		fclose(file);
		free(popravak);
		return;
	}

	printf("Unesite opis popravka: ");
	scanf(" %[^\n]", popravak->opis);

	printf("Unesite cijenu u eurima: ");
	if (scanf("%lf", &popravak->cijena) != 1 || popravak->cijena <= 0) {
		printf("Neispravan unos cijene popravka.\n");
		fclose(file);
		free(popravak);
		return;
	}

	printf("Unesite datum (DD/MM/YYYY): ");
	scanf("%s", popravak->datum);

	printf("Unesite koristene dijelove: ");
	scanf(" %[^\n]", popravak->dijelovi);

	fprintf(file, "%d %d %d %s %.2lf %s %s\n", popravak->id, popravak->idVozila, popravak->idKlijenta, popravak->opis, popravak->cijena, popravak->datum, popravak->dijelovi);
	fclose(file);
	free(popravak);

	printf("Popravak je uspjeno dodan.\n");
}

void ispisPopravaka() {
	FILE* file = fopen("popravci.txt", "r");
	if (file == NULL) {
		perror("\nDatoteka ne postoji!");
		return;
	}

	Popravak popravak;

	printf("\nPopis svih popravaka:\n\n");

	// Postavljanje citaca na pocetak datoteke
	rewind(file);

	while (fscanf(file, "%d %d %d %254[^0-9] %lf %10s %254[^\n]", &popravak.id, &popravak.idVozila, &popravak.idKlijenta, popravak.opis, &popravak.cijena, popravak.datum, popravak.dijelovi) == 7) {
		printf("ID popravka: %d\n", popravak.id);
		printf("ID vozila: %d\n", popravak.idVozila);
		printf("ID klijenta: %d\n", popravak.idKlijenta);
		printf("Opis: %s\n", popravak.opis);
		printf("Cijena: %.2f EUR\n", popravak.cijena);
		printf("Datum: %s\n", popravak.datum);
		printf("Koristeni dijelovi: %s\n", popravak.dijelovi);
		printf("----------------------------------------------------\n");
	}

	fclose(file);
}

void ukupniTrosakPopravaka() {
	FILE* file = fopen("popravci.txt", "r");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za citanje popravaka");
		return;
	}

	Popravak popravak;
	double ukupniTrosak = 0.0;

	// Sprema trenutnu poziciju citaca
	long currentPos = ftell(file);

	while (fscanf(file, "%d %d %d %254[^0-9] %lf %10s %254[^\n]", &popravak.id, &popravak.idVozila, &popravak.idKlijenta, popravak.opis, &popravak.cijena, popravak.datum, popravak.dijelovi) == 7) {
		ukupniTrosak += popravak.cijena;
	}

	printf("\nUkupni trosak svih popravaka: %.2f EUR\n\n", ukupniTrosak);

	// Vraca citac na prethodnu poziciju
	fseek(file, currentPos, SEEK_SET);

	fclose(file);
}


void ukupniTrosakPopravakaZaGodinu() {
	FILE* file = fopen("popravci.txt", "r");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za citanje popravaka");
		return;
	}

	int godina;
	printf("Unesite godinu za koju zelite izracunati ukupni trosak: ");
	if (scanf("%d", &godina) != 1) {
		printf("Neispravan unos godine.\n");
		fclose(file);
		return;
	}

	Popravak popravak;
	double ukupniTrosak = 0.0;
	int postojiPopravak = 0;

	while (fscanf(file, "%d %d %d %254[^0-9] %lf %10s %254[^\n]", &popravak.id, &popravak.idVozila, &popravak.idKlijenta, popravak.opis, &popravak.cijena, popravak.datum, popravak.dijelovi) == 7) {
		int popravakGodina;
		sscanf(popravak.datum + 6, "%d", &popravakGodina); // Izlucujem godinu od datuma
		if (popravakGodina == godina) {
			ukupniTrosak += popravak.cijena;
			postojiPopravak = 1; // Oznacujem da u godini postoji makar jedan popravak
		}
	}

	if (postojiPopravak) {
		printf("\nUkupni trosak svih popravaka za godinu %d: %.2f EUR\n\n", godina, ukupniTrosak);
	}
	else {
		printf("\nU ovoj godini nema zabiljezenih popravaka.\n\n");
	}

	fclose(file);
}
int usporediPopravke(const void* a, const void* b) {
	const Popravak* popravakA = (const Popravak*)a;
	const Popravak* popravakB = (const Popravak*)b;

	if (popravakA->cijena < popravakB->cijena) return -1;
	else if (popravakA->cijena > popravakB->cijena) return 1;
	else return 0;
}

void sortirajPopravke() {
	FILE* file = fopen("popravci.txt", "r");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za popravke");
		return;
	}

	Popravak* popravci = NULL;
	int brojPopravaka = 0;
	Popravak tempPopravak;

	while (fscanf(file, "%d %d %d %254[^0-9] %lf %10s %254[^\n]", &tempPopravak.id, &tempPopravak.idVozila, &tempPopravak.idKlijenta, tempPopravak.opis, &tempPopravak.cijena, tempPopravak.datum, tempPopravak.dijelovi) == 7) {
		popravci = realloc(popravci, sizeof(Popravak) * (brojPopravaka + 1));
		if (popravci == NULL) {
			perror("Ne moze se alocirati memorija za popravke");
			fclose(file);
			return;
		}
		popravci[brojPopravaka++] = tempPopravak;
	}
	fclose(file);

	qsort(popravci, brojPopravaka, sizeof(Popravak), usporediPopravke);

	file = fopen("popravci.txt", "w");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za pisanje popravaka");
		free(popravci);
		return;
	}

	for (int i = 0; i < brojPopravaka; i++) {
		fprintf(file, "%d %d %d %s %.2lf %s %s\n", popravci[i].id, popravci[i].idVozila, popravci[i].idKlijenta, popravci[i].opis, popravci[i].cijena, popravci[i].datum, popravci[i].dijelovi);
	}
	fclose(file);
	free(popravci);

	printf("Popravci su uspjesno sortirani.\n");
}

void obrisiPopravke() {

	const char* imeDatoteke = "popravci.txt";

	if (remove(imeDatoteke) == 0) {
		printf("Datoteka 'popravci.txt' je uspjesno obrisana.\n");
	}
	else {
		perror("Greska pri brisanju datoteke");
	}
}
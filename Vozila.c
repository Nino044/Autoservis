#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Header.h"

// Funkcija koja provjerava postoji li vec uneseni ID
int postojiID(int id) {
	FILE* file = fopen("vozila.txt", "r+");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za vozila");
		exit(1);
	}

	int postojeciId;
	char marka[MADE];
	char model[MODEL];
	char registracija[REGIST];
	int godinaProizvodnje;

	while (fscanf(file, "%d %49s %49s %14s %d", &postojeciId, marka, model, registracija, &godinaProizvodnje) == 5) {
		if (postojeciId == id) {
			fclose(file);
			return 1;  // ID postoji
		}
	}

	fclose(file);
	return 0;  // ID ne postoji
}

void unosNovogVozila() {
	
	FILE* file = fopen("vozila.txt", "a");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za popravke");
		exit(1);
	}

	int id;
	char marka[MADE];
	char model[MODEL];
	char registracija[REGIST];
	int godinaProizvodnje;

	printf("\nUnesite ID: ");
	if (scanf("%d", &id) != 1) {
		printf("Neispravan unos ID-a.\n");
		fclose(file);
		return;
	}

	if (postojiID(id)) {
		printf("\nKlijent s tim ID-om vec postoji!\n");
		fclose(file);
		return;
	}

	printf("\nAko je marka vise od jedne rijeci koristiti '-'!\n");
	printf("Unesite marku: ");
	if (scanf("%s", marka) != 1) {
		printf("Neispravan unos marke.\n");
		fclose(file);
		return;
	}

	printf("\nAko je model vise od jedne rijeci koristiti '-'!\n");
	printf("Unesite model: ");
	if (scanf("%s", model) != 1) {
		printf("Neispravan unos modela.\n");
		fclose(file);
		return;
	}

	printf("\nUnesite registraciju: ");
	if (scanf("%s", registracija) != 1) {
		printf("Neispravan unos registracije.\n");
		fclose(file);
		return;
	}

	printf("\nUnesite godinu proizvodnje: ");
	if (scanf("%d", &godinaProizvodnje) != 1) {
		printf("Neispravan unos godine proizvodnje.\n");
		fclose(file);
		return;
	}

	fprintf(file, "%d %s %s %s %d\n", id, marka, model, registracija, godinaProizvodnje);
	fclose(file);

	printf("Vozilo je uspjesno dodano.\n");
}

void ispisiVozilaHelper(FILE* file) {
	Vozilo vozilo;

	if (fscanf(file, "%d %49s %49s %14s %d", &vozilo.id, vozilo.marka, vozilo.model, vozilo.registracija, &vozilo.godinaProizvodnje) == 5) {
		printf("| %-2d | %-28s | %-28s | %-13s | %-18d |\n", vozilo.id, vozilo.marka, vozilo.model, vozilo.registracija, vozilo.godinaProizvodnje);
		printf("+----+------------------------------+------------------------------+---------------+--------------------+\n");
		ispisiVozilaHelper(file);
	}
}

void ispisiVozila() {
	FILE* file = fopen("vozila.txt", "r");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za vozila");
		exit(1);
	}

	printf("+----+------------------------------+------------------------------+---------------+--------------------+\n");
	printf("| ID |           Marka              |            Model             | Registracija  | Godina proizvodnje |\n");
	printf("+----+------------------------------+------------------------------+---------------+--------------------+\n");

	// postavljam citac na pocetak datoteke
	rewind(file);

	// Rekurzivno pozivamo pomocnu funkciju za ispisivanje podataka iz datoteke
	ispisiVozilaHelper(file);

	fclose(file);

}



void povijestVozila() {
	FILE* file = fopen("popravci.txt", "r");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za citanje popravaka");
		return;
	}

	int idVozila;
	printf("\nUnesite ID vozila za koje zelite vidjeti povijest: ");
	if (scanf("%d", &idVozila) != 1) {
		printf("Neispravan unos ID-a vozila.\n");
		fclose(file);
		return;
	}

	Popravak popravak;

	printf("\nPovijest popravaka za vozilo s ID %d:\n", idVozila);
	printf("-----------------------------------------------------------------------------------------------------------------------\n");

	while (fscanf(file, "%d %d %d %254[^0-9] %lf %10s %254[^\n]", &popravak.id, &popravak.idVozila, &popravak.idKlijenta, popravak.opis, &popravak.cijena, popravak.datum, popravak.dijelovi) == 7) {
		if (popravak.idVozila == idVozila) {

			printf("ID popravka: %d\n", popravak.id);
			printf("ID klijenta: %d\n", popravak.idKlijenta);
			printf("Opis: %s\n", popravak.opis);
			printf("Cijena: %.2f EUR\n", popravak.cijena);
			printf("Datum: %s\n", popravak.datum);
			printf("Koristeni dijelovi: %s\n", popravak.dijelovi);
			printf("-----------------------------------------------------------------------------------------------------------------------\n");
		}
	}

	fclose(file);
}

int usporediVozila(const void* a, const void* b) {
	const Vozilo* voziloA = (const Vozilo*)a;
	const Vozilo* voziloB = (const Vozilo*)b;
	return strcmp(voziloA->marka, voziloB->marka);
}

void sortirajVozila() {
	FILE* file = fopen("vozila.txt", "r");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za vozila");
		return;
	}

	Vozilo* vozila = NULL;
	int brojVozila = 0;
	Vozilo sortVozilo;

	while (fscanf(file, "%d %19s %49s %14s %d", &sortVozilo.id, sortVozilo.marka, sortVozilo.model, sortVozilo.registracija, &sortVozilo.godinaProizvodnje) == 5) {
		vozila = realloc(vozila, sizeof(Vozilo) * (brojVozila + 1));
		if (vozila == NULL) {
			perror("Ne moze se alocirati memorija za vozila");
			fclose(file);
			return;
		}
		vozila[brojVozila++] = sortVozilo;
	}
	fclose(file);

	qsort(vozila, brojVozila, sizeof(Vozilo), usporediVozila);

	file = fopen("vozila.txt", "w");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za pisanje vozila");
		free(vozila);
		return;
	}

	for (int i = 0; i < brojVozila; i++) {
		fprintf(file, "%d %s %s %s %d\n", vozila[i].id, vozila[i].marka, vozila[i].model, vozila[i].registracija, vozila[i].godinaProizvodnje);
	}
	fclose(file);
	free(vozila);

	printf("\nVozila su uspjesno sortirana.\n");
}

void azurirajRegistracijuVozila() {
	FILE* file = fopen("vozila.txt", "r");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za citanje vozila");
		return;
	}

	Vozilo* vozila = NULL;
	int brojVozila = 0;
	Vozilo azurVozilo;

	while (fscanf(file, "%d %49s %49s %14s %d", &azurVozilo.id, azurVozilo.marka, azurVozilo.model, azurVozilo.registracija, &azurVozilo.godinaProizvodnje) == 5) {
		vozila = realloc(vozila, sizeof(Vozilo) * (brojVozila + 1));
		if (vozila == NULL) {
			perror("Neuspjesno zauzimanje memorije za vozila");
			fclose(file);
			return;
		}
		vozila[brojVozila++] = azurVozilo;
	}
	fclose(file);

	int idVozila;
	printf("Unesite ID vozila koje zelite azurirati: ");
	if (scanf("%d", &idVozila) != 1) {
		printf("Neispravan unos ID-a vozila.\n");
		free(vozila);
		return;
	}

	int found = 0;
	for (int i = 0; i < brojVozila; ++i) {
		if (vozila[i].id == idVozila) {
			found = 1;
			printf("Unesite novu registraciju: ");
			if (scanf("%s", vozila[i].registracija) != 1) {
				printf("Neispravan unos registracije.\n");
				free(vozila);
				return;
			}
			break;
		}
	}

	if (!found) {
		printf("Vozilo s ID %d nije pronadeno.\n", idVozila);
		free(vozila);
		return;
	}

	file = fopen("vozila.txt", "w");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za pisanje vozila");
		free(vozila);
		return;
	}

	for (int i = 0; i < brojVozila; ++i) {
		fprintf(file, "%d %s %s %s %d\n", vozila[i].id, vozila[i].marka, vozila[i].model, vozila[i].registracija, vozila[i].godinaProizvodnje);
	}

	fclose(file);
	free(vozila);

	printf("Registracija vozila s ID %d uspjesno azurirana.\n", idVozila);
}

void obrisiVozila() {

	const char* imeDatoteke = "vozila.txt";

	if (remove(imeDatoteke) == 0) {
		printf("Datoteka 'vozila.txt' je uspjesno obrisana.\n");
	}
	else {
		perror("Greska pri brisanju datoteke");
	}
}

void obrisiVozilo(int id) {
	FILE* file = fopen("vozila.txt", "r");
	if (file == NULL) {
		perror("Ne može se otvoriti datoteka za èitanje vozila");
		return;
	}

	Vozilo* vozila = NULL;
	int brojVozila = 0;
	Vozilo delVozilo;

	while (fscanf(file, "%d %49s %49s %14s %d", &delVozilo.id, delVozilo.marka, delVozilo.model, delVozilo.registracija, &delVozilo.godinaProizvodnje) == 5) {
		vozila = realloc(vozila, sizeof(Vozilo) * (brojVozila + 1));
		if (vozila == NULL) {
			perror("Ne može se alocirati memorija za vozila");
			fclose(file);
			return;
		}
		vozila[brojVozila++] = delVozilo;
	}
	fclose(file);

	int found = 0;
	for (int i = 0; i < brojVozila; i++) {
		if (vozila[i].id == id) {
			for (int j = i; j < brojVozila - 1; j++) {
				vozila[j] = vozila[j + 1];
			}
			brojVozila--;
			found = 1;
			break;
		}
	}

	if (!found) {
		printf("Vozilo s ID %d nije pronadeno.\n", id);
		free(vozila);
		return;
	}

	file = fopen("vozila.txt", "w");
	if (file == NULL) {
		perror("Ne može se otvoriti datoteka za pisanje vozila");
		free(vozila);
		return;
	}

	for (int i = 0; i < brojVozila; i++) {
		fprintf(file, "%d %s %s %s %d\n", vozila[i].id, vozila[i].marka, vozila[i].model, vozila[i].registracija, vozila[i].godinaProizvodnje);
	}
	fclose(file);
	free(vozila);

	printf("Vozilo s ID %d je uspjesno obrisano.\n", id);
}

void obrisiVoziloUI() {
	int id;
	printf("Unesite ID vozila za brisanje: ");
	scanf("%d", &id);
	obrisiVozilo(id);
}

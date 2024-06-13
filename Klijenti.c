#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Header.h"

// sigurno oslobadanje prethodno alocirane memorije i postavljanje pokazivaca na NULL kako bi se sprijecilo koristenje dangling pointers-a
void sigurnoOslobodi(void** ptr) {
	if (*ptr != NULL) {
		free(*ptr);
		*ptr = NULL;
	}
}

void unosNovogKlijenta() {
	FILE* file = fopen("klijenti.txt", "a+");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za klijente");
		exit(1);
	}

	Klijent* noviKlijent = (Klijent*)malloc(sizeof(Klijent));
	if (noviKlijent == NULL) {
		perror("Ne moze se alocirati memorija za novog klijenta");
		fclose(file);
		exit(1);
	}

	printf("Unesite ID: ");
	if (scanf("%d", &noviKlijent->id) != 1) {
		printf("Neispravan unos ID-a.\n");
		fclose(file);
		sigurnoOslobodi((void**)&noviKlijent);
		return;
	}

	// Provjera postojanja ID-a
	fseek(file, 0, SEEK_SET);  // Vracanje pokazivaca na pocetak datoteke
	Klijent tempKlijent;
	int idExists = 0;
	while (fscanf(file, "%d %49s %49s %19s %49s", &tempKlijent.id, tempKlijent.ime, tempKlijent.prezime, tempKlijent.telefon, tempKlijent.email) == 5) {
		if (tempKlijent.id == noviKlijent->id) {
			idExists = 1;
			break;
		}
	}
	if (idExists) {
		printf("Klijent s ID-om %d vec postoji.\n", noviKlijent->id);
		fclose(file);
		sigurnoOslobodi((void**)&noviKlijent);
		return;
	}

	printf("Unesite ime: ");
	if (scanf("%s", noviKlijent->ime) != 1) {
		printf("Neispravan unos imena.\n");
		fclose(file);
		sigurnoOslobodi((void**)&noviKlijent);
		return;
	}

	printf("Unesite prezime: ");
	if (scanf("%s", noviKlijent->prezime) != 1) {
		printf("Neispravan unos prezimena.\n");
		fclose(file);
		sigurnoOslobodi((void**)&noviKlijent);
		return;
	}

	printf("Unesite telefon: ");
	if (scanf("%s", noviKlijent->telefon) != 1) {
		printf("Neispravan unos telefona.\n");
		fclose(file);
		sigurnoOslobodi((void**)&noviKlijent);
		return;
	}

	printf("Unesite email: ");
	if (scanf("%s", noviKlijent->email) != 1) {
		printf("Neispravan unos email-a.\n");
		fclose(file);
		sigurnoOslobodi((void**)&noviKlijent);
		return;
	}

	fprintf(file, "%d %s %s %s %s\n", noviKlijent->id, noviKlijent->ime, noviKlijent->prezime, noviKlijent->telefon, noviKlijent->email);

	fclose(file);
	sigurnoOslobodi((void**)&noviKlijent);

	printf("Klijent uspjesno dodan.\n\n");
}


void ispisiKlijente() {
	FILE* file = fopen("klijenti.txt", "r");
	if (file == NULL) {
		perror("Datoteka ne postoji!");
		exit(1);
	}

	// pomicanje pokazivaca na pocetak datoteke
	fseek(file, 0, SEEK_SET);

	Klijent* klijent = (Klijent*)malloc(sizeof(Klijent));
	if (klijent == NULL) {
		perror("Ne moze se alocirati memorija za klijenta");
		fclose(file);
		exit(1);
	}

	printf("+----+------------------------------+------------------------------+--------------------+------------------------------+\n");
	printf("| ID |            Ime               |           Prezime            |      Telefon       |           Email              |\n");
	printf("+----+------------------------------+------------------------------+--------------------+------------------------------+\n");

	while (fscanf(file, "%d %49s %49s %19s %49s", &klijent->id, klijent->ime, klijent->prezime, klijent->telefon, klijent->email) == 5) {
		printf("| %-2d | %-28s | %-28s | %-18s | %-28s |\n", klijent->id, klijent->ime, klijent->prezime, klijent->telefon, klijent->email);
		printf("+----+------------------------------+------------------------------+--------------------+------------------------------+\n");
	}

	// pomicanje pokazivaca na kraj datoteke
	fseek(file, 0, SEEK_END);

	// pomicanje pokazivaca na pocetak datoteke
	rewind(file);

	sigurnoOslobodi((void**)&klijent);
	fclose(file);
}

int usporediKlijentePoImenu(const void* a, const void* b) {
	const Klijent* klijentA = (const Klijent*)a;
	const Klijent* klijentB = (const Klijent*)b;
	return strcmp(klijentA->ime, klijentB->ime);
}

void sortirajKlijente() {
	FILE* file = fopen("klijenti.txt", "r");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za klijente");
		return;
	}

	Klijent* klijenti = NULL;
	int brojKlijenata = 0;
	Klijent tempKlijent;

	while (fscanf(file, "%d %19s %29s %19s %49s", &tempKlijent.id, tempKlijent.ime, tempKlijent.prezime, tempKlijent.telefon, tempKlijent.email) == 5) {
		klijenti = realloc(klijenti, sizeof(Klijent) * (brojKlijenata + 1));
		if (klijenti == NULL) {
			perror("Ne moze se alocirati memorija za klijente");
			fclose(file);
			return;
		}
		klijenti[brojKlijenata++] = tempKlijent;
	}
	fclose(file);

	qsort(klijenti, brojKlijenata, sizeof(Klijent), usporediKlijentePoImenu);

	file = fopen("klijenti.txt", "w");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za pisanje klijenata");
		free(klijenti);
		return;
	}

	for (int i = 0; i < brojKlijenata; i++) {
		fprintf(file, "%d %s %s %s %s\n", klijenti[i].id, klijenti[i].ime, klijenti[i].prezime, klijenti[i].telefon, klijenti[i].email);
	}
	fclose(file);
	free(klijenti);

	printf("\nKlijenti su uspjesno sortirani.\n");
}

int usporediKlijentePoID(const void* a, const void* b) {
	const Klijent* klijentA = (const Klijent*)a;
	const Klijent* klijentB = (const Klijent*)b;
	return (klijentA->id - klijentB->id);
}

Klijent* pretraziKlijenta(int id) {
	FILE* file = fopen("klijenti.txt", "r");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za klijente");
		return NULL;
	}

	Klijent* klijenti = NULL;
	int brojKlijenata = 0;
	Klijent tempKlijent;

	while (fscanf(file, "%d %19s %29s %19s %49s", &tempKlijent.id, tempKlijent.ime, tempKlijent.prezime, tempKlijent.telefon, tempKlijent.email) == 5) {
		klijenti = realloc(klijenti, sizeof(Klijent) * (brojKlijenata + 1));
		if (klijenti == NULL) {
			perror("Ne moze se alocirati memorija za klijente");
			fclose(file);
			return NULL;
		}
		klijenti[brojKlijenata++] = tempKlijent;
	}
	fclose(file);

	qsort(klijenti, brojKlijenata, sizeof(Klijent), usporediKlijentePoID);

	Klijent key = { .id = id };
	Klijent* result = (Klijent*)bsearch(&key, klijenti, brojKlijenata, sizeof(Klijent), usporediKlijentePoID);

	if (result) {
		Klijent* klijent = malloc(sizeof(Klijent));
		if (klijent != NULL) {
			*klijent = *result;
		}
		free(klijenti);
		return klijent;
	}
	else {
		free(klijenti);
		return NULL;
	}
}

void ispisiKlijenta(Klijent* klijent) {
	if (klijent) {
		printf("+----+------------------------------+------------------------------+--------------------+------------------------------+\n");
		printf("| ID |            Ime               |           Prezime            |      Telefon       |           Email              |\n");
		printf("+----+------------------------------+------------------------------+--------------------+------------------------------+\n");
		printf("| %-2d | %-28s | %-28s | %-18s | %-28s |\n", klijent->id, klijent->ime, klijent->prezime, klijent->telefon, klijent->email);
		printf("+----+------------------------------+------------------------------+--------------------+------------------------------+\n");
		free(klijent);
	}
	else {
		printf("Klijent nije pronaden.\n");
	}
}


void pretraziIKlijenta() {
	int id;
	printf("Unesite ID klijenta: ");
	scanf("%d", &id);
	Klijent* klijent = pretraziKlijenta(id);
	ispisiKlijenta(klijent);
}

void azurirajKlijenta(int id, const char* noviTelefon, const char* noviEmail) {
	FILE* file = fopen("klijenti.txt", "r");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za citanje klijenata");
		return;
	}

	Klijent* klijenti = NULL;
	int brojKlijenata = 0;
	Klijent tempKlijent;

	while (fscanf(file, "%d %19s %29s %19s %49s", &tempKlijent.id, tempKlijent.ime, tempKlijent.prezime, tempKlijent.telefon, tempKlijent.email) == 5) {
		klijenti = realloc(klijenti, sizeof(Klijent) * (brojKlijenata + 1));
		if (klijenti == NULL) {
			perror("Ne moze se alocirati memorija za klijente");
			fclose(file);
			return;
		}
		klijenti[brojKlijenata++] = tempKlijent;
	}
	fclose(file);

	int found = 0;
	for (int i = 0; i < brojKlijenata; i++) {
		if (klijenti[i].id == id) {
			strncpy(klijenti[i].telefon, noviTelefon, sizeof(klijenti[i].telefon) - 1);
			klijenti[i].telefon[sizeof(klijenti[i].telefon) - 1] = '\0';
			strncpy(klijenti[i].email, noviEmail, sizeof(klijenti[i].email) - 1);
			klijenti[i].email[sizeof(klijenti[i].email) - 1] = '\0';
			found = 1;
			break;
		}
	}

	if (!found) {
		printf("Klijent sa ID %d nije pronaden.\n", id);
		free(klijenti);
		return;
	}

	file = fopen("klijenti.txt", "w");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za pisanje klijenata");
		free(klijenti);
		return;
	}

	for (int i = 0; i < brojKlijenata; i++) {
		fprintf(file, "%d %s %s %s %s\n", klijenti[i].id, klijenti[i].ime, klijenti[i].prezime, klijenti[i].telefon, klijenti[i].email);
	}
	fclose(file);
	free(klijenti);

	printf("Klijent sa ID %d je uspjesno azuriran.\n", id);
}

void azurirajKlijentaUI() {
	int id;
	char noviTelefon[PHONE];
	char noviEmail[MAIL];

	printf("Unesite ID klijenta za azuriranje: ");
	scanf("%d", &id);
	printf("Unesite novi telefon: ");
	scanf("%s", noviTelefon);
	printf("Unesite novi email: ");
	scanf("%s", noviEmail);

	azurirajKlijenta(id, noviTelefon, noviEmail);
}

void obrisiKlijenta(int id) {
	FILE* file = fopen("klijenti.txt", "r");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za citanje klijenata");
		return;
	}

	Klijent* klijenti = NULL;
	int brojKlijenata = 0;
	Klijent tempKlijent;

	while (fscanf(file, "%d %19s %29s %19s %49s", &tempKlijent.id, tempKlijent.ime, tempKlijent.prezime, tempKlijent.telefon, tempKlijent.email) == 5) {
		klijenti = realloc(klijenti, sizeof(Klijent) * (brojKlijenata + 1));
		if (klijenti == NULL) {
			perror("Ne moze se alocirati memorija za klijente");
			fclose(file);
			return;
		}
		klijenti[brojKlijenata++] = tempKlijent;
	}
	fclose(file);

	int found = 0;
	for (int i = 0; i < brojKlijenata; i++) {
		if (klijenti[i].id == id) {
			for (int j = i; j < brojKlijenata - 1; j++) {
				klijenti[j] = klijenti[j + 1];
			}
			brojKlijenata--;
			found = 1;
			break;
		}
	}

	if (!found) {
		printf("Klijent sa ID %d nije pronaden.\n", id);
		free(klijenti);
		return;
	}

	file = fopen("klijenti.txt", "w");
	if (file == NULL) {
		perror("Ne moze se otvoriti datoteka za pisanje klijenata");
		free(klijenti);
		return;
	}

	for (int i = 0; i < brojKlijenata; i++) {
		fprintf(file, "%d %s %s %s %s\n", klijenti[i].id, klijenti[i].ime, klijenti[i].prezime, klijenti[i].telefon, klijenti[i].email);
	}
	fclose(file);
	free(klijenti);

	printf("Klijent sa ID %d je uspjesno obrisan.\n", id);
}


void obrisiKlijentaUI() {
	int id;
	printf("Unesite ID klijenta za brisanje: ");
	scanf("%d", &id);
	obrisiKlijenta(id);
}

void obrisiKlijente() {

	const char* imeDatoteke = "klijenti.txt";

	if (remove(imeDatoteke) == 0) {
		printf("Datoteka 'klijenti.txt' je uspjesno obrisana.\n");
	}
	else {
		perror("Greska pri brisanju datoteke");
	}
}
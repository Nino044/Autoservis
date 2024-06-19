#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Header.h"


void prikaziGlavniIzbornik() {
	printf("\nOdaberite opciju:\n");
	printf("1. Opcije klijenata\n");
	printf("2. Opcije vozila\n");
	printf("3. Opcije popravaka\n");
	printf("4. Odjava administratora\n");
	printf("0. Izlaz\n");
	printf("Odabir: ");
}

void prikaziIzbornikKlijenata() {
	printf("\nOdaberite opciju za klijente:\n");
	printf("1. Unos novog klijenta\n");
	printf("2. Ispisi klijente\n");
	printf("3. Sortiraj klijente\n");
	printf("4. Pretrazi klijenta\n");
	printf("5. Azuriraj klijenta\n");
	printf("6. Obrisi klijenta\n");
	printf("7. Obrisi datoteku klijenata\n");
	printf("0. Povratak\n");
	printf("Odabir: ");
}

void prikaziIzbornikVozila() {
	printf("\nOdaberite opciju za vozila:\n");
	printf("1. Unos novog vozila\n");
	printf("2. Ispisi vozila\n");
	printf("3. Povijest vozila\n");
	printf("4. Sortiraj vozila\n");
	printf("5. Azuriraj registraciju vozila\n");
	printf("6. Obrisi vozilo\n");
	printf("7. Obrisi datoteku vozila\n");
	printf("0. Povratak\n");
	printf("Odabir: ");
}

void prikaziIzbornikPopravaka() {
	printf("\nOdaberite opciju za popravke:\n");
	printf("1. Unos novog popravka\n");
	printf("2. Ispis popravaka\n");
	printf("3. Ispis ukupnih troskova popravaka\n");
	printf("4. Ukupni trosak popravaka za odredenu godinu\n");
	printf("5. Sortiraj popravke\n");
	printf("6. Obrisi datoteku popravaka\n");
	printf("0. Povratak\n");
	printf("Odabir: ");
}

int main() {
	int izbor;
pocetak:
	do {
		printf("1. Registracija administratora\n");
		printf("2. Prijava administratora\n");
		printf("3. Izlaz\n");
		printf("Izbor: ");

		if (scanf("%d", &izbor) != 1) {
			printf("Neispravan unos. Unesite broj.\n");
			ocistiUlazniSpremnik(); // pozivam funkciju za ciscenje ulaznog spremnika
			continue;
		}

		switch (izbor) {
		case 1:
			globalnaRegistracijaAdmin();
			break;
		case 2:
			if (globalnaPrijavaAdmin()) {
				printf("Dobrodosli!\n");
				goto start;
			}
			break;
		case 3:
			printf("Izlaz.\n");
			return 0;
			break;
		default:
			printf("\nNeispravan izbor. Pokusajte ponovno.\n");
		}
	} while (izbor != 3);

start:
	do {
		prikaziGlavniIzbornik();
		while (scanf("%d", &izbor) != 1) {
			printf("\nNeispravan unos. Molimo unesite broj.\n");
			while (getchar() != '\n'); // cistim ulazni bafer
			goto start;
		}
		switch (izbor) {
		case 1:
			do {
				prikaziIzbornikKlijenata();
				scanf("%d", &izbor);

				switch (izbor) {
				case 1:
					unosNovogKlijenta();
					break;
				case 2:
					ispisiKlijente();
					break;
				case 3:
					sortirajKlijente();
					break;
				case 4:
					pretraziIKlijenta();
					break;
				case 5:
					azurirajKlijentaUI();
					break;
				case 6:
					obrisiKlijentaUI();
					break;
				case 7:
					obrisiKlijente();
					break;
				case 0:
					goto start;
					break;
				default:
					printf("Neispravan odabir. Molimo odaberite ponovno.\n");
				}
			} while (izbor != 0);
			return 0;
			break;

		case 2:
			do {
				prikaziIzbornikVozila();
				scanf("%d", &izbor);

				switch (izbor) {
				case 1:
					unosNovogVozila();
					break;
				case 2:
					ispisiVozila();
					break;
				case 3:
					povijestVozila();
					break;
				case 4:
					sortirajVozila();
					break;
				case 5:
					azurirajRegistracijuVozila();
					break;
				case 6:
					obrisiVoziloUI();
					break;
				case 7:
					obrisiVozila();
					break;
				case 0:
					goto start;
					break;
				default:
					printf("Neispravan odabir. Molimo odaberite ponovno.\n");
				}
			} while (izbor != 0);
			return 0;
			break;

		case 3:
			do {
				prikaziIzbornikPopravaka();
				scanf("%d", &izbor);

				switch (izbor) {
				case 1:
					noviPopravak();
					break;
				case 2:
					ispisPopravaka();
					break;
				case 3:
					ukupniTrosakPopravaka();
					break;
				case 4:
					ukupniTrosakPopravakaZaGodinu();
					break;
				case 5:
					sortirajPopravke();
					break;
				case 6:
					obrisiPopravke();
					break;
				case 0:
					goto start;
					break;
				default:
					printf("Neispravan odabir. Molimo odaberite ponovno.\n");
				}
			} while (izbor != 0);
			return 0;
			break;
		case 4:
			globalnaOdjavaAdmin();
			goto pocetak;
		case 0:
			printf("Izlaz iz programa.\n");
			return 0;
			break;

		default:
			printf("Neispravan odabir. Molimo odaberite ponovno.\n");
		}
	} while (izbor != 0);

	return 0;
}
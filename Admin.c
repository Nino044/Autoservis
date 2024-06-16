#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Header.h"

int prijavljen = 0;

static int provjeriPostojeciAdmin(const char* korisnickoIme) {
    FILE* file = fopen("administratori.txt", "r");
    if (file == NULL) {
        return 0; // Datoteka ne postoji, admin ne postoji
    }

    char existingUsername[USER];
    char existingPassword[PASSWORD];

    while (fscanf(file, "%s %s", existingUsername, existingPassword) != EOF) {
        if (strcmp(existingUsername, korisnickoIme) == 0) {
            fclose(file);
            return 1; // Korisnicko ime vec postoji
        }
    }

    fclose(file);
    return 0; // Korisnicko ime ne postoji
}

static void registracijaAdmin() {
    Administrator admin;

    printf("Unesite korisnicko ime: ");
    scanf("%s", admin.korisnickoIme);

    if (provjeriPostojeciAdmin(admin.korisnickoIme)) {
        printf("Korisnicko ime vec postoji. Pokusajte ponovo.\n");
        return;
    }

    printf("Unesite lozinku: ");
    scanf("%s", admin.lozinka);

    FILE* file = fopen("administratori.txt", "a");
    if (file == NULL) {
        perror("Ne može se otvoriti datoteka za administratore");
        exit(1);
    }

    fprintf(file, "%s %s\n", admin.korisnickoIme, admin.lozinka);
    fclose(file);

    printf("\nAdministrator uspjesno registriran.\n\n");
}

static int prijavaAdmin() {

    FILE* file = fopen("administratori.txt", "r");
    if (file == NULL) {
        perror("Ne moze se otvoriti datoteka za administratore");
        return 0;
    }

    char korisnickoIme[USER];
    char lozinka[PASSWORD];

    printf("Unesite korisnicko ime: ");
    scanf("%s", korisnickoIme);
    printf("Unesite lozinku: ");
    scanf("%s", lozinka);

    Administrator admin;
    while (fscanf(file, "%s %s", admin.korisnickoIme, admin.lozinka) != EOF) {
        if (strcmp(admin.korisnickoIme, korisnickoIme) == 0 && strcmp(admin.lozinka, lozinka) == 0) {
            fclose(file);
            prijavljen = 1; // ako je prijavljen globalna varijabla se postavlja kao 1
            printf("\nPrijava uspjesna.\n");
            return 1;
        }
    }

    fclose(file);
    printf("Neispravno korisnicko ime ili lozinka.\n");
    return 0;
}

static void odjavaAdmin() {
    if (prijavljen) {
        printf("Odjava uspjesna.\n");
        prijavljen = 0; //nakon odjave globalnu varijablu vracamo na 0
    }
    
}

inline void ocistiUlazniSpremnik() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// globalne funkcije koje pozivaju statièke funkcije
void globalnaRegistracijaAdmin() {
    registracijaAdmin();
}

int globalnaPrijavaAdmin() {
    return prijavaAdmin();
}

int globalnaOdjavaAdmin() {
    odjavaAdmin();
}

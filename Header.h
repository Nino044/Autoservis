#ifndef HEADER_H
#define HEADER_H

#define NAME 20
#define SURNAME 30
#define PHONE 20
#define MAIL 50
#define MADE 20
#define MODEL 50
#define REGIST 15
#define DESCRIBE 255
#define DATE 11
#define REPAIR 50
#define PARTS 255
#define USER 20
#define PASSWORD 20

extern void prikaziGlavniIzbornik();
extern void prikaziIzbornikKlijenata();
extern void prikaziIzbornikVozila();
extern void prikaziIzbornikPopravaka();

typedef struct {
	int id;
	char marka[MADE];
	char model[MODEL];
	char registracija[REGIST];
	int godinaProizvodnje;
} Vozilo;

typedef struct {
	int id;
	char ime[NAME];
	char prezime[SURNAME];
	char telefon[PHONE];
	char email[MAIL];
} Klijent;

typedef struct {
	int id;
	int idVozila;
	int idKlijenta;
	char opis[DESCRIBE];
	double cijena;
	char datum[DATE];
	char dijelovi[PARTS];
} Popravak;

typedef struct {
	char korisnickoIme[USER];
	char lozinka[PASSWORD];
} Administrator;


//Vozila
extern void unosNovogVozila();
extern void ispisiVozila();
extern void povijestVozila();
extern void azurirajRegistracijuVozila();
extern void obrisiVoziloUI();
extern void obrisiVozila();
//Klijenti
extern void unosNovogKlijenta();
extern void ispisiKlijente();
extern void azurirajKlijentaUI();
extern void obrisiKlijentaUI();
extern void obrisiKlijente();

//Popravak
extern void noviPopravak();
extern void ispisPopravaka();
extern void ukupniTrosakPopravaka();
extern void ukupniTrosakPopravakaZaGodinu();
extern void obrisiPopravke();

//Sortiranje
extern void sortirajKlijente();
extern void sortirajVozila();
extern void sortirajPopravke();

//Pretrazivanje
extern void pretraziIKlijenta();

//Admin
extern void globalnaRegistracijaAdmin();
int globalnaPrijavaAdmin();
int globalnaOdjavaAdmin();
extern void ocistiUlazniSpremnik();

#endif
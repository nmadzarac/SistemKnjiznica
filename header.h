#ifndef HEADER_H
#define HEADER_H

typedef struct {
    int id;
    char naslov[50];
    char zanr[50];
    int godina;
    char autor[50];
    int posudena;  // 0 - nije posudena, 1 - posudena
    char korisnik[50];  // korisnicko ime osobe koja je posudila knjigu
} KNJIGA;

enum Opcije {
    dodavanjeKnjiga = 1,
    brisanjeKnjiga,
    popisiKnjige,
    pretraziKnjigu,
    sortiranjeKnjigaPoNaslovu,
    posudbaKnjiga,
    vracanjeKnjiga,
    izlazIzPrograma
};

void dodajKnjigu();
void obrisiKnjigu();
void popisKnjiga();
void pretrazivanjeKnjiga();
void sortirajKnjigePoNaslovu();
void posudbaKnjige();
void vracanjeKnjige();

#endif // HEADER_H

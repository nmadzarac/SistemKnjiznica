#pragma once
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
    sortiranjeKnjiga,
    posudbaKnjiga,
    vracanjeKnjiga,
    azuriranjeKnjiga,
    izlazIzPrograma
};

enum OpcijePretrage {
    pretraziPoNaslovu = 1,
    pretraziPoAutoru,
    pretraziPoZanru,
    pretraziPoId
};

void dodajKnjigu();
void obrisiKnjigu();
void popisKnjiga();
void pretrazivanjeKnjiga();
void sortirajKnjige(int);
void posudbaKnjige();
void vracanjeKnjige();
void azurirajKnjigu();

#endif // HEADER_H


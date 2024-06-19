#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "header1.h"

int prijava() {

    char username[50];
    char password[50];
    int admin = 0;

    while (1) {
        printf("Odaberite tip korisnika:\n");
        printf("1. Student\n");
        printf("2. Administrator\n");

        int izbor;
        if (scanf("%d", &izbor) != 1) {
            printf("Neispravan unos. Pokusajte ponovo.\n");
            clearInputBuffer();
            continue;
        }

        if (izbor == 1) {
            admin = 0;
            break;
        }
        else if (izbor == 2) {
            printf("Unesite korisnicko ime: ");
            scanf("%s", username);
            printf("Unesite lozinku: ");
            scanf("%s", password);

            if (strcmp(username, "admin") == 0 && strcmp(password, "12345") == 0) {
                admin = 1;
                printf("Uspjesna prijava kao administrator.\n");
                break;
            }
            else {
                printf("Neispravno korisnicko ime ili lozinka. Pokusajte ponovo.\n");
            }
        }
        else {
            printf("Neispravan izbor. Pokusajte ponovo.\n");
        }
    }

    int uvjet = 1;
    while (uvjet) {
        uvjet = izbornik(admin);
    }
}

int izbornik(int admin) {
    while (1) {
        printf("\n=========================================================================");
        printf("\nIZBORNIK --- odaberite opciju:\n");

        if (admin) {
            printf("\tOpcija 1: Dodavanje knjiga\n");
            printf("\tOpcija 2: Brisanje knjiga\n");
            printf("\tOpcija 3: Popis knjiga\n");
            printf("\tOpcija 4: Pretrazivanje knjiga\n");
            printf("\tOpcija 5: Sortiranje knjiga\n");
            printf("\tOpcija 6: Posudba knjiga\n");
            printf("\tOpcija 7: Vracanje knjiga\n");
            printf("\tOpcija 8: Azuriranje knjiga\n");
            printf("\tOpcija 9: Izlaz iz programa\n");
        }
        else {
            printf("\tOpcija 1: Popis knjiga\n");
            printf("\tOpcija 2: Pretrazivanje knjiga\n");
            printf("\tOpcija 3: Sortiranje knjiga\n");
            printf("\tOpcija 4: Posudba knjiga\n");
            printf("\tOpcija 5: Vracanje knjiga\n");
            printf("\tOpcija 6: Izlaz iz programa\n");
        }

        int odabranaOpcija = 0;
        
        if (admin) {
            if (scanf("%d", &odabranaOpcija) != 1) {
                printf("Nepoznata opcija. Pokusajte ponovo.\n");
                while (getchar() != '\n');
                continue;
            }
            //printf("%d", odabranaOpcija);
        }

        else {
            scanf("%d", &odabranaOpcija);
            odabranaOpcija = odabranaOpcija + 2;
            //printf("%d", odabranaOpcija);
            if (odabranaOpcija < 1 || odabranaOpcija >8) {
                printf("Nepoznata opcija. Pokusajte ponovo.\n");
                while (getchar() != '\n');
                continue;
            }
            if(odabranaOpcija == 8){
                odabranaOpcija = 9;
            }
        }

        switch (odabranaOpcija) {
        case dodavanjeKnjiga:
            if (admin) {
                dodajKnjigu();
            }
            else {
                printf("Nemate ovlasti za ovu opciju.\n");
            }
            break;
        case brisanjeKnjiga:
            if (admin) {
                obrisiKnjigu();
            }
            else {
                printf("Nemate ovlasti za ovu opciju.\n");
            }
            break;
        case popisiKnjige:
            popisKnjiga();
            break;
        case pretraziKnjigu:
            pretrazivanjeKnjiga();
            break;
        case sortiranjeKnjiga:
            printf("Sortiraj po:\n\t1 - Naslov\n\t2 - ID\n\t3 - Autor\n\t4 - Godina\n\t5 - Zanr\n");
            int kriterij;
            scanf("%d", &kriterij);
            sortirajKnjige(kriterij);
            break;
        case posudbaKnjiga:
            posudbaKnjige();
            break;
        case vracanjeKnjiga:
            vracanjeKnjige();
            break;
        case azuriranjeKnjiga:
            if (admin) {
                azurirajKnjigu();
            }
            else {
                printf("Nemate ovlasti za ovu opciju.\n");
            }
            break;
        case izlazIzPrograma:
            printf("Izlaz iz programa...\n");
            return 0;
        default:
            printf("Nepoznata opcija. Pokusajte ponovo.\n");
            break;
        }
    }
}


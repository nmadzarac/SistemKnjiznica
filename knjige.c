#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "header1.h"

void dodajKnjigu() {
    FILE* fp = fopen("knjiznica.bin", "ab");
    if (fp == NULL) {
        perror("Greska pri otvaranju datoteke.\n");
        return;
    }

    KNJIGA novaKnjiga;
    novaKnjiga.posudena = 0;
    memset(novaKnjiga.korisnik, 0, sizeof(novaKnjiga.korisnik));

    printf("Unesite ID knjige: ");
    scanf("%d", &novaKnjiga.id);
    clearInputBuffer();

    printf("Unesite naslov knjige: ");
    fgets(novaKnjiga.naslov, sizeof(novaKnjiga.naslov), stdin);
    novaKnjiga.naslov[strcspn(novaKnjiga.naslov, "\n")] = 0;

    int izborZanra;
    printf("\nOdaberite zanr knjige:\n");
    printf("1. Roman\n");
    printf("2. Horror\n");
    printf("3. Fantazija\n");
    printf("4. Znanstvena fantastika\n");
    printf("5. Komedija\n");
    printf("6. Drama\n");
    printf("7. Ostalo\n");
    if (scanf("%d", &izborZanra) != 1 || izborZanra < 1 || izborZanra > 7) {
        printf("Neispravan unos zanra. Postavljanje zanra na 'Ostalo'.\n");
        izborZanra = 7;
    }
    clearInputBuffer();

    const char* zanrovi[] = { "", "Roman", "Horror", "Fantazija", "Znanstvena fantastika", "Komedija", "Drama", "Ostalo" };
    strcpy(novaKnjiga.zanr, zanrovi[izborZanra]);

    printf("Unesite godinu izdanja: ");
    scanf("%d", &novaKnjiga.godina);
    clearInputBuffer();

    printf("Unesite autora knjige: ");
    fgets(novaKnjiga.autor, sizeof(novaKnjiga.autor), stdin);
    novaKnjiga.autor[strcspn(novaKnjiga.autor, "\n")] = 0;

    fwrite(&novaKnjiga, sizeof(KNJIGA), 1, fp);
    fclose(fp);

    printf("Knjiga je uspjesno dodana.\n");
}

void obrisiKnjigu() {
    FILE* fp = fopen("knjiznica.bin", "rb");
    if (fp == NULL) {
        perror("Error: Neuspjesno otvaranje datoteke.");
        return;
    }

    int id;
    printf("Unesite ID knjige za brisanje: ");
    scanf("%d", &id);

    static int a = 100;
    KNJIGA* knjige = (KNJIGA*)calloc(a, sizeof(KNJIGA));
    if (knjige == NULL) {
        perror("Error alociranja memorije");
        fclose(fp);
        return;
    }

    int brojKnjiga = 0;
    while (fread(&knjige[brojKnjiga], sizeof(KNJIGA), 1, fp)) {
        brojKnjiga++;
    }
    fclose(fp);

    fp = fopen("knjiznica_temp.bin", "wb");
    if (fp == NULL) {
        perror("Greska pri stvaranju privremene datoteke.\n");
        free(knjige);
        return;
    }

    int pronadenaKnjiga = 0;
    for (int i = 0; i < brojKnjiga; i++) {
        if (knjige[i].id == id) {
            pronadenaKnjiga = 1;
            continue;
        }
        fwrite(&knjige[i], sizeof(KNJIGA), 1, fp);
    }
    fclose(fp);

    if (!pronadenaKnjiga) {
        printf("Knjiga s ID-om %d nije pronadjena.\n", id);
    }
    else {
        printf("Knjiga je uspjesno obrisana.\n");
    }

    free(knjige);

    remove("knjiznica.bin");
    rename("knjiznica_temp.bin", "knjiznica.bin");
}

void popisKnjiga() {
    FILE* fp = fopen("knjiznica.bin", "rb");
    if (fp == NULL) {
        perror("Error: Neuspjesno otvaranje datoteke.");
        return;
    }
    
    static int a = 100;
    KNJIGA* knjige = (KNJIGA*)calloc(a, sizeof(KNJIGA));
    if (knjige == NULL) {
        perror("Error alociranja memorije");
        fclose(fp);
        return;
    }

    int brojKnjiga = 0;
    while (fread(&knjige[brojKnjiga], sizeof(KNJIGA), 1, fp)) {
        brojKnjiga++;
    }
    fclose(fp);

    if (brojKnjiga == 0) {
        printf("Nema knjiga u knjiznici.\n");
        free(knjige);
        return;
    }

    for (int i = 0; i < brojKnjiga; i++) {
        printf("\nID: %d\n", knjige[i].id);
        printf("Naslov: %s\n", knjige[i].naslov);
        printf("Zanr: %s\n", knjige[i].zanr);
        printf("Godina izdanja: %d\n", knjige[i].godina);
        printf("Autor: %s\n", knjige[i].autor);
        printf("Posudena: %s\n", knjige[i].posudena ? "Da" : "Ne");
        if (knjige[i].posudena) {
            printf("Korisnik: %s\n", knjige[i].korisnik);
        }
    }

    free(knjige);
}

void pretrazivanjeKnjiga() {
    FILE* fp = fopen("knjiznica.bin", "rb");
    if (fp == NULL) {
        perror("Error: Neuspjesno otvaranje datoteke.");
        return;
    }

    static int a = 100;
    KNJIGA* knjige = (KNJIGA*)calloc(a, sizeof(KNJIGA));
    if (knjige == NULL) {
        perror("Error alociranja memorije");
        fclose(fp);
        return;
    }

    int brojKnjiga = 0;
    while (fread(&knjige[brojKnjiga], sizeof(KNJIGA), 1, fp)) {
        brojKnjiga++;
    }
    fclose(fp);

    if (brojKnjiga == 0) {
        printf("Nema knjiga u knjiznici.\n");
        free(knjige);
        return;
    }

    char pojam[50];
    printf("Unesite pojam za pretragu (Naslov, autor ili zanr) : ");
    clearInputBuffer();
    fgets(pojam, sizeof(pojam), stdin);
    pojam[strcspn(pojam, "\n")] = 0;

    int pronadeno = 0;
    for (int i = 0; i < brojKnjiga; i++) {
        if (strstr(knjige[i].naslov, pojam) || strstr(knjige[i].zanr, pojam) || strstr(knjige[i].autor, pojam)) {
            printf("\nID: %d\n", knjige[i].id);
            printf("Naslov: %s\n", knjige[i].naslov);
            printf("Zanr: %s\n", knjige[i].zanr);
            printf("Godina izdanja: %d\n", knjige[i].godina);
            printf("Autor: %s\n", knjige[i].autor);
            printf("Posudena: %s\n", knjige[i].posudena ? "Da" : "Ne");
            if (knjige[i].posudena) {
                printf("Korisnik: %s\n", knjige[i].korisnik);
            }
            pronadeno = 1;
        }
    }

    if (!pronadeno) {
        printf("Nema knjiga koje zadovoljavaju pretragu.\n");
    }

    free(knjige);
}

void sortirajKnjigePoNaslovu() {
    FILE* fp = fopen("knjiznica.bin", "rb");
    if (fp == NULL) {
        perror("Error: Neuspjesno otvaranje datoteke.");
        return;
    }

    static int a = 100;
    KNJIGA* knjige = (KNJIGA*)calloc(a, sizeof(KNJIGA));
    if (knjige == NULL) {
        perror("Error alociranja memorije");
        fclose(fp);
        return;
    }

    int brojKnjiga = 0;
    while (fread(&knjige[brojKnjiga], sizeof(KNJIGA), 1, fp)) {
        brojKnjiga++;
    }
    fclose(fp);

    if (brojKnjiga == 0) {
        printf("Nema knjiga u knjiznici.\n");
        free(knjige);
        return;
    }

    for (int i = 0; i < brojKnjiga - 1; i++) {
        for (int j = i + 1; j < brojKnjiga; j++) {
            if (strcmp(knjige[i].naslov, knjige[j].naslov) > 0) {
                KNJIGA temp = knjige[i];
                knjige[i] = knjige[j];
                knjige[j] = temp;
            }
        }
    }

    fp = fopen("knjiznica.bin", "wb");
    if (fp == NULL) {
        perror("Error: Neuspjesno otvaranje datoteke.");
        free(knjige);
        return;
    }

    for (int i = 0; i < brojKnjiga; i++) {
        fwrite(&knjige[i], sizeof(KNJIGA), 1, fp);
    }
    fclose(fp);

    printf("Knjige su uspjesno sortirane.\n");
    free(knjige);
}

void posudbaKnjige() {
    FILE* fp = fopen("knjiznica.bin", "rb+");
    if (fp == NULL) {
        perror("Error: Neuspjesno otvaranje datoteke.");
        return;
    }

    int id;
    char korisnik[50];
    printf("Unesite ID knjige za posudbu: ");
    scanf("%d", &id);
    clearInputBuffer();
    printf("Unesite vase korisnicko ime: ");
    fgets(korisnik, sizeof(korisnik), stdin);
    korisnik[strcspn(korisnik, "\n")] = 0;

    KNJIGA knjiga;
    int found = 0;
    while (fread(&knjiga, sizeof(KNJIGA), 1, fp)) {
        if (knjiga.id == id) {
            if (knjiga.posudena) {
                printf("Knjiga je vec posudena.\n");
            }
            else {
                knjiga.posudena = 1;
                strcpy(knjiga.korisnik, korisnik);
                fseek(fp, -(long)sizeof(KNJIGA), SEEK_CUR);
                fwrite(&knjiga, sizeof(KNJIGA), 1, fp);
                printf("Knjiga je uspjesno posudena.\n");
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Knjiga s ID-om %d nije pronadjena.\n", id);
    }

    fclose(fp);
}

void vracanjeKnjige() {
    FILE* fp = fopen("knjiznica.bin", "rb+");
    if (fp == NULL) {
        perror("Error: Neuspjesno otvaranje datoteke.");
        return;
    }

    int id;
    printf("Unesite ID knjige za vracanje: ");
    scanf("%d", &id);
    clearInputBuffer();

    KNJIGA knjiga;
    int found = 0;
    while (fread(&knjiga, sizeof(KNJIGA), 1, fp)) {
        if (knjiga.id == id) {
            if (!knjiga.posudena) {
                printf("Knjiga nije posudena.\n");
            }
            else {
                knjiga.posudena = 0;
                memset(knjiga.korisnik, 0, sizeof(knjiga.korisnik));
                fseek(fp, -(long)sizeof(KNJIGA), SEEK_CUR);
                fwrite(&knjiga, sizeof(KNJIGA), 1, fp);
                printf("Knjiga je uspjesno vracena.\n");
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Knjiga s ID-om %d nije pronadjena.\n", id);
    }

    fclose(fp);
}

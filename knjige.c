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
        perror("Greška pri otvaranju datoteke.");
        return;
    }

    static int a = 100;
    KNJIGA* knjige = (KNJIGA*)calloc(a, sizeof(KNJIGA));
    if (knjige == NULL) {
        perror("Greška alociranja memorije.");
        fclose(fp);
        return;
    }

    int brojKnjiga = 0;
    while (fread(&knjige[brojKnjiga], sizeof(KNJIGA), 1, fp)) {
        brojKnjiga++;
    }
    fclose(fp);

    if (brojKnjiga == 0) {
        printf("Nema knjiga u knjižnici.\n");
        free(knjige);
        return;
    }

    int opcija;
    printf("Odaberite kriterij pretrage:\n");
    printf("1. Pretrazi po naslovu\n");
    printf("2. Pretrazi po autoru\n");
    printf("3. Pretrazi po žanru\n");
    printf("4. Pretrazi po ID-u\n");
    if (scanf("%d", &opcija) != 1 || opcija < 1 || opcija > 4) {
        printf("\nNeispravan odabir. Prekid pretrage.\n");
        free(knjige);
        return;
    }
    getchar(); // uhvati novi red nakon unosa opcije

    char pojam[100];
    KNJIGA kljuc;

    switch (opcija) {
    case 1: // pretraziPoNaslovu
        printf("\nUnesite naslov knjige za pretragu: ");
        fgets(pojam, sizeof(pojam), stdin);
        pojam[strcspn(pojam, "\n")] = '\0';

        strcpy(kljuc.naslov, pojam);
        KNJIGA* rezultatNaslov = (KNJIGA*)bsearch(&kljuc, knjige, brojKnjiga, sizeof(KNJIGA), compareByTitle);

        if (rezultatNaslov != NULL) {
            printf("\nKnjiga je uspješno pronađena.\n");
            printf("\nID: %d\n", rezultatNaslov->id);
            printf("Naslov: %s\n", rezultatNaslov->naslov);
            printf("Zanr: %s\n", rezultatNaslov->zanr);
            printf("Godina izdanja: %d\n", rezultatNaslov->godina);
            printf("Autor: %s\n", rezultatNaslov->autor);
            printf("Posudena: %s\n", rezultatNaslov->posudena ? "Da" : "Ne");
            if (rezultatNaslov->posudena) {
                printf("Korisnik: %s\n", rezultatNaslov->korisnik);
            }
            printf("\n");
        } else {
            printf("\nKnjiga nije pronađena.\n");
        }
        break;
    case 2: // pretraziPoAutoru
        printf("\nUnesite ime autora za pretragu: ");
        fgets(pojam, sizeof(pojam), stdin);
        pojam[strcspn(pojam, "\n")] = '\0';

        strcpy(kljuc.autor, pojam);
        KNJIGA* rezultatAutor = (KNJIGA*)bsearch(&kljuc, knjige, brojKnjiga, sizeof(KNJIGA), compareByAuthor);

        if (rezultatAutor != NULL) {
            printf("\nKnjiga je uspješno pronađena.\n");
            printf("\nID: %d\n", rezultatAutor->id);
            printf("Naslov: %s\n", rezultatAutor->naslov);
            printf("Zanr: %s\n", rezultatAutor->zanr);
            printf("Godina izdanja: %d\n", rezultatAutor->godina);
            printf("Autor: %s\n", rezultatAutor->autor);
            printf("Posudena: %s\n", rezultatAutor->posudena ? "Da" : "Ne");
            if (rezultatAutor->posudena) {
                printf("Korisnik: %s\n", rezultatAutor->korisnik);
            }
            printf("\n");
        } else {
            printf("\nKnjiga nije pronađena.\n");
        }
        break;
    case 3: // pretraziPoZanru
        printf("\nUnesite žanr knjige za pretragu: ");
        fgets(pojam, sizeof(pojam), stdin);
        pojam[strcspn(pojam, "\n")] = '\0';

        strcpy(kljuc.zanr, pojam);
        KNJIGA* rezultatZanr = (KNJIGA*)bsearch(&kljuc, knjige, brojKnjiga, sizeof(KNJIGA), compareByGenre);

        if (rezultatZanr != NULL) {
            printf("\nKnjiga je uspješno pronađena.\n");
            printf("\nID: %d\n", rezultatZanr->id);
            printf("Naslov: %s\n", rezultatZanr->naslov);
            printf("Zanr: %s\n", rezultatZanr->zanr);
            printf("Godina izdanja: %d\n", rezultatZanr->godina);
            printf("Autor: %s\n", rezultatZanr->autor);
            printf("Posudena: %s\n", rezultatZanr->posudena ? "Da" : "Ne");
            if (rezultatZanr->posudena) {
                printf("Korisnik: %s\n", rezultatZanr->korisnik);
            }
            printf("\n");
        } else {
            printf("\nKnjiga nije pronađena.\n");
        }
        break;
    case 4: // pretraziPoId
        printf("\nUnesite ID knjige za pretragu: ");
        int id;
        if (scanf("%d", &id) != 1) {
            printf("Neispravan unos ID-a.\n");
            free(knjige);
            return;
        }

        kljuc.id = id;
        KNJIGA* rezultatId = (KNJIGA*)bsearch(&kljuc, knjige, brojKnjiga, sizeof(KNJIGA), compareById);

        if (rezultatId != NULL) {
            printf("\nKnjiga je uspješno pronađena.\n");
            printf("\nID: %d\n", rezultatId->id);
            printf("Naslov: %s\n", rezultatId->naslov);
            printf("Zanr: %s\n", rezultatId->zanr);
            printf("Godina izdanja: %d\n", rezultatId->godina);
            printf("Autor: %s\n", rezultatId->autor);
            printf("Posudena: %s\n", rezultatId->posudena ? "Da" : "Ne");
            if (rezultatId->posudena) {
                printf("Korisnik: %s\n", rezultatId->korisnik);
            }
            printf("\n");
        } else {
            printf("\nKnjiga nije pronađena.\n");
        }
        break;
    default:
        printf("\nNepoznata opcija. Prekid pretrage.\n");
        break;
    }

    free(knjige);
}

void sortirajKnjige(int kriterij) {
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
        printf("Nema knjiga u knjižnici.\n");
        free(knjige);
        return;
    }

    switch (kriterij) {
    case 1:
        qsort(knjige, brojKnjiga, sizeof(KNJIGA), compareByTitle);
        break;
    case 2:
        qsort(knjige, brojKnjiga, sizeof(KNJIGA), compareById);
        break;
    case 3:
        qsort(knjige, brojKnjiga, sizeof(KNJIGA), compareByAuthor);
        break;
    case 4:
        qsort(knjige, brojKnjiga, sizeof(KNJIGA), compareByYear);
        break;
    case 5:
        qsort(knjige, brojKnjiga, sizeof(KNJIGA), compareByGenre);
        break;
    default:
        printf("Nepoznata opcija. Knjige nisu sortirane.\n");
        free(knjige);
        return;
    }

    fp = fopen("knjiznica.bin", "wb");
    if (fp == NULL) {
        perror("Error: Neuspjesno otvaranje datoteke.");
        free(knjige);
        return;
    }

    fwrite(knjige, sizeof(KNJIGA), brojKnjiga, fp);
    fclose(fp);

    free(knjige);
    printf("Knjige su uspješno sortirane.\n");
}

inline int compareByTitle(const void* a, const void* b) {
    return strcmp(((KNJIGA*)a)->naslov, ((KNJIGA*)b)->naslov);
}

inline int compareById(const void* a, const void* b) {
    return ((KNJIGA*)a)->id - ((KNJIGA*)b)->id;
}

inline int compareByAuthor(const void* a, const void* b) {
    return strcmp(((KNJIGA*)a)->autor, ((KNJIGA*)b)->autor);
}

inline int compareByYear(const void* a, const void* b) {
    return ((KNJIGA*)b)->godina - ((KNJIGA*)a)->godina;
}

inline int compareByGenre(const void* a, const void* b) {
    return strcmp(((KNJIGA*)a)->zanr, ((KNJIGA*)b)->zanr);
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
    char korisnik[50];
    printf("Unesite ID knjige za vracanje: ");
    scanf("%d", &id);
    clearInputBuffer();
    printf("Unesite vase korisnicko ime: ");
    fgets(korisnik, sizeof(korisnik), stdin);
    korisnik[strcspn(korisnik, "\n")] = 0;

    KNJIGA knjiga;
    int found = 0;
    while (fread(&knjiga, sizeof(KNJIGA), 1, fp)) {
        if (knjiga.id == id) {
            if (!knjiga.posudena) {
                printf("Knjiga nije posudena.\n");
            }
            else if (strcmp(knjiga.korisnik, korisnik) != 0) {
                printf("Nemate ovlasti za vracanje ove knjige.\n");
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

void azurirajKnjigu() {
    FILE* fp = fopen("knjiznica.bin", "rb+");
    if (fp == NULL) {
        perror("Error: Neuspjesno otvaranje datoteke.");
        return;
    }

    int id;
    printf("Unesite ID knjige za azuriranje: ");
    scanf("%d", &id);
    clearInputBuffer();

    KNJIGA knjiga;
    int found = 0;
    long pos;
    while ((pos = ftell(fp)) != -1L && fread(&knjiga, sizeof(KNJIGA), 1, fp)) {
        if (knjiga.id == id) {
            found = 1;
            printf("Knjiga pronadjena.\n");
            printf("Unesite novi naslov knjige (trenutno: %s): ", knjiga.naslov);
            fgets(knjiga.naslov, sizeof(knjiga.naslov), stdin);
            knjiga.naslov[strcspn(knjiga.naslov, "\n")] = 0;

            printf("Unesite novi zanr knjige (trenutno: %s): ", knjiga.zanr);
            fgets(knjiga.zanr, sizeof(knjiga.zanr), stdin);
            knjiga.zanr[strcspn(knjiga.zanr, "\n")] = 0;

            printf("Unesite novu godinu izdanja (trenutno: %d): ", knjiga.godina);
            scanf("%d", &knjiga.godina);
            clearInputBuffer();

            printf("Unesite novog autora knjige (trenutno: %s): ", knjiga.autor);
            fgets(knjiga.autor, sizeof(knjiga.autor), stdin);
            knjiga.autor[strcspn(knjiga.autor, "\n")] = 0;

            fseek(fp, pos, SEEK_SET);
            fwrite(&knjiga, sizeof(KNJIGA), 1, fp);
            printf("Knjiga je uspjesno azurirana.\n");
            break;
        }
    }

    if (!found) {
        printf("Knjiga s ID-om %d nije pronadjena.\n", id);
    }

    fclose(fp);
}
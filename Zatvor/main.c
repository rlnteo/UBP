#include <stdio.h>
#include <stdlib.h>

#include "operacije_nad_datotekom.h"
int main()
{
    int running = 1;
    int userInput;

    FILE *fajl = NULL;

    while (running)
    {
        printf("Odaberite opciju:\n");
        printf("1 - Otvaranje datoteke\n");
        printf("2 - Formiranje datoteke\n");
        printf("3 - Pretraga datoteke\n");
        printf("4 - Unos sloga\n");
        printf("5 - Ispis svih slogova\n");
        printf("6 - Azuriranje sloga\n");
        printf("7 - Brisanje sloga (logicko)\n");
        printf("0 - Izlaz\n");

        if (fajl == NULL)
        {
            printf("!!!PAZNJA: datoteka jos uvek nije otvorena!!!\n");
        }

        scanf("%d", &userInput);
        getc(stdin); //

        switch (userInput)
        {
        case 1:
        {
            char filename[20];
            printf("Unesite ime datoteke za otvaranje: ");
            scanf("%s", &filename[0]);
            fajl = otvoriDatoteku(filename);
            printf("\n");
            break;
        }
        case 2:
        {
            char filename[20];
            printf("Unesite ime datoteke za kreiranje: ");
            scanf("%s", filename);
            kreirajDatoteku(filename);
            printf("\n");
            break;
        }
        case 3:
        {
            // to ti je primarni kljuc - po njemu mora da se vrsi pretraga
            char evidBroj[8 + 1];
            scanf("%s", evidBroj);
            SLOG *slog = pronadjiSlog(fajl, evidBroj);
            if (slog == NULL)
            {
                printf("Nema tog sloga u datoteci.\n");
            }
            else
            {
                printf("Evid.Br     Sifra Zatvorenika    Dat.Vrem.Dol   Celija   Kazna\n");
                ispisiSlog(slog);
                printf("\n");
            }
            break;
        }
        case 4:
        {
            SLOG slog;
            printf("Evid. broj(8 cifara): ");
            scanf("%s", slog.evidBroj);
            printf("Sifra zatvorenika (7 karaktera): ");
            scanf("%s", slog.sifraZatvorenika);
            printf("Datum i vreme dolaska (dd-mm-yyyy HH:mm): ");
            scanf("%d-%d-%d %d:%d",
                  &slog.datumVremeDolaska.dan,
                  &slog.datumVremeDolaska.mesec,
                  &slog.datumVremeDolaska.godina,
                  &slog.datumVremeDolaska.sati,
                  &slog.datumVremeDolaska.minuti);
            printf("Oznaka celije (tacno 5 karaktera): ");
            scanf("%s", slog.oznakaCelije);
            printf("Duzina kazne (do 480 meseci): ");
            scanf("%d", &slog.duzinaKazne);
            slog.deleted = 0;
            dodajSlog(fajl, &slog);
            printf("\n");
            break;
        }
        case 5:
        {
            ispisiSveSlogove(fajl);
            printf("\n");
            break;
        }
        case 6:
        {
            char evidBroj[8 + 1];
            printf("Unesite evid. broj sloga koji azurirate: ");
            scanf("%s", evidBroj);
            char oznalaCelije[5 + 1];
            printf("Unesite novu oznaku celije: ");
            scanf("%s", oznalaCelije);
            int duzinaKazne;
            printf("Unesite novu duzinu kazne: ");
            scanf("%d", &duzinaKazne);
            azurirajSlog(fajl, evidBroj, oznalaCelije, duzinaKazne);
            printf("\n");
            break;
        }
        case 7:
        {
            char evidBroj[8 + 1];
            printf("Unesite evid. broj sloga za logicko brisanje: ");
            scanf("%s", evidBroj);
            obrisiSlogLogicki(fajl, evidBroj);
            printf("\n");
            break;
        }
        case 0:
        {
            running = 0;
            if (fajl != NULL)
            {
                fclose(fajl);
            }
        }
        }
    }

    return 0;
}

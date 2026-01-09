#include "operacije_nad_datotekom.h"

FILE *otvoriDatoteku(char *filename)
{
    // Citaj i pisi u binarnu datoteku
    FILE *fajl = fopen(filename, "rb+");
    // Provera da li je uspesno otvoreno
    if (fajl == NULL)
    {
        printf("Doslo je do greske! Moguce da datoteka \"%s\" ne postoji.\n", filename);
    }
    else
    {
        printf("Datoteka \%s\ otvorena!", filename);
    }
    return fajl;
}

void kreirajDatoteku(char *filename)
{
    // PISI BINARNO
    // Ukoliko ne postoji ta datoteka, on ce da je kreira
    FILE *fajl = fopen(filename, "wb");
    if (fajl == NULL)
    {
        printf("Doslo je do greske prilikom kreiranja datoteke \"%s\"!\n", filename);
    }
    else
    {
        // Upisi pocetni blok
        // MORAMO u nasoj datoteci da imamo barem jedan blok i barem jedan slog - to je onaj za kraj
        BLOK blok;
        // U taj blok na nulto mesto upisujemo umesto broja za evidenciju, oznaku za kraj datoteke
        strcpy(blok.slogovi[0].evidBroj, OZNAKA_KRAJA_DATOTEKE);
        // Ovaj blok, ove velicine, 1 blok u taj fajl da mi upises
        fwrite(&blok, sizeof(BLOK), 1, fajl);
        printf("Datoteka \"%s\" uspesno kreirana.\n", filename);
        fclose(fajl);
    }
}

SLOG *pronadjiSlog(FILE *fajl, char *evidBroj)
{
    if (fajl == NULL)
    {
        return NULL;
    }

    // Nas pokazivac se nalazi ispred prvog bloka
    fseek(fajl, 0, SEEK_SET);
    BLOK blok;

    // Citaj mi blok po blok
    while (fread(&blok, sizeof(BLOK), 1, fajl))
    {
        for (int i = 0; i < FAKTOR_BLOKIRANJA; i++)
        {
            // Poredimo dva niza karaktera
            if (strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0)
            {
                // Nema vise slogova
                return NULL;
            }
            if (strcmp(blok.slogovi[i].evidBroj, evidBroj) == 0)
            {
                // Ako se evidBroj poklapa i slog NIJE logicki obrisan
                //(logcki obrisane slogove tretiramo kao da i ne postoje u datoteci)
                SLOG *slog = (SLOG *)malloc(sizeof(SLOG));
                memcpy(slog, &blok.slogovi[i], sizeof(SLOG));
                return slog;
            }
        }
    }
    return NULL;
}

void dodajSlog(FILE *fajl, SLOG *slog)
{
    if (fajl == NULL)
    {
        printf("Datoteka nije otvorena!\n");
        return;
    }

    // NE SMEMO DA IMAMO SLOG SA ISTIM BROJEM EVIDENCIJE
    SLOG *slogStari = pronadjiSlog(fajl, slog->evidBroj);
    if (slogStari != NULL)
    {
        // U datoteci vec postoji slog sa tim evid. brojem pa ne mozemo upisati novi slog
        printf("Vec postoji slog sa tim evid brojem!\n");
        return;
    }

    BLOK blok;
    fseek(fajl, -sizeof(BLOK), SEEK_END); // U poslednji blok upisujemo novi slog
    fread(&blok, sizeof(BLOK), 1, fajl);

    int i;
    for (i = 0; i < FAKTOR_BLOKIRANJA; i++)
    {
        if (strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0)
        {
            // Ovo je mesto gde se nalazi slog sa oznakom kraja datoteke
            // Tu treba upisati novi slog
            memcpy(&blok.slogovi[i], slog, sizeof(SLOG));
            break;
        }
    }
    i++; // Na to mesto u bloku cemo upisati krajDatoteke

    if (i < FAKTOR_BLOKIRANJA)
    {
        // Jos uvek ima mesta u ovom bloku, mozemo tu smestiti
        // slog sa oznakom kraja datoteke.
        strcpy(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE);

        // Sada blok mozemo vratiti u datoteku
        fseek(fajl, -sizeof(BLOK), SEEK_CUR);
        fwrite(&blok, sizeof(BLOK), 1, fajl);
    }
    else
    {
        // Nema vise mesta u tom bloku, tako da moramo praviti novi blok
        // u koji cemo upisati slog sa oznakom kraja datoteke

        // Prvo moramo vratiti u datoteku blok koji smo popunili
        BLOK noviBlok;
        strcpy(noviBlok.slogovi[0].evidBroj, OZNAKA_KRAJA_DATOTEKE);
        // Vec smo na kraju datoteke, nema potrebe za fseek-om
        fwrite(&noviBlok, sizeof(BLOK), 1, fajl);
    }

    if (ferror(fajl))
    {
        printf("Greska pri upisu u fajl!\n");
    }
    else
    {
        printf("Upis novog sloga zavrsen.\n");
    }
}

void ispisiSveSlogove(FILE *fajl)
{
    if (fajl == NULL)
    {
        printf("Datoteka nije otvorena!\n");
        return;
    }

    fseek(fajl, 0, SEEK_SET);
    BLOK blok;
    int rbBloka = 0;
    printf("BL SL Evid.Br   Sif.Zat      Dat.Vrem.Dol    Celija    Kazna\n ");
    while (fread(&blok, sizeof(BLOK), 1, fajl))
    {
        for (int i = 0; i < FAKTOR_BLOKIRANJA; i++)
        {
            if (strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0)
            {
                printf("B%d S%d *\n", rbBloka, i);
                break; // Citaj sledeci blok
            }
            if (!blok.slogovi[i].deleted)
            {
                printf("B%d S%d", rbBloka, i);
                ispisiSlog(&blok.slogovi[i]);
                printf("\n");
            }
        }
        rbBloka++;
    }
}

void ispisiSlog(SLOG *slog)
{
    printf("%8s  %7s   %02d-%02d-%4d  %02d:%02d  %7s  %6d",
           slog->evidBroj,
           slog->sifraZatvorenika,
           slog->datumVremeDolaska.dan,
           slog->datumVremeDolaska.mesec,
           slog->datumVremeDolaska.godina,
           slog->datumVremeDolaska.sati,
           slog->datumVremeDolaska.minuti,
           slog->oznakaCelije,
           slog->duzinaKazne);
}

void azurirajSlog(FILE *fajl, char *evidBroj, char *oznakaCelije, int duzinaKazne)
{
    if (fajl == NULL)
    {
        printf("Datoteka nije otvorena!\n");
        return;
    }

    fseek(fajl, 0, SEEK_SET);
    BLOK blok;
    while (fread(&blok, sizeof(BLOK), 1, fajl))
    {
        for (int i = 0; i < FAKTOR_BLOKIRANJA; i++)
        {
            if (strcmp(blok.slogovi[i].evidBroj, evidBroj) == 0 && !blok.slogovi[i].deleted)
            {
                // To je trazeni slog(koji zelimo da modifikujemo) i on
                // NIJE logicki obrisan -> mozemo ga modifikovati
                // primetimo da , ako nadjemo slog koji ima odgovarajuci evidBroj
                // ali je logicki obrisan, on nas ne interesuje, nastavljamo pretragu

                // azuriraj oznaku celije i duzinu kazne
                strcpy(blok.slogovi[i].oznakaCelije, oznakaCelije);
                blok.slogovi[i].duzinaKazne = duzinaKazne;

                fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                fwrite(&blok, sizeof(BLOK), 1, fajl);

                printf("Slog izmenjen.\n");
                return;
            }
        }
    }
}

void obrisiSlogLogicki(FILE *fajl, char *evidBroj)
{
    if (fajl == NULL)
    {
        printf("Datoteka nije otvorena.\n");
        return;
    }

    fseek(fajl, 0, SEEK_SET);
    BLOK blok;
    while (fread(&blok, sizeof(BLOK), 1, fajl))
    {
        for (int i = 0; i < FAKTOR_BLOKIRANJA; i++)
        {
            if (strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0)
            {
                printf("Nema tog sloga u datoteci.\n");
                return;
            }

            if (strcmp(blok.slogovi[i].evidBroj, evidBroj) == 0 && !blok.slogovi[i].deleted)
            {
                blok.slogovi[i].deleted = 1;
                fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                fwrite(&blok, sizeof(BLOK), 1, fajl);

                printf("Slogje logicki obrisan.\n");
                return;
            }
        }
    }
}

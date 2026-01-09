#ifndef STRUKTURA_PODATAKA_H_INCLUDED
#define STRUKTURA_PODATAKA_H_INCLUDED

#define FAKTOR_BLOKIRANJA 3
#define OZNAKA_KRAJA_DATOTEKE "********"

typedef struct DateTime
{
    int dan;
    int mesec;
    int godina;
    int sati;
    int minuti;
} DATETIME;

// svaki slog predstavlja JEDAN dolazak
typedef struct Slog
{
    char evidBroj[8 + 1]; // koristi se kao kljuc sloga
    char sifraZatvorenika[7 + 1];
    DATETIME datumVremeDolaska;
    char oznakaCelije[5 + 1]; // Ovo + 1 je za kraj tj za \0
    int duzinaKazne;
    int deleted;
} SLOG;

// LISTA SLOGOVA
typedef struct Blok
{
    SLOG slogovi[FAKTOR_BLOKIRANJA];
} BLOK;

#endif // STRUKTURA_PODATAKA_H_INCLUDED

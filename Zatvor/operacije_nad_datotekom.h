#ifndef OPERACIJE_NAD_DATOTEKOM_H_INCLUDED
#define OPERACIJE_NAD_DATOTEKOM_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>

#include "struktura_podataka.h"

FILE *otvoriDatoteku(char *filename);
void kreirajDatoteku(char *filename);
SLOG *pronadjiSlog(FILE *fajl, char *evidBroj);
void dodajSlog(FILE *fajl, SLOG *slog);
void ispisiSveSlogove(FILE *fajl);
void ispisiSlog(SLOG *slog);
void azurirajSlog(FILE *fajl, char *evidBroj, char *oznakaCelije, int duzinaKazne);
void obrisiSlogLogicki(FILE *fajl, char *evidBroj);

#endif // OPERACIJE_NAD_DATOTEKOM_H_INCLUDED

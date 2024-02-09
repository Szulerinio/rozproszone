#ifndef WATEK_KOMUNIKACYJNY_H
#define WATEK_KOMUNIKACYJNY_H

#include "lamportTable.h"
/* wątek komunikacyjny: odbieranie wiadomości i reagowanie na nie poprzez zmiany
 * stanu */
void *startKomWatek(void *ptr);

#endif

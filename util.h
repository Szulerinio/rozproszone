#ifndef UTILH
#define UTILH
#include "main.h"

extern pthread_mutex_t mutex;

/* typ pakietu */
typedef struct {
  int ts; /* timestamp (zegar lamporta */
  int src;
  int data; /* przykładowe pole z danymi; można zmienić nazwę na bardziej
               pasującą */
  int isGoingUp;

} packet_t;
/* packet_t ma trzy pola, więc NITEMS=3. Wykorzystane w inicjuj_typ_pakietu */
#define NITEMS 4

/* Typy wiadomości */
#define REQUEST 1
#define ACK 2
#define RELEASE 3
#define TAKEN 4

extern MPI_Datatype MPI_PAKIET_T;
void inicjuj_typ_pakietu();

/* wysyłanie pakietu, skrót: wskaźnik do pakietu (0 oznacza stwórz pusty
 * pakiet), do kogo, z jakim typem */
void sendPacket(int destination, int tag, int data, int isGoingUp);

extern pthread_mutex_t mutex;
#endif

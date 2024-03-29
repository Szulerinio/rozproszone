#include "watek_komunikacyjny.h"
#include "lamportTable.h"
#include "main.h"
#include "queue.h"
#include "util.h"

/* wątek komunikacyjny; zajmuje się odbiorem i reakcją na komunikaty */
void *startKomWatek(void *ptr) {
  MPI_Status status;
  packet_t pakiet;

  while (stan != InFinish) {
    MPI_Recv(&pakiet, 1, MPI_PAKIET_T, MPI_ANY_SOURCE, MPI_ANY_TAG,
             MPI_COMM_WORLD, &status);

    int recivedTimestamp = pakiet.ts;
    int pakietSrc = pakiet.src;

    pthread_mutex_lock(&mutex);
    timestamp =
        recivedTimestamp > timestamp ? recivedTimestamp + 1 : timestamp + 1;
    addToLamportTable(pakiet.src, pakiet.ts);
    pthread_mutex_unlock(&mutex);

    switch (status.MPI_TAG) {
    case REQUEST:
      debug("Dostałem request od %d z ts=%d", pakiet.src, pakiet.ts);
      pthread_mutex_lock(&mutex);
      sendPacket(pakiet.src, ACK, 0, 0);

      addToQueue(pakiet.src, pakiet.ts, pakiet.data, pakiet.isGoingUp);

      pthread_mutex_unlock(&mutex);

      break;

    case TAKEN:
      debug("Dostałem TAKEN od %d z ts=%d i data=%d", pakiet.src, pakiet.ts,
            pakiet.data);

      // dodaję do listy lamporta proces który wszedł do windy z timestampem od
      // menagera
      addToLamportTable(pakiet.data, pakiet.ts);
      if (isCriticalOccupied == 0) {
        printQueue();
      }

      pthread_mutex_lock(&mutex);
      isCriticalOccupied = 1;
      if (pakiet.data == rank) {
        // we are in the lift
        stan = InLift;
      }

      removeFromQueue(pakiet.data);
      pthread_mutex_unlock(&mutex);

      break;

    case ACK:
      // debug("Dostałem ACK od %d z ts=%d", pakiet.src, pakiet.ts);

      pthread_mutex_lock(&mutex);

      pthread_mutex_unlock(&mutex);

      break;
    case RELEASE:

      pthread_mutex_lock(&mutex);

      isCriticalOccupied = 0;
      if (stan == InLift) {
        if (iAmAtBottomFloor == 0) {
          iAmAtBottomFloor = 1;
          stan = InRun;
        } else {
          iAmAtBottomFloor = 0;
          stan = AtTopFloor;
        }

      } else {
      }
      pthread_mutex_unlock(&mutex);

      break;
    default:
      break;
    }
  }
}

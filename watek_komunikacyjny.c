#include "watek_komunikacyjny.h"
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
      sendPacket(pakiet.src, ACK, 0);

      addToQueue(pakiet.src, pakiet.ts, pakiet.data);
      printQueue();
      // if (lamportTable.size == queue.capacity - 1) {
      if (queue.size == queue.capacity - 1) {
        // simple example - weight 1. Later should sum up all weight
        int ourPosition = getPosition(rank);
        if (ourPosition == liftCapacity) {
          //  I am the manager of the lift
          stan = Manager;
          for (int sendingIndex = 0; sendingIndex < ourPosition;
               sendingIndex++) {
            for (int i = 0; i < size; i++) {
              sendPacket(i, TAKEN, sendingIndex);
            }
          }
        }
      }

      pthread_mutex_unlock(&mutex);

      break;

    case TAKEN:
      debug("Dostałem TAKEN od %d z ts=%d", pakiet.src, pakiet.ts);
      pthread_mutex_lock(&mutex);
      if (pakiet.data == rank) {
        // we are in the lift
        stan = InLiftUp;
      }
      removeFromQueue(pakiet.data);
      pthread_mutex_unlock(&mutex);

      break;

    case ACK:
      debug("Dostałem ACK od %d z ts=%d", pakiet.src, pakiet.ts);

      pthread_mutex_lock(&mutex);

      pthread_mutex_unlock(&mutex);

      break;
    case RELEASE:

      pthread_mutex_lock(&mutex);
      if (stan == InLiftUp) {
        stan = AtTopFloor;
      } else {
      }
      pthread_mutex_unlock(&mutex);

      break;
    default:
      break;
    }
  }
}

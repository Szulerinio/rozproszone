#include "watek_glowny.h"
#include "main.h"
#include "queue.h"
#include "util.h"
#include <unistd.h>

void mainLoop() {
  // srandom(rank);
  int tag;

  while (stan != InFinish) {
    switch (stan) {
    case InRun: {
      pthread_mutex_lock(&mutex);
      stan = WantToQueue;
      pthread_mutex_unlock(&mutex);
      break;
    }
    case WantToQueue: {
      pthread_mutex_lock(&mutex);
      for (int i = 0; i < size; i++) {
        sendPacket(i, REQUEST, currentPackageWeight);
      }
      stan = Waiting;
      pthread_mutex_unlock(&mutex);
      break;
    }
    case Manager: {
      pthread_mutex_lock(&mutex);
      usleep(500000);

      for (int i = 0; i < size; i++) {
        sendPacket(i, RELEASE, currentPackageWeight);
      }
      pthread_mutex_unlock(&mutex);
      break;
    }
    case AtTopFloor: {
      pthread_mutex_lock(&mutex);
      // odkładam paczkę jakiś czas
      usleep(500000);
      // do uzupełnienia
      pthread_mutex_unlock(&mutex);
      break;
    }
    }
  }
}
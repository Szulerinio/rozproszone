#include "watek_glowny.h"
#include "main.h"
#include "queue.h"
#include "util.h"
#include <stdio.h>
#include <unistd.h>

void mainLoop() {
  // srandom(rank);
  int tag;

  while (stan != InFinish) {
    switch (stan) {
    case InRun: {
      pthread_mutex_lock(&mutex);
      stan = WantToQueueToTop;
      pthread_mutex_unlock(&mutex);
      break;
    }
    case WantToQueueToTop: {
      pthread_mutex_lock(&mutex);
      for (int i = 0; i < size; i++) {
        sendPacket(i, REQUEST, currentPackageWeight, 1);
      }
      stan = Waiting;
      pthread_mutex_unlock(&mutex);
      break;
    }

    case Waiting: {
      pthread_mutex_lock(&mutex);
      if (isCriticalOccupied == 0) {
        if (lamportTable.size == lamportTable.capacity) {

          int ourReqHasLowerLamportThanCurrentLamportOfPeopleOutside = 1;
          for (int i = 0; i < size; i++) {
            // check if is not yet in queue
            if (getPosition(i) == -1) {
              // check if we know that their timestamp is higher than our
              // request's - then they cant get in front of us
              if (getReqTimestamp(rank) < getProcessTimestamp(i)) {
                ourReqHasLowerLamportThanCurrentLamportOfPeopleOutside = 0;
                break;
              }
            }
          }
          if (ourReqHasLowerLamportThanCurrentLamportOfPeopleOutside != 0) {
            // simple example - weight 1. Later should sum up all weight
            int ourPosition = getPosition(rank);
            int isLiftGoingUp = queue.elements[0].isGoingUp;
            int managerPosition = liftCapacity;

            for (int i = 0; i < liftCapacity; i++) {
              // someone in front of us is going up
              if (queue.elements[i].isGoingUp != isLiftGoingUp) {
                managerPosition = i;
                break;
              }
            }
            if (managerPosition == ourPosition) {

              stan = Manager;

              for (int sendingIndex = 0; sendingIndex < ourPosition;
                   sendingIndex++) {
                for (int i = 0; i < size; i++) {
                  // if (i != rank) {
                  sendPacket(i, TAKEN, queue.elements[sendingIndex].processId,
                             0);
                  // removeFromQueue(sendingIndex);
                  // }
                }
              }
            }
          }
        }
      }
      pthread_mutex_unlock(&mutex);
      usleep(1000);
      break;
    }

    case Manager: {

      debug("Jestem manger");
      pthread_mutex_lock(&mutex);
      usleep(5000000);

      for (int i = 0; i < size; i++) {
        sendPacket(i, RELEASE, currentPackageWeight, 0);
      }

      debug("nie jestem manger");
      stan = Waiting;
      pthread_mutex_unlock(&mutex);
      break;
    }
    case AtTopFloor: {
      pthread_mutex_lock(&mutex);
      // odkładam paczkę jakiś czas
      usleep(500000);
      stan = WantToQueueToBottom;

      pthread_mutex_unlock(&mutex);
      break;
    }
    case WantToQueueToBottom: {
      pthread_mutex_lock(&mutex);
      for (int i = 0; i < size; i++) {
        sendPacket(i, REQUEST, currentPackageWeight, 0);
      }
      stan = Waiting;
      pthread_mutex_unlock(&mutex);
      break;
    }
    }
  }
}
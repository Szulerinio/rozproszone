#include "queue.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

void initPriorityQueue(int capacity) {
  queue.elements = (QueueElement *)malloc(sizeof(QueueElement) * capacity);
  queue.capacity = capacity;
  queue.size = 0;
}

void addToQueue(int processId, int timestamp, int weight) {
  int toAdd = TRUE;

  // jeśli już takie id procesu istnieje to podmień ts

  if (queue.size < queue.capacity) {
    queue.elements[queue.size].processId = processId;
    queue.elements[queue.size].timestamp = timestamp;
    queue.elements[queue.size].weight = weight;
    queue.size++;
  } else {
    printf("kolejka jest pełna");
  }

  sortPriorityQueue();
}
void removeFromQueue(int processId) {
  if (queue.size > 0) {

    int position = getPosition(processId);
    for (int i = position; i < queue.size - 1; i++) {
      queue.elements[i] = queue.elements[i + 1];
    }
    queue.size--;
  }
}

// TODO: sprawdzić czy potrzebne
//  void clearPriorityQueue() {
//    free(queue.elements);
//    queue.size = 0;
//    queue.elements =
//        (QueueElement *)malloc(sizeof(QueueElement) * queue.capacity);
//  }

void sortPriorityQueue() {
  // Sorting using bubble sort
  for (int i = 0; i < queue.size - 1; i++) {
    for (int j = 0; j < queue.size - i - 1; j++) {
      if ((queue.elements[j].timestamp > queue.elements[j + 1].timestamp) ||
          ((queue.elements[j].timestamp == queue.elements[j + 1].timestamp) &&
           (queue.elements[j].processId > queue.elements[j + 1].processId))) {
        // Swap elements
        QueueElement temp = queue.elements[j];
        queue.elements[j] = queue.elements[j + 1];
        queue.elements[j + 1] = temp;
      }
    }
  }
}

int getPosition(int processId) {
  for (int i = 0; i < queue.size; i++)
    if (queue.elements[i].processId == processId)
      return i;

  return -1;
}

void printQueue() {
  printf("[%d] Queue (size: %d): ", rank, queue.size);
  for (int i = 0; i < queue.size; i++)
    printf("(P: %d, T:%d) ", queue.elements[i].processId,
           queue.elements[i].timestamp);
  printf("\n");
}

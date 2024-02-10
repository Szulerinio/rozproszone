#include "queue.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

void initPriorityQueue(int capacity) {
  queue.elements = (QueueElement *)malloc(sizeof(QueueElement) * capacity);
  queue.capacity = capacity;
  queue.size = 0;
}

void addToQueue(int processId, int timestamp, int weight, int isGoingUp) {

  if (queue.size < queue.capacity) {
    queue.elements[queue.size].processId = processId;
    queue.elements[queue.size].timestamp = timestamp;
    queue.elements[queue.size].weight = weight;
    queue.elements[queue.size].isGoingUp = isGoingUp;
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
    printf("(P: %d, T:%d, D:%d) ", queue.elements[i].processId,
           queue.elements[i].timestamp, queue.elements[i].isGoingUp);
  printf("\n");
}

int getReqTimestamp(int processId) {
  for (int i = 0; i < queue.size; i++)
    if (queue.elements[i].processId == processId)
      return queue.elements[i].timestamp;
  return -1;
}

int getReqDirection(int processId) {
  for (int i = 0; i < queue.size; i++)
    if (queue.elements[i].processId == processId)
      return queue.elements[i].isGoingUp;
  return -1;
}
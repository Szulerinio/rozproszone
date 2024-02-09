#ifndef QUEUE_H
#define QUEUE_H

typedef struct {
  int processId;
  int timestamp;
  int weight;
} QueueElement;

typedef struct {
  QueueElement *elements;
  int capacity;
  int size;
} PriorityQueue;

void initPriorityQueue(int capacity);
void addToQueue(int processId, int timestamp, int weight);
void removeFromQueue(int processId);
void clearPriorityQueue();
void sortPriorityQueue();
int getPosition(int processId);
void printQueue();

#endif // QUEUE_H

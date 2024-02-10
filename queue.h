#ifndef QUEUE_H
#define QUEUE_H

typedef struct {
  int processId;
  int timestamp;
  int weight;
  int isGoingUp;
} QueueElement;

typedef struct {
  QueueElement *elements;
  int capacity;
  int size;
} PriorityQueue;

void initPriorityQueue(int capacity);
void addToQueue(int processId, int timestamp, int weight, int isGoingUp);
void removeFromQueue(int processId);
void clearPriorityQueue();
void sortPriorityQueue();
int getPosition(int processId);
void printQueue();
int getReqTimestamp(int processId);
int getReqDirection(int processId);

#endif // QUEUE_H

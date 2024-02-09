#ifndef LAMPORT_TABLE_H
#define LAMPORT_TABLE_H

typedef struct {
  int processId;
  int timestamp;
} LamportTableElement;

typedef struct {
  LamportTableElement *elements;
  int capacity;
  int size;
} LamportTable;

void initLamportTable(int capacity);
void addToLamportTable(int processId, int timestamp);
int getLamportTablePosition(int processId);
void printLamportTable();

#endif // LAMPORT_TABLE_H
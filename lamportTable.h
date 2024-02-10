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
void addToLamportTable(int processId, int processTimestamp);
int getLamportTablePosition(int processId);
void printLamportTable();
int getProcessTimestamp(int processId);

#endif // LAMPORT_TABLE_H

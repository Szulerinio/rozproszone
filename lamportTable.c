#include "lamportTable.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

void initLamportTable(int capacity) {
  lamportTable.elements =
      (LamportTableElement *)malloc(sizeof(LamportTableElement) * capacity);
  lamportTable.capacity = capacity;
  lamportTable.size = 0;
}

void addToLamportTable(int processId, int processTimestamp) {

  int index = getLamportTablePosition(processId);
  if (index == -1) {
    // not in table yet
    if (lamportTable.size < lamportTable.capacity) {
      lamportTable.elements[lamportTable.size].processId = processId;
      lamportTable.elements[lamportTable.size].timestamp = processTimestamp;

      lamportTable.size++;
    } else {
      printf("błąd niewyjaśniony, magia jakoś proces 2 razy w tabeli");
    }
  } else {
    // already in table
    if (lamportTable.elements[index].timestamp < processTimestamp) {
      lamportTable.elements[index].timestamp = processTimestamp;
    }
  }
}

int getLamportTablePosition(int processId) {
  for (int i = 0; i < lamportTable.size; i++)
    if (lamportTable.elements[i].processId == processId)
      return i;

  return -1;
}

void printLamportTable() {
  printf("[%d] Table (size: %d): ", rank, lamportTable.size);
  for (int i = 0; i < lamportTable.size; i++)
    printf("(P: %d, T:%d) ", lamportTable.elements[i].processId,
           lamportTable.elements[i].timestamp);
  printf("\n");
}

int getProcessTimestamp(int processId) {
  for (int i = 0; i < lamportTable.size; i++)
    if (lamportTable.elements[i].processId == processId)
      return lamportTable.elements[i].timestamp;
  return -1;
}
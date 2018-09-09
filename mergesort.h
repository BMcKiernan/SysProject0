#ifndef MERGESORT_H
#define MERGESORT_H
#include "sorter.h"


void setcol(int i);

int less(char* a, char* b);

record* merge(record* a, record* b);

record* mergesort(record* c);

#endif

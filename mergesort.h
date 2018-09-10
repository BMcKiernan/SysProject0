#ifndef MERGESORT_H
#define MERGESORT_H
#include "ADTList.h"

void set_comparetype(int j);

void setcol(int i);

int less(char* a, char* b);

record* merge(record* a, record* b);

record* mergesort(record* c);

#endif

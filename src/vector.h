#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdlib.h>

typedef struct
{
  long size;
  long capacity;
  void** elements;
} Vector;

Vector* vector_new();
int vector_add(Vector* v, void* element);
int vector_remove(Vector* v, void* element);
void* vector_item(Vector* v, const int index);
void vector_free(Vector* v);

#endif

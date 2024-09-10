#include "vector.h"

Vector* vector_new()
{
  Vector* v = malloc(sizeof(Vector));
  v->capacity = 0;
  v->size = 0;
  v->elements = NULL;
  return v;
}

int vector_add(Vector* v, void* element)
{
  v->size++;

  if(v->size > v->capacity)
  {
    v->capacity = v->size * 2;
    v->elements = realloc(v->elements, v->capacity * sizeof(void*));
  }

  v->elements[v->size - 1] = element;
  return v->size;
}

int vector_remove(Vector* v, void* element)
{
  return 0;
}

void* vector_item(Vector* v, const int index)
{
  if(index >= v->size)
    return NULL;

  return v->elements[index];
}

void vector_free(Vector* v)
{
  free(v->elements);
  free(v);
}

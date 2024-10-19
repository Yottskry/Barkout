/*
*
* Barkout - Copyright 2024 Stephen Branley.
*
* This file is part of Barkout.
*
* Barkout is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License,
* or (at your option) any later version.
*
* Barkout is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Foobar. If not, see <https://www.gnu.org/licenses/>.
*
*/


#include "vector.h"
#include "app.h"

Vector* vector_new()
{
  Vector* v = malloc(sizeof(Vector));
  v->capacity = 0;
  v->size = 0;
  v->elements = NULL;
  return v;
}

bool vector_add(Vector* v, void* element)
{
  v->size++;

  if(v->size > v->capacity)
  {
    v->capacity = v->size * 2;
    v->elements = realloc(v->elements, v->capacity * sizeof(void*));
    TEST_ALLOC(v->elements);

    // Initialise new elements to NULL
    for(int i = v->size-1; i < v->capacity; i++)
    {
      v->elements[i] = NULL;
    }
  }

  v->elements[v->size - 1] = element;
  return true;
}

bool vector_remove(Vector* v, void* element)
{
  for(int i = 0; i < v->size; v++)
  {
    if(v->elements[i] == element)
    {
      v->elements[i] = NULL;
      for(int start = i; start < v->size; start++)
      {
        v->elements[start] = v->elements[start + 1];
        v->elements[start + 1] = NULL;
      }
      v->size--;

      if(v->size < (v->capacity / 4))
      {
        long newcap = v->size * 2;
        v->elements = realloc(v->elements, newcap * sizeof(void*));
        TEST_ALLOC(v->elements);
        v->capacity = newcap;
      }

      return true;
    }
  }
  return false;
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

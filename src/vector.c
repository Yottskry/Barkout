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
    if(v->elements == NULL)
      return false;
  }

  v->elements[v->size - 1] = element;
  return true;
}

bool vector_remove(Vector* v, void* element)
{
  return true;
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

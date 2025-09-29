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
	v->first = NULL;
//	printf("size %ld\n", sizeof(vMember));
//	printf("size v %ld\n", sizeof(Vector));
  return v;
}

bool vector_add(Vector* v, void* element)
{
//	printf("Size %ld\n", sizeof(*element));
	
	vMember* mem = malloc(sizeof(vMember));
	mem->next = NULL;
	mem->data = element;
	if(v->first == NULL)
	{
		v->first = mem;
	}
	else
	{
		vMember* item = v->first;
		while(item->next != NULL)
		{
			item = item->next;
		}
		item->next = mem;
	}

	v->size++;	

  return true;
}

bool vector_remove(Vector* v, void* element)
{
	vMember* item = v->first;
	vMember* lastitem = NULL;

	if(item == NULL)
		return false;

  while((item != NULL))
	{
		if(item->data == element)
		{
			if(lastitem == NULL)
				v->first = item->next;
			else
				lastitem->next = item->next;
			free(item);
			v->size--;
			return true;
		}
		lastitem = item;
		item = item->next;
  }

  return false;
}

void* vector_item(Vector* v, const int index)
{
  if(index >= v->size)
    return NULL;

	vMember* item = v->first;
	for(int i = 0; i < index; i++)
	{
		item = item->next;
	}

  return item->data;
}

void vector_free(Vector* v)
{
  vMember* item = v->first;
	while(item != NULL)
	{
		vMember* next = item->next;
		free(item);
		item = next;
	}
  free(v);
}

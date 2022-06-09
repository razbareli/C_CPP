#include "vector.h"
#include <stdio.h>

/**
 * Dynamically allocates a new vector.
 * @param elem_copy_func func which copies the element stored in the vector (returns
 * dynamically allocated copy).
 * @param elem_cmp_func func which is used to compare elements stored in the vector.
 * @param elem_free_func func which frees elements stored in the vector.
 * @return pointer to dynamically allocated vector.
 * @if_fail return NULL.
 */
vector *vector_alloc (vector_elem_cpy elem_copy_func,vector_elem_cmp elem_cmp_func,
                      vector_elem_free elem_free_func)
{
  if (elem_copy_func == NULL || elem_free_func == NULL || elem_cmp_func == NULL)
    {
      return NULL;
    }
  vector *v = malloc (sizeof (vector));
  if (v == NULL)
    {
      return NULL;
    }
  v->capacity = VECTOR_INITIAL_CAP;
  v->size = 0;
  v->elem_cmp_func = elem_cmp_func;
  v->elem_free_func = elem_free_func;
  v->elem_copy_func = elem_copy_func;
  void **d = malloc (sizeof (void *) * (v->capacity));
  if (d == NULL)
    {
      free (v);
      return NULL;
    }
  v->data = d;
  return v;
}

/**
 * Frees a vector and the elements the vector itself allocated.
 * @param p_vector pointer to dynamically allocated pointer to vector.
 */
void vector_free (vector **p_vector)
{
  vector_clear (*(p_vector));
  free ((*p_vector)->data);
  (*p_vector)->data = NULL;
  free (*p_vector);
  *p_vector = NULL;
}

/**
 * Returns the element at the given index.
 * @param vector pointer to a vector.
 * @param ind the index of the element we want to get.
 * @return the element at the given index if exists (the element itself, not a copy of it),
 * NULL otherwise.
 */
void *vector_at (const vector *vector, size_t ind)
{
  if (vector == NULL || ind >= vector->size)
    {
      return NULL;
    }
  return vector->data[ind];
}

/**
 * Gets a value and checks if the value is in the vector.
 * @param vector a pointer to vector.
 * @param value the value to look for.
 * @return the index of the given value if it is in the vector ([0, vector_size - 1]).
 * Returns -1 if no such value in the vector.
 */
int vector_find (const vector *vector, const void *value)
{
  if (vector == NULL)
    {
      return -1;
    }
  size_t i = 0;
  size_t ans;
  while (i < vector->size)
    {
      ans = vector->elem_cmp_func (vector->data[i], value);
      if (ans == 1)
        {
          return i;
        }
      i++;
    }
  return -1;
}

/**
 * Adds a new value to the back (index vector_size) of the vector.
 * @param vector a pointer to vector.
 * @param value the value to be added to the vector.
 * @return 1 if the adding has been done successfully, 0 otherwise.
 */
int vector_push_back (vector *vector, const void *value)
{
  if (vector == NULL || vector->capacity == 0)
    {
      return 0;
    }
  vector->data[vector->size] = vector->elem_copy_func (value);
  if (vector->data[vector->size] == NULL)
    {
      return 0;
    }
  vector->size++;
  if (vector_get_load_factor (vector) > VECTOR_MAX_LOAD_FACTOR)
    {
      vector->capacity = vector->capacity * VECTOR_GROWTH_FACTOR;
      void **d = realloc (vector->data, sizeof (void *) * vector->capacity);
      if (d == NULL)
        {
          vector_free (&vector);
          return 0;
        }
      vector->data = d;
    }
  return 1;
}

/**
 * This function returns the load factor of the vector.
 * @param vector a vector.
 * @return the vector's load factor, -1 if the function failed.
 */
double vector_get_load_factor (const vector *vector)
{
  if (vector == NULL || vector->capacity == 0)
    {
      return -1;
    }
  double ans = (double) vector->size / (double) vector->capacity;
  return ans;
}

/**
 * Removes the element at the given index from the vector. alters the indices of the remaining
 * elements so that there are no empty indices in the range [0, size-1] (inclusive).
 * @param vector a pointer to vector.
 * @param ind the index of the element to be removed.
 * @return 1 if the removing has been done successfully, 0 otherwise.
 */
int vector_erase (vector *vector, size_t ind)
{
  if (vector == NULL || ind >= vector->size)
    {
      return 0;
    }

  //delete the element in the index
  void **temp = &(vector->data[ind]);
  vector->elem_free_func (temp);
  //move back all the elements that are after the deleted element
  for (size_t i = ind; i < (vector->size - 1); ++i)
    {
      vector->data[i] = vector->elem_copy_func (vector->data[i + 1]);
      temp = &(vector->data[i + 1]);
      vector->elem_free_func (temp);
    }
  vector->data[vector->size - 1] = NULL;
  vector->size--;
  //if capacity is 1 then don't shrink
  if (vector->size == 0)
    {
      return 1;
    }
  //shrink vector if needed
  if (vector_get_load_factor (vector) < VECTOR_MIN_LOAD_FACTOR)
    {
      vector->capacity = vector->capacity / VECTOR_GROWTH_FACTOR;
      void **d = realloc (vector->data, sizeof (void *) * vector->capacity);
      if (d == NULL)
        {
          vector_free (&vector);
          return 0;
        }
      vector->data = d;
    }
  return 1;

}

/**
 * Deletes all the elements in the vector.
 * @param vector vector a pointer to vector.
 */
void vector_clear (vector *vector)
{
  while (vector->size != 0)
    {
      vector_erase (vector, vector->size - 1);
    }
}


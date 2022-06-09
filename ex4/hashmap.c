#include "hashmap.h"

/**
 * Compares two key pairs, using the key_cmp function of p1
 * @param p1 pair
 * @param k1 key
 * @return 1 if keys are equal , 0 else
 */
int key_pair_cmp (const void *p1, const void *k1)
{
  if (!p1 || !k1)
    {
      return 0;
    }
  const pair *pair1 = (const pair *) p1;
  const keyT *key1 = (const keyT *) k1;
  int key_cmp = pair1->key_cmp (pair1->key, key1);
  return key_cmp;
}

/**
 * re-hashes the elements in the table when it's size is changing
 * @param hash_map the hash map to be re-hashed.
 * @param prev_cap the capacity before the scaling of the map
 * @return returns 1 for successful re-hashing, 0 otherwise.
 */
int rehash (hashmap *hash_map, size_t prev_cap)
{
  for (size_t i = 0; i < prev_cap; ++i)
    {
      vector *curr_vec = (hash_map->buckets)[i];
      size_t j = 0;
      while (j < curr_vec->size)
        {
          const pair *curr_pair = vector_at (curr_vec, j);
          size_t key = hash_map->hash_func (curr_pair->key);
          size_t ind = key & (hash_map->capacity - 1);
          if (ind != i)
            {
              hash_map->size--;
              hashmap_insert (hash_map, curr_pair);
              vector_erase (curr_vec, j);
            }
          else
            {
              j++;
            }
        }
    }
  return 1;
}

/**
 * Allocates dynamically new hash map element.
 * @param func a function which "hashes" keys.
 * @return pointer to dynamically allocated hashmap.
 * @if_fail return NULL.
 */
hashmap *hashmap_alloc (hash_func func)
{
  hashmap *h = malloc (sizeof (hashmap));
  if (h == NULL)
    {
      return NULL;
    }
  vector **b = malloc (sizeof (vector) * HASH_MAP_INITIAL_CAP);
  if (b == NULL)
    {
      free (h);
      return NULL;
    }
  h->buckets = b;
  h->capacity = HASH_MAP_INITIAL_CAP;
  h->size = 0;
  h->hash_func = func;
  for (size_t i = 0; i < HASH_MAP_INITIAL_CAP; ++i)
    {
      b[i] = vector_alloc (pair_copy, key_pair_cmp, pair_free);
      if (b[i] == NULL)
        {
          for (size_t j = 0; j < i; ++j)
            {
              vector_free(&b[j]);
            }
          free (b);
          free (h);
          return NULL;
        }
    }
  return h;
}

/**
 * Frees a hash map and the elements the hash map itself allocated.
 * @param p_hash_map pointer to dynamically allocated pointer to hash_map.
 */
void hashmap_free (hashmap **p_hash_map)
{
//  vector **temp;
  for (size_t i = 0; i < (*p_hash_map)->capacity; ++i)
    {
      vector* curr_vec = (*p_hash_map)->buckets[i];
      vector_free(&curr_vec);
    }
  free ((*p_hash_map)->buckets);
  (*p_hash_map)->buckets = NULL;
  free (*p_hash_map);
  *p_hash_map = NULL;
}

/**
 * Inserts a new in_pair to the hash map.
 * The function inserts *new*, *copied*, *dynamically allocated* in_pair,
 * NOT the in_pair it receives as a parameter.
 * @param hash_map the hash map to be inserted with new element.
 * @param in_pair a in_pair the hash map would contain.
 * @return returns 1 for successful insertion, 0 otherwise.
 */
int hashmap_insert (hashmap *hash_map, const pair *in_pair)
{
  if (hash_map == NULL || in_pair == NULL){
      return 0;
    }
  if (hashmap_at (hash_map, in_pair->key) != NULL)
    {
      return 0;
    }
  size_t key = hash_map->hash_func (in_pair->key);
  size_t ind = key & (hash_map->capacity - 1);
  vector *in_vec = hash_map->buckets[ind];
  int temp = vector_push_back (in_vec, in_pair);
  if (temp == 0)
    {
      return 0;
    }
  hash_map->size++;
  if (hashmap_get_load_factor (hash_map) > HASH_MAP_MAX_LOAD_FACTOR) //increase size of hashmap
    {
      vector **b = realloc (hash_map->buckets, sizeof (vector *)
      * (hash_map->capacity * HASH_MAP_GROWTH_FACTOR));
      if (b == NULL)
        {
          return 0;
        }
      for (size_t i = (hash_map->capacity); i <
          hash_map->capacity * HASH_MAP_GROWTH_FACTOR; ++i)
        {
          b[i] = vector_alloc (pair_copy, key_pair_cmp, pair_free);
        }
      hash_map->buckets = b;
      size_t prev_cap = hash_map->capacity;
      hash_map->capacity = (hash_map->capacity) * HASH_MAP_GROWTH_FACTOR;
      rehash (hash_map, prev_cap);
    }
  return 1;
}

/**
 * The function returns the value associated with the given key.
 * @param hash_map a hash map.
 * @param key the key to be checked.
 * @return the value associated with key if exists, NULL otherwise (the value itself,
 * not a copy of it).
 */
valueT hashmap_at (const hashmap *hash_map, const_keyT key)
{
  if (hash_map == NULL){
      return NULL;
    }
  size_t k = hash_map->hash_func (key);
  size_t ind = k & (hash_map->capacity - 1);
  int ans = vector_find (hash_map->buckets[ind], key);
  if (ans == -1)
    {
      return NULL;
    }
  valueT val;
  pair *p = vector_at (hash_map->buckets[ind], ans);
  val = p->value;
  return val;
}

/**
 * The function erases the pair associated with key.
 * @param hash_map a hash map.
 * @param key a key of the pair to be erased.
 * @return 1 if the erasing was done successfully, 0 otherwise. (if key not in map,
 * considered fail).
 */
int hashmap_erase (hashmap *hash_map, const_keyT key)
{
  if (!hash_map || !key)
    {
      return 0;
    }
  size_t k = hash_map->hash_func (key);
  size_t ind = k & (hash_map->capacity - 1);
  vector *vec = hash_map->buckets[ind];
  int index = vector_find (vec, key);
  if (index == -1)
    {
      return 0;
    }
  int ans = vector_erase (vec, index);
  if (ans == 0)
    {
      return ans;
    }
  hash_map->size--;
  //now check if we need to shrink the table
  if (hashmap_get_load_factor (hash_map) < HASH_MAP_MIN_LOAD_FACTOR)
    {
      size_t prev_cap = hash_map->capacity;
      hash_map->capacity = hash_map->capacity / HASH_MAP_GROWTH_FACTOR;
      rehash (hash_map, prev_cap);
      for (size_t i = hash_map->capacity; i < prev_cap; ++i)
        {
          vector* curr = hash_map->buckets[i];
          vector_free(&curr);
        }
      vector **b = realloc (hash_map->buckets,
                            sizeof (vector *) * hash_map->capacity);
      if (b == NULL)
        {
          hash_map->capacity = hash_map->capacity * HASH_MAP_GROWTH_FACTOR;
          rehash (hash_map, hash_map->capacity);
          return 0;
        }

      hash_map->buckets = b;
    }
  return 1;
}

/**
 * This function returns the load factor of the hash map.
 * @param hash_map a hash map.
 * @return the hash map's load factor, -1 if the function failed.
 */
double hashmap_get_load_factor (const hashmap *hash_map)
{
  if (hash_map == NULL || hash_map->size == 0)
    {
      return -1;
    }
  double ans = (double) hash_map->size / (double) hash_map->capacity;
  return ans;
}

/**
 * This function receives a hashmap and 2 functions, the first checks a condition on the keys,
 * and the seconds apply some modification on the values. The function should apply the modification
 * only on the values that are associated with keys that meet the condition.
 *
 * Example: if the hashmap maps char->int, keyT_func checks if the char is a capital letter (A-Z),
 * and val_t_func multiples the number by 2, hashmap_apply_if will change the map:
 * {('C',2),('#',3),('X',5)}, to: {('C',4),('#',3),('X',10)}, and the return value will be 2.
 * @param hash_map a hashmap
 * @param keyT_func a function that checks a condition on keyT and return 1 if true, 0 else
 * @param valT_func a function that modifies valueT, in-place
 * @return number of changed values
 */
int hashmap_apply_if (const hashmap *hash_map,
                      keyT_func keyT_func, valueT_func valT_func){
  if (hash_map == NULL || keyT_func == NULL || valT_func == NULL){
      return -1;
    }
  int changed_values = 0;
  for (size_t i = 0; i < hash_map->capacity; ++i)
    {
      vector* curr_vec = hash_map->buckets[i];
      for (size_t j = 0; j < curr_vec->size; ++j)
        {
          pair* curr_pair = curr_vec->data[j];
          int res = keyT_func(curr_pair->key);
          if (res == 1){
              valT_func(curr_pair->value);
              changed_values++;
            }
        }
    }
  return changed_values;
}



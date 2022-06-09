#include "test_suite.h"
#include "hash_funcs.h"
#include "test_pairs.h"

/**
 * This function checks the hashmap_insert function of the hashmap library.
 * If hashmap_insert fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_insert(void){
  int increase = HASH_MAP_INITIAL_CAP * HASH_MAP_MAX_LOAD_FACTOR;
  hashmap* map = hashmap_alloc(hash_char);
  for (int j = 0; j < increase; ++j)
    {
      char key = (char) (j + 48);
      //even keys are capital letters, odd keys are digits
      if (key % 2)
        {
          key += 17;
        }
      int value = j;
      pair* par = pair_alloc (&key, &value, char_key_cpy
                              ,int_value_cpy, char_key_cmp,
                             int_value_cmp, char_key_free, int_value_free);
      hashmap_insert (map, par);
      pair_free((void**)&par);
    }

  assert(map->capacity == HASH_MAP_INITIAL_CAP);
  char c = ' ';
  int i = 1;
  pair *p = pair_alloc (&c, &i, char_key_cpy, int_value_cpy,
                        char_key_cmp, int_value_cmp, char_key_free,
                        int_value_free);
  assert(hashmap_insert (map, p) == 1);
  assert(map->capacity == HASH_MAP_INITIAL_CAP*HASH_MAP_GROWTH_FACTOR);
  size_t size = map->size;
  pair_free ((void **) &p);
  p = pair_alloc (&c, &i, char_key_cpy, int_value_cpy, char_key_cmp,
                  int_value_cmp, char_key_free, int_value_free);
  hashmap_insert (map, p);
  pair_free ((void **) &p);
  assert(map->size==size);
  hashmap_free(&map);
}

/**
 * This function checks the hashmap_at function of the hashmap library.
 * If hashmap_at fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_at(void){
  hashmap* map = hashmap_alloc(hash_char);
  char c_arr[] = {'z','4'};
  int i_arr[] =  {1 ,  2};
  pair *par[2];
  for (int i = 0; i < 2; ++i){
      par[i] = pair_alloc (&c_arr[i], &i_arr[i],
                           char_key_cpy, int_value_cpy, char_key_cmp,
                           int_value_cmp, char_key_free, int_value_free);
  }
  hashmap_insert(map, par[0]);
  hashmap_insert(map, par[1]);
  char temp1 = '4';
  assert(*(int*)hashmap_at(map, &temp1) == 2);
  temp1 = 'p';
  assert(hashmap_at(map, &temp1) == NULL);
  for (int k = 0; k < 2; ++k)
    {
      pair_free ((void **) &par[k]);
    }
  hashmap_free(&map);

}

/**
 * This function checks the hashmap_erase function of the hashmap library.
 * If hashmap_erase fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_erase(void){
  size_t decrease = HASH_MAP_INITIAL_CAP*HASH_MAP_MIN_LOAD_FACTOR;
  hashmap* map = hashmap_alloc(hash_char);
  for (size_t j = 0; j < decrease; ++j)
    {
      char key = (char) (j + 48);
      //even keys are capital letters, odd keys are digits
      if (key % 2)
        {
          key += 17;
        }
      int value = j;
      pair* par = pair_alloc (&key, &value, char_key_cpy, int_value_cpy, char_key_cmp,
                              int_value_cmp, char_key_free, int_value_free);
      hashmap_insert (map, par);
      pair_free((void**)&par);
    }
  assert(map->size == decrease);
  char c = ' ';
  int i = 1;
  pair *p = pair_alloc (&c, &i, char_key_cpy,
                        int_value_cpy, char_key_cmp,
                        int_value_cmp, char_key_free, int_value_free);
  hashmap_insert (map, p);
  char temp1 = '#';
  assert(hashmap_erase(map, &temp1) == 0);
  temp1 = 'B';
  assert(hashmap_erase(map, &temp1) == 1);
  assert(map->size == 4);
  assert(map->capacity == HASH_MAP_INITIAL_CAP);
  char temp2 = '2';
  assert(hashmap_erase(map, &temp2) == 1);
  assert(map->capacity == HASH_MAP_INITIAL_CAP/HASH_MAP_GROWTH_FACTOR);
  hashmap_free(&map);

}

/**
 * This function checks the hashmap_get_load_factor function of the hashmap library.
 * If hashmap_get_load_factor fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_get_load_factor(void){
  hashmap* map = hashmap_alloc(hash_char);
  assert(hashmap_get_load_factor(map) == -1);
  pair *par[12];
  for (int j = 0; j < 12; ++j)
    {
      char key = (char) (j + 48);
      int value = j;
      par[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy, char_key_cmp,
                             int_value_cmp, char_key_free, int_value_free);
    }
  for (int k = 0; k < 12; ++k)
    {
      hashmap_insert (map, par[k]);
    }
  assert(hashmap_get_load_factor(NULL) == -1);
  assert(hashmap_get_load_factor(map) == 0.75);
  for (int k = 0; k < 12; ++k)
    {
      pair_free ((void **) &par[k]);
    }
  hashmap_free(&map);

}

/**
 * This function checks the HashMapGetApplyIf function of the hashmap library.
 * If HashMapGetApplyIf fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_apply_if(){
  hashmap* map = hashmap_alloc(hash_char);
  char c_arr[] = {'z','4'};
  int i_arr[] =  {1 ,  2};
  pair *par[2];
  for (int i = 0; i < 2; ++i){
      par[i] = pair_alloc (&c_arr[i], &i_arr[i],
                           char_key_cpy, int_value_cpy,char_key_cmp,
                           int_value_cmp, char_key_free, int_value_free);
    }
  hashmap_insert(map, par[0]);
  hashmap_insert(map, par[1]);
  assert(hashmap_apply_if(map, is_digit, double_value) == 1);
  for (int k = 0; k < 2; ++k)
    {
      pair_free ((void **) &par[k]);
    }
  hashmap_free(&map);
}



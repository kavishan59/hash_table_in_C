#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define HT_PRIME_1 31
#define HT_PRIME_2 37

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>



typedef struct 
{
  char *key;
  void *value;
  size_t value_size;
} ht_item;

typedef struct 
{
  int size;
  int count;
  ht_item **items;
} ht_hash_table;



ht_hash_table *ht_new(int size);
void ht_del_hash_table(ht_hash_table *ht);
void ht_insert(ht_hash_table* ht, const char* key, const void* value,size_t value_size);
void* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* ht, const char* key);
void ht_print(ht_hash_table *ht, char type);

#endif // !HASH_TABLE_H

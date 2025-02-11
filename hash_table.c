#include "hash_table.h"
#include <stdlib.h>
#include <string.h>

// represent a deleted item
static ht_item HT_DELETED_ITEM = {NULL, NULL};

static ht_item * ht_new_item(const char *k, const char *v)
{
  ht_item *i = malloc(sizeof(ht_item));
  i->key = strdup(k);
  i->value = strdup(v);
  return (i);
}

//create a new hash table
ht_hash_table *ht_new(int size)
{
  ht_hash_table *ht = malloc(sizeof(ht_hash_table));

  ht->size = size;
  ht->count = 0;
  ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
  return ht;
}

//delete an item
static void ht_del_item(ht_item *i)
{
  free(i->key);
  free(i->value);
  free(i);
}

//delete the whole hash table
void ht_del_hash_table(ht_hash_table *ht)
{
  for (int i = 0; i < ht->size; i++)
  {
    ht_item *item = ht->items[i];
    if (item != NULL && item != &HT_DELETED_ITEM)
      ht_del_item(item);
  }
  free(ht->items);
  free(ht); 
}

//our hash function
static int ht_hash(const char *s, const int a, const int m)
{
  long hash = 0;
  const int len_s = strlen(s);
  for (int i = 0; i < len_s; i++)
  {
    hash += (long)pow(a,len_s - (i+1)) * s[i];
    hash = hash % m;
  }
  return (int)hash;
}

// manage collision using double hash technique
static int ht_get_hash(const char *s, const int num_buckets, const int attempt)
{
  const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
  const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
  return (hash_a + (attempt *(hash_b + 1))) % num_buckets;
}

//resize function
static void ht_resize(ht_hash_table *ht, const int new_size)
{
  ht_hash_table *new_ht = ht_new(new_size);

  for(int i = 0; i < ht->size; i++)
  {
    ht_item *item = ht->items[i];
    if (item != NULL && item !=&HT_DELETED_ITEM)
    {
      //reinsert item into new has table
      int index = ht_get_hash(item->key, new_ht->size, 0);
      ht_item *cur_item = new_ht->items[index];
      int j = 1;

      while (cur_item != NULL && cur_item != &HT_DELETED_ITEM)
      {
        index = ht_get_hash(item->key, new_ht->size, j);
        cur_item = new_ht->items[index];
        j++;
      }
      new_ht->items[index] = item;
      new_ht->count++;
    }
  }
 
    
  ht->size = new_ht->size;
  ht->count = new_ht->count;
  ht_item **old_items = ht->items;
  ht->items = new_ht->items;
  
  free(old_items);
  free(new_ht);
}                  


void ht_insert(ht_hash_table* ht, const char* key, const char* value)
{
  //resize if load factor > 0.5
  if(ht->count > ht->size/2)
    ht_resize(ht, ht->size * 2);

  ht_item *item = ht_new_item(key, value);
  int index = ht_get_hash(item->key, ht->size, 0);
  ht_item *cur_item = ht->items[index];
  int i = 1;
  while (cur_item != NULL && cur_item != &HT_DELETED_ITEM)
  {
    if (strcmp(cur_item->key, key) == 0)
    {
      ht_del_item(cur_item);
      ht->items[index] = item;
      return ;
    }
    index = ht_get_hash(item->key, ht->size, i);
    cur_item = ht->items[index];
    i++;
  }
  ht->items[index] = item;
  ht->count++;
}


//search function
char* ht_search(ht_hash_table* ht, const char* key)
{
  int index = ht_get_hash(key, ht->size, 0);
  ht_item *item = ht->items[index];
  int i = 1;
  while (item != NULL)
  {
    if (item != &HT_DELETED_ITEM && strcmp(item->key, key)== 0)
      return item->value;

    index = ht_get_hash(key, ht->size, i);
    item = ht->items[index];
    i++;
  }
  return NULL;
}

//delete a bucket by key
void ht_delete(ht_hash_table* ht, const char* key)
{
  int index = ht_get_hash(key, ht->size, 0);
  ht_item *item = ht->items[index];
  int i = 1;
  while (item != NULL)
  {
    if (item != &HT_DELETED_ITEM && strcmp(item->key, key) == 0)
    {
      ht_del_item(item);
      ht->items[index] = &HT_DELETED_ITEM;
      ht->count--;
      return ;
    }
    index = ht_get_hash(key, ht->size, i);
    item = ht->items[index];
    i++;
  }
}

void ht_print(ht_hash_table *ht)
{
  printf("\n===== Hash Table =====\n");
    for (int i = 0; i < ht->size; i++)
    {
        ht_item* item = ht->items[i];
        if (item == NULL)
        {
            printf("[%d] EMPTY\n", i);
        }
        else if (item == &HT_DELETED_ITEM)
        {
            printf("[%d] DELETED\n", i);
        }
        else
        {
            printf("[%d] Key: %s | Value: %s\n", i, item->key, item->value);
        }
    }
    printf("=======================\n");
}



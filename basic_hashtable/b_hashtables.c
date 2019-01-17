#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/****
  Basic hash table key/value pair
 ****/
typedef struct Pair {
  char *key;
  char *value;
} Pair;

/****
  Basic hash table
 ****/
typedef struct BasicHashTable {
  int capacity;
  Pair **storage;
} BasicHashTable;


//  Creates a key/value pair to be stored in the hash table.

Pair *create_pair(char *key, char *value)
{
  Pair *pair = malloc(sizeof(Pair));
  pair->key = strdup(key);
  pair->value = strdup(value);

  return pair;
}


//  Use this function to safely destroy a hashtable pair.

void destroy_pair(Pair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}


//  djb2 hash function

unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char * u_str = (unsigned char *)str;

  while ((c = *u_str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}


BasicHashTable *create_hash_table(int capacity)
{
  BasicHashTable *ht = malloc(sizeof(BasicHashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(Pair *));

  return ht;
}


void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  Pair *new_pair = create_pair(key, value);
  Pair *stored_pair = ht->storage[hash(key, ht->capacity)];

  if (stored_pair != NULL)
  {
    if (strcmp(stored_pair->key, key) != 0)
    {
      printf("Warning Overwriting '%s'/'%s' with '%s'/'%s'\n", stored_pair->key, stored_pair->value, new_pair->key, new_pair->value);
    }
    
  }

  destroy_pair(stored_pair);
  ht->storage[hash(new_pair->key, ht->capacity)] = new_pair;
}


void hash_table_remove(BasicHashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);
  destroy_pair(ht->storage[index]);
  ht->storage[index] = NULL;
}


char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  if (ht->storage[hash(key,ht->capacity)] != NULL)
  {
    return ht->storage[hash(key, ht->capacity)]->value;
  }

  return NULL;
}

void destroy_hash_table(BasicHashTable *ht)
{
  for (int i = 0; i < ht->capacity; i++)
  {
    if (ht->storage[i] != NULL)
    {
      destroy_pair(ht->storage[i]);
    }
  }

  free(ht->storage);
  free(ht);
}


#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);

  hash_table_insert(ht, "line", "Here today...\n");

  printf("%s", hash_table_retrieve(ht, "line"));

  hash_table_remove(ht, "line");

  if (hash_table_retrieve(ht, "line") == NULL) {
    printf("...gone tomorrow. (success)\n");
  } else {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }

  destroy_hash_table(ht);

  return 0; 
}
#endif 

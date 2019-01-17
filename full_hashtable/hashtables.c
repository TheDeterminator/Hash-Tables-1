#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//  Hash table key/value pair with linked list pointer
typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

//  Hash table with linked pairs
typedef struct HashTable {
  int capacity;
  int count;
  LinkedPair **storage;
} HashTable;


//  Create a key/value linked pair to be stored in the hash table.

LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}


//  This function safely destroys a hashtable pair.
void destroy_pair(LinkedPair *pair)
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


HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->count = 0;
  ht->storage = calloc(capacity, sizeof(LinkedPair*));

  return ht;
}

// Inserts key/value pair into hash table as a linked list node. 
// If a collision is encountered new value can be appended to the linked list. 
// If the same key is entered twice, the second entry will overwrite the first.
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  unsigned int index = hash(key, ht->capacity);
  LinkedPair *current_pair = ht->storage[index];

  if (current_pair != NULL)
  {
    while (current_pair->next != NULL && strcmp(current_pair->key, key) != 0)
    {
      current_pair = current_pair->next;
    }

    if (current_pair->next != NULL)
    {
      current_pair->value = value;
    } else if (strcmp(current_pair->key, key) == 0)
    {
      current_pair->value = value;
    } else {
      // if ((ht->count/ht->capacity) >= 0.7)
      // {
      //   hash_table_resize(ht);
      // } hash table resizing...
      current_pair->next = create_pair(key, value);
      ht->count++;
    }   
  } else {
    ht->storage[index] = create_pair(key, value);
    ht->count++;
  }
}


void hash_table_remove(HashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);
  LinkedPair *current_pair = ht->storage[index];
  LinkedPair *previous_pair = NULL;
  // printf("removing %s from index %i\n", current_pair->key, index);

// Need to clean this up too many ifs, specifically if (current_pair == NULL)
  if (current_pair == NULL)
  {
   fprintf(stderr, "ValueError: value %s not in hash table\n", key);
   return; 
  }


  while (current_pair != NULL && strcmp(current_pair->key, key) != 0)
  {
    previous_pair = current_pair;
    current_pair = current_pair->next;
  }

  if (current_pair == NULL)
  {
    fprintf(stderr, "ValueError: value %s not in hash table", key);
  } else {
    if (previous_pair == NULL)
    {
      ht->storage[index] = current_pair->next;
    } else {
      previous_pair->next = current_pair->next;
    }
    destroy_pair(current_pair);
    ht->count--;
  }
}


char *hash_table_retrieve(HashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);
  LinkedPair *current_pair = ht->storage[index];

  if (current_pair == NULL)
  {
    return NULL;
  }

  if (strcmp(current_pair->key, key) == 0)
  {
    return current_pair->value;
  } else {
    while (current_pair->next)
    {
      current_pair = current_pair->next;
      if (strcmp(current_pair->key, key) == 0)
      {
        return current_pair->value;
      }
    }
  }

  return NULL;
}


void destroy_hash_table(HashTable *ht)
{
  for (int i = 0; i < ht->capacity; i++)
  {
    if (ht->storage[i] != NULL)
    {
      LinkedPair *deleting_pair = ht->storage[i];
      while (deleting_pair->next)
      {
        LinkedPair *temp = deleting_pair->next;
        // free(deleting_pair); 
        deleting_pair = temp;
      }
    }
    free(ht->storage[i]);
  }
  free(ht->storage);
}


// Should resize the hash table when load factor >= 0.7
//  Should create a new hash table with double the capacity
//  of the original and copy all elements into the new hash table.
HashTable *hash_table_resize(HashTable *ht)
{
  int old_capacity = ht->capacity;
  int new_capacity = old_capacity * 2;
  HashTable *new_ht = malloc(sizeof(HashTable));
  new_ht->storage = calloc(new_capacity, sizeof(LinkedPair *));
  new_ht->capacity = new_capacity;
  

  for (int i = 0; i < ht->capacity; i++)
  {
    new_ht->storage[i] = ht->storage[i];
    LinkedPair *current_pair = ht->storage[i];
    if (current_pair->next != NULL)
    {
      LinkedPair *new_pair = new_ht->storage[i];
      while (current_pair->next)
      {
        new_pair->next = current_pair->next;
        current_pair = current_pair->next;
        new_pair = new_pair->next;
      }
    }
  }

  printf("resizing hash table from %i to %i", old_capacity, new_capacity);
  return new_ht;
}


#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(8);

  // hash_table_insert(ht, "line_1", "Tiny hash table\n");
  // hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  // hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  // printf("%s", hash_table_retrieve(ht, "line_1"));
  // printf("%s", hash_table_retrieve(ht, "line_2"));
  // printf("%s", hash_table_retrieve(ht, "line_3"));

  hash_table_insert(ht, "key-0", "val-0"); //3
  hash_table_insert(ht, "key-1", "val-1"); //4
  hash_table_insert(ht, "key-2", "val-2"); //5
  hash_table_insert(ht, "key-3", "val-3"); //6
  hash_table_insert(ht, "key-4", "val-4"); //7
  hash_table_insert(ht, "key-5", "val-5"); //0
  hash_table_insert(ht, "key-6", "val-6"); //1
  hash_table_insert(ht, "key-7", "val-7"); //2
  hash_table_insert(ht, "key-8", "val-8"); //3
  hash_table_insert(ht, "key-9", "val-9"); //4
  
  printf("%s\n", hash_table_retrieve(ht, "key-0"));
  printf("%s\n", hash_table_retrieve(ht, "key-1"));
  printf("%s\n", hash_table_retrieve(ht, "key-2"));
  printf("%s\n", hash_table_retrieve(ht, "key-3"));
  printf("%s\n", hash_table_retrieve(ht, "key-4"));
  printf("%s\n", hash_table_retrieve(ht, "key-5"));
  printf("%s\n", hash_table_retrieve(ht, "key-6"));
  printf("%s\n", hash_table_retrieve(ht, "key-7"));
  printf("%s\n", hash_table_retrieve(ht, "key-8"));
  printf("%s\n", hash_table_retrieve(ht, "key-9"));


  hash_table_remove(ht, "key-9");
  hash_table_remove(ht, "key-8");
  hash_table_remove(ht, "key-7");
  hash_table_remove(ht, "key-6");
  hash_table_remove(ht, "key-5");
  hash_table_remove(ht, "key-4");
  hash_table_remove(ht, "key-3");
  hash_table_remove(ht, "key-2");
  hash_table_remove(ht, "key-1");
  hash_table_remove(ht, "key-0");

  printf("%s\n", hash_table_retrieve(ht, "key-0"));
  printf("%s\n", hash_table_retrieve(ht, "key-1"));
  printf("%s\n", hash_table_retrieve(ht, "key-2"));
  printf("%s\n", hash_table_retrieve(ht, "key-3"));
  printf("%s\n", hash_table_retrieve(ht, "key-4"));
  printf("%s\n", hash_table_retrieve(ht, "key-5"));
  printf("%s\n", hash_table_retrieve(ht, "key-6"));
  printf("%s\n", hash_table_retrieve(ht, "key-7"));
  printf("%s\n", hash_table_retrieve(ht, "key-8"));
  printf("%s\n", hash_table_retrieve(ht, "key-9"));

  hash_table_remove(ht, "key-9");


  // int old_capacity = ht->capacity;
  // ht = hash_table_resize(ht);
  // int new_capacity = ht->capacity;

  // printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
} 
#endif

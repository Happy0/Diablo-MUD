#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"

typedef struct hash_item hash_item;
typedef struct linked_list linked_list;

struct hash_item {
	char *key;
	void *payload;
	struct hash_item *next;
};

struct linked_list {
	struct hash_item *head;
	int size;
};

struct hashtable {
	struct linked_list **items;
	int size;
};



hashtable *hashtable_init(int initial_capacity) 
{
	hashtable *table;
	linked_list **items;
	
	table = malloc(sizeof(hashtable));
	items = calloc(initial_capacity, sizeof(struct linked_list *));
	
	/* Initialise the buckets */
	int i;
	linked_list *ll;
	for (i = 0; i < initial_capacity; i++) 
	{
		ll = malloc (sizeof(struct linked_list));		
		ll->head = NULL;
		ll->size = 0;

		*(items + i) = ll;
	} 

	table->items = items;
	table->size = 0;

	return table; 
}

int hashtable_add(const char *key, void *payload)
{


}

void *hashtable_get(const char *key)
{

}

void *hashtable_delete(const char *key)
{

}

void hashtable_destroy(hashtable *ht)
{

}

int hash (const char *key, int table_size) 
{
	/*  SDBM hash function */
	long h = 0;
	while (*key) h+=*key++ + (h<<6) + (h<<16) - h;

	return h % table_size;
}

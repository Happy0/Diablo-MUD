#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"

#define MAX_SLOTS 4

static int hash (const char *key, int table_size);

typedef struct hash_item hash_item;
typedef struct linked_list linked_list;

struct hash_item {
	const char *key;
	void *payload;
	struct hash_item *next;
	struct hash_item *prev;
};

struct linked_list {
	struct hash_item *head;
	struct hash_item *tail;
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
		ll->tail = NULL;
		ll->size = 0;

		*(items + i) = ll;
	} 

	table->items = items;
	table->size = 0;

	return table; 
}

int hashtable_add(hashtable *ht, const char *key, void *payload)
{
	int bucket_no;
	linked_list *ll;
	hash_item *item;	

	bucket_no = hash(key, ht->size);
	ll = *( (ht->items) + bucket_no);

	item = malloc(sizeof(hash_item));
	item->key = key;
	item->payload = payload;	
	
	/* First item in this bucket*/
	if (ll->head == NULL)
	{		
		item->prev = NULL;
		item->next = NULL;

		/* Set the head to this item */
		ll->head = item;
		ll->size = 1;
	} 
	/* If there is room in the bucket, add it at the tail of the linked list */
	else if (ll->size < MAX_BUCKET_SIZE) 
	{
		ll->tail->next = item;
		item->prev = ll->tail;
		item->next = NULL;
		ll->tail = item;					
		ll->size +=1;	
	}
	else
	/* There isn't room in the bucket, expand the hashtable */
	{

	}

	return 1;		

}

void hashtable_print(hashtable *table) 
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

static int hash (const char *key, int table_size) 
{
	/*  SDBM hash function */
	long h = 0;
	while (*key) h+=*key++ + (h<<6) + (h<<16) - h;

	return h % table_size;
}

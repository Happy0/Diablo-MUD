#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"

#define MAX_SLOTS 4

static int hash(const char *key, int table_size);

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
	int no_buckets;
	int no_items;
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
	table->no_buckets = 0;
	table->no_items = 0;

	return table; 
}

int hashtable_add(hashtable *ht, const char *key, void *payload)
{
	int bucket_no;
	linked_list *ll;
	hash_item *item;	

	/* If the key already exists in the hashtable, return False */
	if (hashtable_get(key) == NULL)
		return 0;

	bucket_no = hash(key, ht->no_buckets);
	ll = *( (ht->items) + bucket_no);

	item = malloc(sizeof(hash_item));

	if (item == NULL)
		return -1;

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
		ht->no_items+=1;
	} 
	/* If there is room in the bucket, add it at the tail of the linked list */
	else if (ll->size < MAX_BUCKET_SIZE) 
	{
		ll->tail->next = item;
		item->prev = ll->tail;
		item->next = NULL;
		ll->tail = item;					
		ll->size +=1;
		ht->no_items++;
	}
	else
	/* There isn't room in the bucket, expand the hashtable */
	{
		hashtable *ht;
		ht = realloc(ht, ht->no_buckets * 2);
		
		if (ht != NULL)
		{
			ht->no_buckets *= 2;
			hashtable_add(ht, key, payload);
		}
		else
		{
		/* There wasn't enough memory for the reallocation */	
			free(item);
			return -1;
		} 
	}
	return 1;		

}

/* Prints the contents of the hashtable. Assumes the payload is a char* */
static void debug_hashtable_print(hashtable *table) 
{
	int i;
	for (i = 0; i < table->no_buckets; i++)
	{
		linked_list *ll;
		ll = *(table->items) + i;

		if (ll->size == 0)
		{
			printf("%d -> EMPTY\n", i);
		}
		else
		{
			hash_item *current;
			current = ll->head;			
			while (current != NULL)
			{
				printf("%s, ");
				current = current->next;
			}	
			printf("\n");
		}
	}
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

static int hash(const char *key, int table_size) 
{
	/*  SDBM hash function */
	long h = 0;
	while (*key) h+=*key++ + (h<<6) + (h<<16) - h;

	return h % table_size;
}

/* Test */
int main(int argc, char **argv)
{



}



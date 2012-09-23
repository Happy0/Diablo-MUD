#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"

#define MAX_SLOTS 4

static int hash(const char *key, int table_size);
static hashtable *expand_table(hashtable **ht, int expand_to);

typedef struct hash_item hash_item;
typedef struct linked_list linked_list;

struct hash_item {
	const char *key;
	void *payload;
	struct hash_item *next;
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
	table->no_buckets = initial_capacity;
	table->no_items = 0;

	return table; 
}

int hashtable_add(hashtable *ht, const char *key, void *payload)
{
	int bucket_no;
	linked_list *ll;
	hash_item *item;	

	/* If the key already exists in the hashtable, return False */
	if (hashtable_get(ht, key) != NULL)
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
		item->next = NULL;

		/* Set the head to this item */
		ll->head = item;
		ll->tail = item;
		ll->size = 1;
		ht->no_items+=1;

		#ifdef HASH_DEBUG
		printf("Added at head\n");

		#endif
	} 
	/* If there is room in the bucket, add it at the tail of the linked list */
	else if (ll->size < MAX_BUCKET_SIZE) 
	{
		ll->tail->next = item;
		item->next = NULL;
		ll->tail = item;					
		ll->size +=1;
		ht->no_items++;
	}
	else
	/* There isn't room in the bucket, expand the hashtable */
	{	
		/* TODO: Think about whether expansion policy is sensible. 
 		* Should it instead go by load factor, rather than full bucket? */
		
		hashtable** point;
		point = &ht;
		ht = expand_table(point, ht->no_buckets*2);

		
		if (ht != NULL)
		{
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

/* Expands the hashtable to have more buckets. The number of buckets is
 * specified by 'expand_to' 
 *
 * The members of the old table are added to the new table. The old table 'ht'
 * is freed. The new table is returned. 
 *
 * 'NULL' is returned if there was not enough memory to allocate for the new hashtable*/
static hashtable *expand_table(hashtable **hat, int expand_to)
{
	hashtable *newht;
	hashtable *ht;
	newht = hashtable_init(expand_to);	
	ht = *hat;

	if (newht == NULL)
		return NULL;

	/* Copy the old table to the new table, rehashing its members */
	int i;
	for (i = 0; i < ht->no_buckets; i++)
	{
		linked_list *ll;
		ll = ht->items[i];

		if (!(ll->size == 0))
		{
			hash_item *current;
			current = ll->head;

			while (current != NULL)
			{
				hashtable_add(newht, current->key, current->payload);
				current = current->next;
			}
		}
	}

	/* Free the old hashtable */
	hashtable_destroy(ht);	
	
	return newht;
	
}

/* Prints the contents of the hashtable. Assumes the payload is a char* */
static void debug_hashtable_print(hashtable *table) 
{
	int i;
	for (i = 0; i < table->no_buckets; i++)
	{
		linked_list *ll;
		ll = table->items[i];

		if (ll->size == 0)
		{
			printf("%d -> EMPTY\n", i);
		}
		else
		{
			hash_item *current;
			current = ll->head;

			printf("%d -> ", i);
			while (current != NULL)
			{
				printf("{%s,%s} ", (char *) current->key, (char *) current->payload);
				current = current->next;
			}	
			printf("\n");
		}
	}

	printf("------------------------------------------------------------\n");
}

void *hashtable_get(hashtable *ht, const char *key)
{
	linked_list *ll;
	hash_item *current;
	int hash_no;

	hash_no = hash(key, ht->no_buckets);
	ll = ht->items[hash_no];

	current = ll->head;

	/* Iterate over the linked list until we find the desired key */
	while (current != NULL)
	{
		if (current->key == key)
		{
			return current->payload;
		}	
		current = current->next;
	}	

	/* The item isn't resident in the table */	
	return NULL;
}

void *hashtable_delete(hashtable *ht, const char *key)
{

	linked_list *ll;
	hash_item *current;
	hash_item *prev;
	
	int hash_no;

	hash_no = hash(key, ht->no_buckets);
	ll = ht->items[hash_no];

	current = ll->head;
	prev = NULL;

	/* Iterate over the linked list until we find the desired key */
	while (current != NULL)
	{
		if (current->key == key)
		{
			/* Link together the remaining nodes  */
			if (prev == NULL)
			{
				ll->head = ll->head->next;
			}
			else 
			{
				prev->next = current->next;
			}
		
			ht->no_items--;
			ll->size--;

			void *pay = current->payload;
			
			/* Free the hash_item */
			free(current);

			/* Return the payload */
			return pay;		
		}	
		prev = current;	
		current = current->next;
	}	

	/* The item isn't resident in the table */	
	return NULL;
}

void hashtable_destroy(hashtable *ht)
{
	int i;
	linked_list *ll;
	for (i = 0; i < ht->no_buckets; i++)
	{
		ll = ht->items[i];
		
		if (ll->size == 0)
		{
			free(ll);
		}
		else
		{
			hash_item *current;
			current = ll->head;
			while (current != NULL)
			{
				hash_item *next;
				next = current->next;

				free(current);
				current = next;		
			}

			free(ll);		
		}		
	} 
}

static int hash(const char *key, int table_size) 
{
	/*  SDBM hash function */
	long h = 0;
	while (*key) h+=*key++ + (h<<6) + (h<<16) - h;

	return h % table_size;
}

#ifdef HASH_DEBUG

/* Test */
int main(int argc, char **argv)
{
	int r; int second;
	hashtable *ht;
	ht = hashtable_init(2);
	
	
	r = hashtable_add(ht, "key", "data");

	debug_hashtable_print(ht);	
	printf("r was: %d\n", r);

	hashtable_add(ht, "key6", "data2");
	hashtable_add(ht, "key2", "data2");
	
	debug_hashtable_print(ht);	
	hashtable_add(ht, "key3", "data2");

	debug_hashtable_print(ht);	
	hashtable_add(ht, "key4", "data2");
	hashtable_add(ht, "key5", "data2");
	hashtable_add(ht, "key", "data2");
	debug_hashtable_print(ht);	
	hashtable_add(ht, "key7", "data2");
	hashtable_add(ht, "key8", "data2");
	hashtable_add(ht, "key9", "data2");
	
	debug_hashtable_print(ht);

	hashtable_delete(ht, "key");

	debug_hashtable_print(ht);

	hashtable_destroy(ht);
	
	return 1;
}

#endif


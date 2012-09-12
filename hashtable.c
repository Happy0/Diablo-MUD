#include <stdio.h>
#include <stdio.c>

struct hashtable {
	struct linked_list **items;
	int size;
};

struct linked_list {
	hash_item *head;
	int size;
};

struct hash_item {
	void *key;
	void *payload;
	hash_item *next;
}:

hashtable *hashtable_init(int initial_capacity) 
{
	hashtable *table;
	hash_item **items;
	
	table = malloc(sizeof(hashtable));
	items = calloc(initial_size, sizeof(struct linked_list *));

	table->items = items;
	table->size = size;

	return table; 
}

int hashtable_add(void *key, void *payload)
{


}


int hash (void 

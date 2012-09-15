#ifndef HASHTABLE_H
#define HASHTABLE_H

#define MAX_BUCKET_SIZE 4

#define HASH_DEBUG

typedef struct hashtable hashtable;

/* Creates a hashtable with the capacity for the specified number of items */
hashtable *hashtable_init(int initial_size);

/* Adds entry to the hashtable
 * Returns True if successful, and False if operation failed
 *
 * Returns -1 if there was not enough memory to add the item */
int hashtable_add(hashtable *ht, const char *key, void *payload);

/* Returns a pointer to the payload. Returns NULL if the key is not present. */
void *hashtable_get(const char *key);

/* Removes the entry with the given key from the hashtable.
 * Returns the payload associated with the key.
 *
 * Returns NULL if the key is not present in the hashtable. 
 */
void *hashtable_delete(const char *key);

/** Frees the hashtable */
void hashtable_destroy(hashtable *ht);

#endif

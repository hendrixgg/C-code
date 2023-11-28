#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// assert assumptions for each function.
#include <assert.h>

uint32_t hash(const char *str)
{
    uint32_t cur_hash = 1;
    while (*str)
    {
        cur_hash = (cur_hash << 3) | (cur_hash >> 29);
        cur_hash ^= ((uint8_t)*str * 19) & 0xFF;
        str++;
    }
    return cur_hash;
}

struct hash_table_entry
{
    char *key;
    int value;
    // Pointer to next entry.
    struct hash_table_entry *next;
};

#define LOAD_MIN 0.5
#define LOAD_MAX 2.0

struct hash_table
{
    struct hash_table_entry **data;
    size_t array_capacity, num_entries;
};

// TODO: Maintain documentation here
/**
 * If the key was already present in the hash table, the value is updated.
 *
 *
 * @param key The key to insert, the memory is now owned and will be managed by the hash table.
 *
 */
void insert_unchecked(struct hash_table *table, char *key, int value, bool check_for_duplicate)
{
    assert(table != NULL);
    assert(table->array_capacity > 0);
    assert(key != NULL);
    // Compute hash.
    uint32_t key_hash = hash(key);
    // Take remaninder of hash / array_capacity.
    // use remainder as index into data array.
    size_t index = key_hash % table->array_capacity;

    if (check_for_duplicate)
    {
        // Check that the hash table does not already contain the key.
        struct hash_table_entry *cur_entry = table->data[index];
        while (cur_entry)
        {
            // if the hash table already contains the key, update the value and return.
            if (strcmp(cur_entry->key, key) == 0)
            {
                cur_entry->value = value;
                free(key);
                return;
            }
            cur_entry = cur_entry->next;
        }
    }

    // Create new entry that has the desired key and value and points to the first element in that index of the hash table.
    struct hash_table_entry *new_entry = malloc(sizeof(struct hash_table_entry));
    *new_entry = (struct hash_table_entry){strdup(key), value, table->data[index]};
    // link the new entry into the list.
    table->data[index] = new_entry;

    table->num_entries++;
}

// TODO: Maintain documentation here
/**
 *
 */
void insert(struct hash_table *table, const char *key, int value)
{
    // TODO: update the table, maintaining a load factor between 0.5 and 2.0.
    if (table->array_capacity == 0)
    {
        table->array_capacity = 8;
        table->data = calloc(table->array_capacity, sizeof(struct hash_table_entry *));
    }
    else if (table->num_entries >= table->array_capacity * LOAD_MAX)
    {
        size_t new_capacity = table->array_capacity * LOAD_MAX;
        // construct new table with double capacity.
        struct hash_table new_table = {
            .data = calloc(new_capacity, sizeof(struct hash_table_entry *)),
            .array_capacity = new_capacity,
            .num_entries = 0,
        };
        // rehash all the entries. or use precaculated hash values.
        // reinsert all the entries.
        for (size_t i = 0; i < table->array_capacity; i++)
        {
            // iterate over the linked list of entries at the index i.
            struct hash_table_entry *cur_entry = table->data[i];
            while (cur_entry)
            {
                insert_unchecked(&new_table, cur_entry->key, cur_entry->value, false);
                cur_entry = cur_entry->next;
            }
        }
    }

    insert_unchecked(table, strdup(key), value, true);
}

// TODO: Maintain documentation here
bool search(struct hash_table *table, const char *key, int *value_out)
{
    assert(table != NULL);
    assert(key != NULL);
    assert(value_out != NULL);
    if (table->array_capacity == 0)
        return false;

    // TODO: search for the value in the table, without removing it.
    // Compute hash.
    uint32_t key_hash = hash(key);
    // Take remaninder of hash / array_capacity.
    // use remainder as index into data array.
    size_t index = key_hash % table->array_capacity;
    // Check that the hash table contains the key.
    struct hash_table_entry *cur_entry = table->data[index];
    while (cur_entry)
    {
        // if the hash table already contains the key, update the value and return.
        if (strcmp(cur_entry->key, key) == 0)
        {
            *value_out = cur_entry->value;
            return true;
        }
        cur_entry = cur_entry->next;
    }
    return false;
}

// TODO: create a function to remove an entry from the hash table.

int main()
{
    struct hash_table table = {
        .data = NULL,
        .array_capacity = 0,
        .num_entries = 0,
    };

    insert(&table, "this", 1);
    insert(&table, "is", 1);
    insert(&table, "a", 1);
    insert(&table, "test", 1);
    insert(&table, "of", 1);
    insert(&table, "the", 1);
    insert(&table, "hash", 1);
    insert(&table, "table", 1);
    insert(&table, "data", 1);
    insert(&table, "structure", 1);

    return 0;
}

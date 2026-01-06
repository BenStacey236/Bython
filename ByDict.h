#ifndef BYDICT_H
#define BYDICT_H

/* ----- Type Definitions----- */

/**
 * A single node in a Bython dictionary
 */
typedef struct ByDictNode ByDictNode;
struct ByDictNode
{
    char *key;
    char *value;
    ByDictNode *next;
};

/**
 * A Bython Dictionary
 */
typedef struct
{
    struct ByDictNode *head;
    struct ByDictNode *tail;
    int count;
} ByDict;

/* ----- Function Definitions ----- */

/**
 * Creates and allocates a new Bython dictionary
 * 
 * @returns The newly allocated `ByDict` structure
 */
ByDict *bydict_new();

/**
 * Deletes and frees a Bython dictionary
 * 
 * @param dict The `ByDict` to free
 */
void bydict_delete(ByDict *dict);


/**
 * Gets the value for a given key in a Bython dictionary
 * 
 * @param dict The `ByDict` to get the value from
 * @param key The key of the value to search for
 */
char *bydict_get(ByDict *dict, char *key);


/**
 * Puts a value into a Bython dictionary
 * 
 * @param dict The `ByDict` to put an item into
 * @param key The key to insert with
 * @param value The value to insert with
 */
void bydict_put(ByDict *dict, char *key, char *value);


/**
 * Gets the length of a Bython dictionary
 * 
 * @param dict The `ByDict` to find the lenght of
 */
int bydict_len(ByDict *dict);


/**
 * Prints out a Bython dictionary
 * 
 * @param dict the `ByDict` to print out
 */
void bydict_print(ByDict *dict);


#endif
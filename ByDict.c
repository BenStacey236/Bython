#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ByDict.h"

ByDict* bydict_new() {
    ByDict *dict = malloc(sizeof(*dict));

    dict->head = NULL;
    dict->tail = NULL;
    dict->count = 0;

    return dict;
}


void bydict_delete(ByDict *dict) {
    ByDictNode *curr, *next;

    curr = dict->head;
    while (curr) {
        free(curr->key);
        free(curr->value);
        next = curr->next;
        free(curr);
        curr = next;
    }

    free(dict);
}


static ByDictNode *bydict_find(ByDict *dict, char *key) {
    ByDictNode *curr, *next;

    curr = dict->head;
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            return curr;
        }

        next = curr->next;
        curr = next;
    }

    return NULL;
}


char *bydict_get(ByDict *dict, char *key) {
    ByDictNode *entry = bydict_find(dict, key);

    if (!entry) {
        return NULL;
    }

    return entry->value;
}


void bydict_put(ByDict *dict, char *key, char *value) {
    ByDictNode *old = bydict_find(dict, key);

    if (old) {
        strcpy(old->value, value);
        return;
    }

    ByDictNode *new = malloc(sizeof(ByDictNode));
    if (!new) return;

    new->key = malloc(strlen(key) + 1);
    new->value = malloc(strlen(value) + 1);

    strcpy(new->key, key);
    strcpy(new->value, value);
    new->next = NULL;

    if (!dict->head) {
        dict->head = new;
        dict->tail = new;
    } else {
        dict->tail->next = new;
        dict->tail = new;
    }
    dict->count++;
}


int bydict_len(ByDict *dict) {
    return dict->count;
}


void bydict_print(ByDict *dict) {
    ByDictNode *curr, *next;

    curr = dict->head;
    printf("{");
    while (curr) {
        printf("'%s': '%s', ", curr->key, curr->value);

        next = curr->next;
        curr = next;
    }

    printf("\b\b}\n");
}

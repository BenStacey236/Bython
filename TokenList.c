#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TokenList.h"


TokenList *new_tokenlist()
{
    TokenList *newList = (TokenList *) malloc(sizeof(TokenList));
    newList->head = NULL;
    newList->tail = NULL;
    newList->length = 0;

    return newList;
}


void append_token(TokenList *list, TokenType type, const char *lexeme, int lineNum)
{
    Token *newToken = (Token *) malloc(sizeof(Token));
    newToken->type = type;
    newToken->lexeme = strdup(lexeme);
    newToken->lineNum = lineNum;

    TokenListNode *newNode = (TokenListNode *) malloc(sizeof(TokenListNode));
    newNode->data = newToken;
    newNode->next = NULL;
    newNode->prev = list->tail;

    if (!list->head)
    {
        // If no list head, make the new node the head and tail of the TokenList
        list->head = newNode;
        list->tail = newNode;
        list->length++;
    }
    else if (list->tail)
    {
        // If the list has a head and a valid tail, append node to the tail of the TokenList
        list->tail->next = newNode;
        list->tail = newNode;
        list->length++;
    }
    else
    {
        // Error if the tail has a head but no valid tail
        fprintf(stderr, "Error appending to list: TokenList has head but no tail");
    }
}


void print_tokenlist(TokenList *list)
{
    // If list is empty, return early and print message
    if (!list->head)
    {
        printf("TokenList is empty");
        return;
    }

    // Print out each token in the list
    TokenListNode *current = list->head;
    while (current)
    {
        printf("Line %2d: %-12s %s\n", current->data->lineNum,
        (current->data->type == TOKEN_IDENTIFIER) ? "IDENTIFIER" :
        (current->data->type == TOKEN_NUMBER) ? "NUMBER" :
        (current->data->type == TOKEN_STRING) ? "STRING" :
        (current->data->type == TOKEN_ASSIGNMENT) ? "ASSIGNMENT" :
        (current->data->type == TOKEN_OPERATOR) ? "OPERATOR" :
        (current->data->type == TOKEN_LPAREN) ? "LPAREN" :
        (current->data->type == TOKEN_RPAREN) ? "RPAREN" :
        (current->data->type == TOKEN_COLON) ? "COLON" :
        (current->data->type == TOKEN_NEWLINE) ? "NEWLINE" :
        (current->data->type == TOKEN_INDENT) ? "INDENT" :
        (current->data->type == TOKEN_DEDENT) ? "DEDENT" :
        (current->data->type == TOKEN_EOF) ? "EOF" : "UNKNOWN",
        current->data->lexeme);

        current = current->next;
    }

    printf("TokenList length: %d\n", list->length);
}


void free_tokenlist_node(TokenListNode *node)
{
    free(node->data->lexeme);
    free(node->data);
    free(node);
}


void free_tokenlist(TokenList *list)
{
    if (list->head)
    {
        TokenListNode *current = list->head;
        
        while (current)
        {
            TokenListNode *next = current->next;
            free_tokenlist_node(current);
            current = next;
        }
    }

    free(list);
}

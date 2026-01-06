#ifndef PARSER_H
#define PARSER_H

#include "TokenList.h"
#include "AST.h"

/**
 * Parser structure that keeps track of where in the token list it has got to
 */
typedef struct
{
    TokenListNode *current;
} Parser;


/**
 * Parses a `TokenList` into an Abstract Syntax Tree
 * 
 * @param tokens The `TokenList` to parse
 * @returns The root AST node for the program
 */
ASTNode *parse_program(TokenList *tokens);

#endif
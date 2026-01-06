#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "AST.h"


/* ----- Function definitions for recursive use ----- */

static ASTNode *parse_expression(Parser *parser);


/**
 * Peeks at the actual `Token` associated with the current `TokenListNode`
 * 
 * @param parser The parser structure used for tracking parsing progress
 * @returns The current `Token`
 */
static Token *peek(Parser *parser) {
    return parser->current ? parser->current->data : NULL;
}


/**
 * Advances to the next token in the `TokenList`
 * 
 * @param parser The parser structure used for tracking parsing progress
 * @returns The next token in the `TokenList`
 */
static Token *advance(Parser *parser) {
    Token *token = peek(parser);

    if (parser->current) {
        parser->current = parser->current->next;
    }

    return token;
}


/**
 * Determines whether the current Token is a match to the provided `TokenType`, and advances the cursor if it is
 * 
 * @param parser The parser structure used for tracking parsing progress
 * @param type The `TokenType` we are trying to match
 */
static int match(Parser *parser, TokenType type)
{
    if (peek(parser) && peek(parser)->type == type)
    {
        advance(parser);
        return 1;
    }
    return 0;
}


/**
 * Throws an error at the current token, with an error message
 * 
 * @param token The `Token` which caused the syntax error
 * @param message The error message to display
 */
static void throw_error(Token *token, const char *message) {
    if (token) {
        fprintf(stderr, "[Line %d] Syntax error: %s\n", token->lineNum, message);
    } else {
        fprintf(stderr, "Syntax error at end of input: %s\n", message);
    }

    exit(1);
}


/**
 * Creates a new number literal AST node
 * 
 * @param value The value of the node
 * @param lineNum The line number the literal is found at
 * @returns The newly allocated number literal AST node
 */
static ASTNode *new_number_node(double value, int lineNum)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node) return NULL;

    node->type = AST_NUMBER;
    node->lineNum = lineNum;
    node->nodeVal.number.value = value;

    return node;
}


/**
 * Creates a new binary operator AST node
 * 
 * @param op The operator character
 * @param left The node attached to the left of the operator
 * @param right THe node attached to the right of the operator
 * @param lineNum The line number the operator is found at
 * @returns The newly allocated binary AST node
 */
static ASTNode *new_binary_node(char op, ASTNode *left, ASTNode *right, int lineNum)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node) return NULL;

    node->type = AST_BINARY_OP;
    node->lineNum = lineNum;
    node->nodeVal.binary.op = op;
    node->nodeVal.binary.left = left;
    node->nodeVal.binary.right = right;

    return node;
}


/**
 * Parses a factor of a term (either number or parentheses)
 * 
 * @param parser The parser keeping track of location in the `TokenList`
 * @returns A new ASTNode representing the factor
 */
static ASTNode *parse_factor(Parser *parser)
{
    Token *token = peek(parser);

    if (!token)
        throw_error(token, "Unexpected end of input");

    // Handle
    if (token->type == TOKEN_NUMBER)
    {
        advance(parser);
        double value = atof(token->lexeme);
        return new_number_node(value, token->lineNum);
    }

    // Handle parentheses
    if (token->type == TOKEN_LPAREN)
    {
        // Skip the left paranthesis
        advance(parser);
        ASTNode *expr = parse_expression(parser);

        // Match the right parenthesis
        if (!match(parser, TOKEN_RPAREN)) {
            throw_error(peek(parser), "Expected ')'");
        }

        return expr;
    }

    throw_error(token, "Expected number or '('");
    return NULL;
}


/**
 * Parses a term within the `TokenList`
 * 
 * @param parser The parser keeping track of location within the `TokenList`
 * @returns The root AST node for the term
 */
static ASTNode *parse_term(Parser *parser)
{
    ASTNode *node = parse_factor(parser);

    while (peek(parser) &&
           peek(parser)->type == TOKEN_OPERATOR &&
           (peek(parser)->lexeme[0] == '*' || peek(parser)->lexeme[0] == '/')) {

        Token *opToken = advance(parser);
        char op = opToken->lexeme[0];

        ASTNode *right = parse_factor(parser);
        node = new_binary_node(op, node, right, opToken->lineNum);
    }

    return node;
}


/**
 * Parses an expression from the `TokenList`
 * 
 * @param parser The parser keeping track of location within the `TokenList`
 * @returns The AST node representing the expression
 */
static ASTNode *parse_expression(Parser *parser)
{
    ASTNode *node = parse_term(parser);

    while (peek(parser) &&
           peek(parser)->type == TOKEN_OPERATOR &&
           (peek(parser)->lexeme[0] == '+' || peek(parser)->lexeme[0] == '-')) {

        Token *opToken = advance(parser);
        char op = opToken->lexeme[0];

        ASTNode *right = parse_term(parser);
        node = new_binary_node(op, node, right, opToken->lineNum);
    }

    return node;
}


ASTNode *parse_program(TokenList *tokens)
{
    Parser parser;
    parser.current = tokens->head;

    ASTNode *ast = parse_expression(&parser);

    if (peek(&parser) && peek(&parser)->type != TOKEN_EOF) {
        throw_error(peek(&parser), "Unexpected token after expression");
    }

    return ast;
}

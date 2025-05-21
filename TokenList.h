// ifndef ensuring that header file can only be included once to prevent errors
#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H


/* ----- Type Declarations ----- */

// Enum defining token types
typedef enum
{   
    TOKEN_IDENTIFIER,       // Identifier token
    TOKEN_NUMBER,           // Number token
    TOKEN_STRING,           // String token
    
    // Operators/brackets
    TOKEN_OPERATOR,         // Operator token
    TOKEN_PAREN,            // Brackets token
    TOKEN_COLON,            // Colon token
    
    // Whitespace
    TOKEN_NEWLINE,          // New line token
    TOKEN_INDENT,           // Indent token
    TOKEN_DEDENT,           // Dedent token

    // Unknown tokens
    TOKEN_EOF,              // End of file token
    TOKEN_UNKNOWN           // Unknown token

} TokenType;


// Token structure defining type and value
typedef struct 
{
    TokenType type;     // Token type
    char *lexeme;       // String with value of the token
    int lineNum;           // Line number token is found on
} Token;


// TokenList Node structure defining pointers to data, next and previous nodes
typedef struct TokenListNode TokenListNode;

struct TokenListNode
{   
    Token *data;
    TokenListNode *next;
    TokenListNode *prev;
};


// TokenList structure defining head, tail and length
typedef struct
{
    TokenListNode *head;
    TokenListNode *tail;
    int length;
} TokenList;


/* ----- Function Declarations ----- */

TokenList *new_tokenlist();
void append_token(TokenList *list, TokenType type, const char *lexeme, int lineNum);
void print_tokenlist(TokenList *list);
void free_tokenlist_node(TokenListNode *node);
void free_tokenlist(TokenList *list);

#endif // TOKEN_LIST_H
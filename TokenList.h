#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H


/* ----- Type Declarations ----- */

/**
 * Enum defining token types
 */
typedef enum
{   
    TOKEN_IDENTIFIER,       // Identifier token
    TOKEN_NUMBER,           // Number token
    TOKEN_STRING,           // String token
    
    // Operators/brackets
    TOKEN_ASSIGNMENT,       // Assignment operator
    TOKEN_OPERATOR,         // Operator token
    TOKEN_LPAREN,            // Left brackets token
    TOKEN_RPAREN,            // Right brackets token
    TOKEN_COLON,            // Colon token
    
    // Whitespace
    TOKEN_NEWLINE,          // New line token
    TOKEN_INDENT,           // Indent token
    TOKEN_DEDENT,           // Dedent token

    // Unknown tokens
    TOKEN_EOF,              // End of file token
    TOKEN_UNKNOWN           // Unknown token

} TokenType;


/**
 * Token structure defining type and value
 */
typedef struct 
{
    TokenType type;     // Token type
    char *lexeme;       // String with value of the token
    int lineNum;           // Line number token is found on
} Token;


/**
 * TokenList Node structure defining pointers to data, next and previous nodes
 */
typedef struct TokenListNode TokenListNode;
struct TokenListNode
{   
    Token *data;
    TokenListNode *next;
    TokenListNode *prev;
};


/**
 * TokenList structure defining head, tail, and length
 */
typedef struct
{
    TokenListNode *head;
    TokenListNode *tail;
    int length;
} TokenList;


/* ----- Function Declarations ----- */

/**
 * Initialises and allocates memory for a new TokenList
 *
 * @return A pointer to the newly created TokenList
 */
TokenList *new_tokenlist();

/**
 * Appends a new token to a `TokenList`
 * 
 * @param list The TokenList to append to
 * @param type The TokenType of the Token to append
 * @param lexeme A character array storing the value of the token (must be null terminated)
 * @param lineNum The line number that the Token occurs on
 */
void append_token(TokenList *list, TokenType type, const char *lexeme, int lineNum);

/**
 * Prints out the contents of a `TokenList`
 * 
 * @param list The `TokenListNode` to print out
 */
void print_tokenlist(TokenList *list);


/**
 * Frees a single node of a `TokenList` and all the data inside it
 * 
 * @param list The `TokenListNode` to free
 */
void free_tokenlist_node(TokenListNode *node);

/**
 * Frees a `TokenList` and all the data inside it
 * 
 * @param list The `TokenList` to free
 */
void free_tokenlist(TokenList *list);

#endif // TOKEN_LIST_H
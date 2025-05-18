# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <regex.h>
# include <stdbool.h>


# define MAX_TOKENS 1000                // Max number of tokens in the token list
# define MAX_INDENT_STACK_SIZE 100      // Max amount of indentation levels
# define NUM_TOKEN_PATTERNS 7           // The number of token patterns in the TOKEN_PATTERNS array


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


// Array of regex patterns for different tokens
const char *TOKEN_PATTERNS[] = {
    "^[a-zA-Z_][a-zA-Z0-9_]*",                      // TOKEN_IDENTIFIER
    "^[0-9]+(\\.[0-9]+)?",                          // TOKEN_NUMBER
    "^\"([^\"\\\\]|\\\\.)*\"|^'([^'\\\\]|\\\\.)*'", // TOKEN_STRING

    "^[\\+\\-\\*/=<>]=?",                             // TOKEN_OPERATOR
    "^[({})]",                                      // TOKEN_PAREN
    "^:",                                           // TOKEN_COLON

    "^\r?\n"                                        // TOKEN_NEWLINE
};


// Array of token types matching TOKEN_PATTERNS array
const TokenType TYPES[] = {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,

    TOKEN_OPERATOR,
    TOKEN_PAREN,
    TOKEN_COLON,

    TOKEN_NEWLINE
};


// Token structure defining type and value
typedef struct 
{
    TokenType type;     // Token type
    char *lexeme;       // String with value of the token
    int lineNum;           // Line number token is found on
} Token;


static Token tokens[MAX_TOKENS];
int tokenCount = 0;

int indentStack[MAX_INDENT_STACK_SIZE];
int indentTop = 0;


/**
 * Adds a Token to the tokens array
 * @param type A TokenType of the token to add to the array
 * @param lexeme A character array of the value of the token
 * @param lineNum An integer line number that the token appears on
 * @see
 * @return
 */
void add_token(TokenType type, const char *lexeme, int lineNum) 
{
    tokens[tokenCount].type = type;
    tokens[tokenCount].lexeme = strdup(lexeme);
    tokens[tokenCount].lineNum = lineNum;
    tokenCount++;
}


/**
 * Handles changes in indentation and when to add indent and dedent tokens
 * @param newIndent The new indentation level of the current line
 * @param lineNum An integer line number currently operating on
 * @see
 * @return
 */
void handle_indentation(int newIndent, int lineNum) 
{
    int current_indent = indentStack[indentTop - 1];

    if (newIndent > current_indent)
    {
        // Indent token if indentation is higher than the previous indent
        indentStack[indentTop++] = newIndent;
        add_token(TOKEN_INDENT, "<INDENT>", lineNum);
    } 
    else 
    {   
        // Dedent token if indentation is lower than the previous indent
        while (newIndent < current_indent)
        {
            indentTop--;
            current_indent = indentStack[indentTop - 1];
            add_token(TOKEN_DEDENT, "<DEDENT>", lineNum);
        }

        if (newIndent != current_indent)
        {
            fprintf(stderr, "Indentation error on line %d\n", lineNum);
            exit(1);
        }
    }
}


/**
 * Tokenises a line provided as a string
 * @param line A pointer to the start of the line
 * @param lineNum An integer line number of the current line
 * @see
 * @return
 */
void tokenise_line(char *line, int lineNum)
{
    int indent = 0;
    char *cursor = line;
    
    // Check if line is empty or a comment (#) and return if it is to skip line
    regex_t regex;
    regmatch_t match;
    regcomp(&regex, "^( *\r?\n?| *#.*\r?\n?)$", REG_EXTENDED);
    if (!regexec(&regex, line, 0, NULL, 0)) 
    {
        // Skip line if it is empty or a comment
        regfree(&regex);
        return;
    }
    regfree(&regex);

    // Updates indentation if line starts with spaces or tabs
    while (*cursor == ' ' || *cursor == '\t')
    {
        indent++;
        cursor++;
    }

    // Handles indentation
    handle_indentation(indent, lineNum);

    // Tokenises the rest of the line
    while (*cursor)
    {
        // Ignore whitespace
        if (*cursor == ' ' || *cursor == '\t')
        {
            cursor++;
            continue;
        }

        // Loops through all token patterns to find next token
        int matched = false;
        for (int i = 0; i < NUM_TOKEN_PATTERNS; i++)
        {
            // Attempt to compile regex pattern
            int errcode = regcomp(&regex, TOKEN_PATTERNS[i], REG_EXTENDED);
            if (errcode != 0)
            {
                char errbuf[256];
                regerror(errcode, &regex, errbuf, sizeof(errbuf));
                fprintf(stderr, "Regex compile error: %s (pattern: %s)\n", errbuf, TOKEN_PATTERNS[i]);
                continue;
            }

            // Execute the regex search
            if (regexec(&regex, cursor, 1, &match, 0) == 0)
            {
                // Copy the lexeme to a temporary variable
                int len = match.rm_eo;
                char *lexeme = (char *)malloc(len + 1);
                strncpy(lexeme, cursor, len);
                lexeme[len] = '\0';
                
                // Add a token to the token list
                add_token(TYPES[i], lexeme, lineNum);
                free(lexeme);

                // Move past token if one found
                cursor += len;
                matched = true;
                regfree(&regex);
                break;
            }
            
            regfree(&regex);
        }
        
        // Add unknown token if no match
        if (!matched)
        {
            char unknown[2] = {*cursor, '\0'};
            printf("UNKNOWN TOKEN: %d\n", *cursor);
            add_token(TOKEN_UNKNOWN, unknown, lineNum);
            cursor++;
        }
    }
}


/**
 * Tokenises a a whole file and populates the tokens array with Token structs
 * @param filePath A string of the filePath of the file to tokenise
 * @see
 * @return 0 if successful tokenisation, 1 if tokenisation failed
 */
int tokenise(const char *filePath) 
{
    FILE *scriptPtr;
    indentStack[indentTop++] = 0;
    int lineNum = 1;

    // Attempt to open file
    scriptPtr = fopen(filePath, "r");
    if (scriptPtr == NULL) 
    {
        fprintf(stderr, "Error opening file");
        return 1;
    }

    // Read and tokenise each line
    char line[1024];
    while (fgets(line, sizeof(line), scriptPtr) != NULL) 
    {
        tokenise_line(line, lineNum++);
    }

    // Add remaining DEDENTs
    while (indentTop > 1)
    {
        indentTop--;
        add_token(TOKEN_DEDENT, "<DEDENT>", lineNum);
    }

    add_token(TOKEN_EOF, "<EOF>", lineNum);

    fclose(scriptPtr);
    return 0;
}


/**
 * Frees the tokens in the list from memory
 * @param None
 * @see
 * @return
 */
void free_token_list()
{
    int i;

    for (int i = 0; i < tokenCount; i++) 
    {
        free(tokens[i].lexeme);
    }
}


/**
 * Prints the token list
 * @param None
 * @see
 * @return
 */
void print_token_list()
{   
    int i;

    for (int i = 0; i < tokenCount; i++) 
    {
        printf("Line %2d: %-12s %s\n", tokens[i].lineNum,
               (tokens[i].type == TOKEN_IDENTIFIER) ? "IDENTIFIER" :
               (tokens[i].type == TOKEN_NUMBER) ? "NUMBER" :
               (tokens[i].type == TOKEN_STRING) ? "STRING" :
               (tokens[i].type == TOKEN_OPERATOR) ? "OPERATOR" :
               (tokens[i].type == TOKEN_PAREN) ? "PAREN" :
               (tokens[i].type == TOKEN_COLON) ? "COLON" :
               (tokens[i].type == TOKEN_NEWLINE) ? "NEWLINE" :
               (tokens[i].type == TOKEN_INDENT) ? "INDENT" :
               (tokens[i].type == TOKEN_DEDENT) ? "DEDENT" :
               (tokens[i].type == TOKEN_EOF) ? "EOF" : "UNKNOWN",
               tokens[i].lexeme);
    }
}


int main(int argc, char **argv)
{
    // Check for correct useage
    if (argc != 2)
    {
        fprintf(stderr, "Incorrect Useage: %s <script.py>\n", argv[0]);
        return 1;
    }

    tokenise(argv[1]);

    print_token_list();
    free_token_list();

    return 0;
}
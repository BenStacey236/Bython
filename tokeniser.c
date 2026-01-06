#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdbool.h>

#include "TokenList.h"

#define MAX_INDENT_STACK_SIZE 100      // Max amount of indentation levels
#define NUM_TOKEN_PATTERNS 9           // The number of token patterns in the TOKEN_PATTERNS array


// Array of regex patterns for different tokens
static const char *TOKEN_PATTERNS[] = {
    "^[a-zA-Z_][a-zA-Z0-9_]*",                      // TOKEN_IDENTIFIER
    "^[0-9]+(\\.[0-9]+)?",                          // TOKEN_NUMBER
    "^\"([^\"\\\\]|\\\\.)*\"|^'([^'\\\\]|\\\\.)*'", // TOKEN_STRING

    "^=",                                           // TOKEN_ASSIGNMENT                  
    "^[\\+-\\*/=<>]=?",                           // TOKEN_OPERATOR
    "^[({]",                                        // TOKEN_LPAREN
    "^[})]",                                        // TOKEN_RPAREN
    "^:",                                           // TOKEN_COLON

    "^\r?\n"                                        // TOKEN_NEWLINE
};


// Array of token types matching TOKEN_PATTERNS array
static const TokenType TYPES[] = {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,

    TOKEN_ASSIGNMENT,
    TOKEN_OPERATOR,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_COLON,

    TOKEN_NEWLINE
};


static int indentStack[MAX_INDENT_STACK_SIZE];
static int indentTop = 0;


/**
 * Handles changes in indentation and when to add indent and dedent tokens
 * 
 * @param tokenList A pointer to the TokenList tokens should be added to
 * @param newIndent The new indentation level of the current line
 * @param lineNum An integer line number currently operating on
 */
static void handle_indentation(TokenList *tokenList, int newIndent, int lineNum) 
{
    int current_indent = indentStack[indentTop - 1];

    if (newIndent > current_indent)
    {
        // Indent token if indentation is higher than the previous indent
        indentStack[indentTop++] = newIndent;
        append_token(tokenList, TOKEN_INDENT, "<INDENT>", lineNum);
    } 
    else 
    {   
        // Dedent token if indentation is lower than the previous indent
        while (newIndent < current_indent)
        {
            indentTop--;
            current_indent = indentStack[indentTop - 1];
            append_token(tokenList, TOKEN_DEDENT, "<DEDENT>", lineNum);
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
 * 
 * @param tokenList A pointer to the TokenList tokens should be added to
 * @param line A pointer to the start of the line
 * @param lineNum An integer line number of the current line
 */
static void tokenise_line(TokenList *tokenList, char *line, int lineNum)
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
    handle_indentation(tokenList, indent, lineNum);

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
                append_token(tokenList, TYPES[i], lexeme, lineNum);
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
            append_token(tokenList, TOKEN_UNKNOWN, unknown, lineNum);
            cursor++;
        }
    }
}


TokenList *tokenise(const char *filePath) 
{
    FILE *scriptPtr;
    indentStack[indentTop++] = 0;
    int lineNum = 1;

    // Attempt to open file
    scriptPtr = fopen(filePath, "r");
    if (scriptPtr == NULL) 
    {
        fprintf(stderr, "Error opening file");
        return NULL;
    }

    // Create a new TokenList
    TokenList *tokenList = new_tokenlist();

    // Read and tokenise each line
    char line[1024];
    while (fgets(line, sizeof(line), scriptPtr) != NULL) 
    {
        tokenise_line(tokenList, line, lineNum++);
    }

    // Add remaining DEDENTs
    while (indentTop > 1)
    {
        indentTop--;
        append_token(tokenList, TOKEN_DEDENT, "<DEDENT>", lineNum);
    }

    append_token(tokenList, TOKEN_EOF, "<EOF>", lineNum);

    fclose(scriptPtr);
    return tokenList;
}

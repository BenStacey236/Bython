#include <stdio.h>

#include "tokeniser.h"


int main(int argc, char **argv)
{
    // Check for correct useage
    if (argc != 2)
    {
        fprintf(stderr, "Incorrect Usage: %s <script.py>\n", argv[0]);
        return 1;
    }

    TokenList *tokenList = tokenise(argv[1]);

    print_tokenlist(tokenList);
    free_tokenlist(tokenList);

    return 0;
}
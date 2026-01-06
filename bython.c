#include <stdio.h>

#include "tokeniser.h"
#include "ByDict.h"


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

    // Dictionary demo
    ByDict *dict = bydict_new();

    bydict_put(dict, "name", "Ben");
    bydict_print(dict);
    bydict_put(dict, "age", "18");
    printf("Name from dict: %s\n", bydict_get(dict, "name"));
    printf("Age from dict: %s\n", bydict_get(dict, "age"));
    bydict_put(dict, "age", "19");
    printf("New age from dict (should be overridden): %s\n", bydict_get(dict, "age"));
    bydict_put(dict, "home", "Chester");
    bydict_put(dict, "uni", "Nottingham");
    bydict_print(dict);

    bydict_delete(dict);

    return 0;
}
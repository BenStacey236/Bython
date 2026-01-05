#ifndef TOKENISER_H
#define TOKENISER_H

#include "TokenList.h"

/**
 * Tokenises a whole file, populating and returning the tokens in a `TokenList`
 * 
 * @param filePath A string of the filePath of the file to tokenise
 * @return A pointer to the TokenList containing all of the tokens from the tokenised file
 */
TokenList *tokenise(const char *filePath);

#endif // TOKENISER_H
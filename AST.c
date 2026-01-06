#include <stdio.h>
#include <stdlib.h>

#include "AST.h"

void free_AST(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case AST_NUMBER:
            break;

        case AST_BINARY_OP:
            free_AST(node->nodeVal.binary.left);
            free_AST(node->nodeVal.binary.right);
            break;

        default:
            break;
    }

    free(node);
}


void print_AST_rpn(ASTNode *node) {
    if (!node)
        return;

    switch (node->type)
    {
        case AST_NUMBER:
            printf("%g ", node->nodeVal.number.value);
            break;

        case AST_BINARY_OP:
            print_AST_rpn(node->nodeVal.binary.left);
            print_AST_rpn(node->nodeVal.binary.right);
            printf("%c ", node->nodeVal.binary.op);
            break;

        default:
            break;
    }
}

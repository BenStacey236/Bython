#ifndef AST_H
#define AST_H

/**
 * Enum defining AST node types
 */
typedef enum
{
    AST_NUMBER,         // Number node
    AST_BINARY_OP       // Binary operator node

} ASTNodeType;


/**
 * Structure for a single node of an AST
 */
typedef struct ASTNode ASTNode;
typedef struct ASTNode
{
    ASTNodeType type;
    int lineNum;

    union
    {
        /**
         * A Number literal node
         */
        struct
        {
            double value;
        } number;

        /**
         * A binary operator node
         */
        struct
        {
            char op;
            ASTNode *left;
            ASTNode *right;
        } binary;

    } nodeVal;

} ASTNode;


/**
 * Frees an AST from the root node downwards
 * 
 * @param node The root of the AST to free, or the node you want to free from
 */
void free_AST(ASTNode *node);


/**
 * Prints out the reverse-polish notation of an AST
 * 
 * @param node The root node of the AST to print
 */
void print_AST_rpn(ASTNode *node);

#endif // AST_H
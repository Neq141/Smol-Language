#ifndef PARSER_H_
#define PARSER_H_

#include "str.h"
#include "tokenizer.h"
#include <stdint.h>

#define NO_CONTENT (void *)0xDED // defined as the garbage value
#define NOT_SET_YET                                                            \
    (void *)0xDAD // defined as the garbage value for not set vars

#define EXHAUST_TOKEN(i)            (*(i)++)
#define EXHAUST_TOKENS(i, how_many) (*(i) += (how_many))
#define WITHOUT_SPACE               1
#define WITH_SPACE                  0

enum Parse_Token {
    ROOT = -1,
    // MARKDOWN
    TEXT = 0,
    MD_TOK_HEADER_1,
    MD_TOK_HEADER_2,
    MD_TOK_HEADER_3,
    MD_TOK_HEADER_4,
    MD_TOK_HEADER_5,
    MD_TOK_HEADER_6,

    MD_TOK_BOLD,
    MD_TOK_ITALIC,
    MD_TOK_BOLD_ITALIC,

    MD_TOK_BLOCK_QUOTE,

    MD_TOK_ORDERED_LIST,
    MD_TOK_UNORDERED_LIST,

    MD_TOK_CODE_SHORT,
    MD_TOK_CODE_LONG,

    MD_TOK_HORIZONTAL_RULE,
    MD_TOK_LINK,
    MD_TOK_IMAGE,

    // !MARKDOWN
};

// if you have the time optimize this to use void*'s or generalize it or idk :p
typedef struct {
    char *contents;
    int   number; // if number < 0, then LIST is unordered
} MD_LIST;

typedef struct {
    char *contents;
    char *language_name;
} MD_CODE_LONG;

typedef struct {
    char *text;
    char *link;
} MD_LINK;

typedef struct {
    char *text;
    char *link;
} MD_IMAGE;

typedef struct {
    char *text;
} MD_TOK;

typedef struct AST_node {
    enum Parse_Token type;
    void            *contents;
    struct AST_node *child;
    struct AST_node *next_sibling;
} AST_node;

// prototypes

AST_node new_node(
    enum Parse_Token type, void *contents, struct AST_node *child,
    struct AST_node *next_sibling
);

void append_child(AST_node *parent, AST_node *child);
void append_sibling(AST_node *parent, AST_node *sibling);


// print routines
void printMD_LIST(const MD_LIST *md_list);
void printMD_CODE_LONG(const MD_CODE_LONG *md_code_long);
void printMD_LINK(const MD_LINK *md_link);
void printMD_IMAGE(const MD_IMAGE *md_image);
void printMD_TOK(const MD_TOK *md_tok);
void printAST_node(const AST_node *ast_node);

// parse routines

// expect this token
bool expect_token(
    lexical_token const *source_tok, enum Lexical_Tokens token_type, LTA *array,
    size_t index_of_source_tok, bool without_space
);

// is this token the first character in the line (except from tabs and spaces)
bool is_first_in_line(lexical_token const *tok, str *contents);

// get characters until it's breaker has been satisfied
str walk_until(size_t pos_in_file, lexical_token const *breaker, str *contents);
str walk_until_end_of_line(size_t pos_in_file, const char *contents);
#endif // PARSER_H_

#ifndef NEWLANG_LEXER_H
#define NEWLANG_LEXER_H

static const size_t MaxIdentifierLength = 256;
static const size_t MaxOperatorLength = 3;
static char *ReservedKeywords[] = {
        "if", "else", "var", "return", "break",
        "continue", "while", "void", "function",
        "import", "namespace", "bool", "string",
        "array", "typeof", "as", "with", "goto",
};

static char *ReservedOperators[] = {
        "<<=", ">>=", "...", "==", "!=", "<=", ">=",
        "->", "++", "--", "<<", ">>", "+=", "-=", "*=",
        "/=", "&&", "||", "&=", "|=", "^="
};

typedef enum {
    TokenKind_Operator,
    TokenKind_Identifier,
    TokenKind_Reserved,
    TokenKind_Number,
    TokenKind_String,
} TokenKind;

typedef struct Token Token;
struct Token {
    TokenKind kind;
    Token *next;
    long integerValue;
    char *value;
    int length;
    
    char *contents; // String literal contents including terminating '\0'
    size_t contentsLength;  // string literal length
};

void Tokenize();

void OutputTokens();

Token *GetHeadToken();

#endif

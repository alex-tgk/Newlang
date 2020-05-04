#include "input.h"
#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <ctype.h>


Token *head = NULL;
char *user_input = NULL;

static bool startsWith(char *p, char *q) {
    return strncmp(p, q, strlen(q)) == 0;
}


static bool isLetter(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

static bool isLetterOrNumber(char c) {
    return isLetter(c) || ('0' <= c && c <= '9');
}

static char *startsWithKeyword(char *p) {
    // Keyword
    for (int i = 0; i < sizeof(ReservedKeywords) / sizeof(*ReservedKeywords); i++) {
        int len = strlen(ReservedKeywords[i]);
        if (startsWith(p, ReservedKeywords[i]) && !isLetterOrNumber(p[len])) {
            return ReservedKeywords[i];
        }
    }
    return NULL;
}

static char *startsWithOperator(char *p) {
    for (int i = 0; i < sizeof(ReservedOperators) / sizeof(*ReservedOperators); i++) {
        if (startsWith(p, ReservedOperators[i])) {
            return ReservedOperators[i];
        }
    }
    return NULL;
}

static bool isWhitespace(char ch) {
    return ch == ' ' || ch == '\n';
}

static int isStringLiteral(char ch) {
    return ch == '\'' || ch == '"' || ch == '`';
}

/**
 * Creates a pointer to and allocates memory for the next token.
 * Sets current->next to the new token.
 * @param kind      TokenKind for current token.
 * @param current   Current token pointer in linked list.
 * @param value     Token value.
 * @param length    Length of value.
 * @return          Pointer to new token with next set to NULL;
 */
static Token *newToken(TokenKind kind, Token *current, char *value, int length) {
    Token *token = (Token *) malloc(sizeof(Token));
    token->next = NULL;

    current->kind = kind;
    current->value = value;
    current->length = length;
    current->next = token;

    return token;
}

void Tokenize(char *buffer) {
    user_input = buffer;
    char *p = user_input;
    assert(p != NULL);
    head = (Token *) malloc(sizeof *head);
    Token *token = head;
    printf("Beginning tokenization step.\n");
    while (*p) {
        /**
         * End of File
         */
        if (*p == EOF) {
            break;
        }
        /**
         * Single Line Comment
         */
        if (startsWith(p, "//")) {
            p += 2;
            while (*p != '\n')
                p++;
            continue;
        }
        /**
         * Block Comment
         */
        // Skip block comments.
        if (startsWith(p, "/*")) {
            char *q = strstr(p + 2, "*/");
            p = q + 2;
            continue;
        }
        /**
         * Whitespace
         */
        if (isWhitespace(*p)) {
            p++;
            continue;
        }
        /**
         * String Literals
         */
        if (isStringLiteral(*p)) {
            char closingChar = *p;
            char *q = ++p;
            size_t length = 1; //length starts at one to allow space for terminating null;
            while (*p != closingChar) {
                length++;
                p++;
            }
            char *contents = (char *) malloc(length * sizeof(char));
            strncpy(contents, q, p - q);
            contents[length] = '\0';
            token->contents = contents;
            token->contentsLength = length;
            token = newToken(TokenKind_String, token, q, p - q);
            p++;
            continue;
        }
        /**
         * Keywords
         */
        char *keyword = startsWithKeyword(p);
        if (keyword) {
            int len = strlen(keyword);
            token = newToken(TokenKind_Reserved, token, p, len);
            p += len;
            continue;
        }
        /**
         * Operators
         */
        char *operator = startsWithOperator(p);
        if (operator) {
            int len = strlen(operator);
            token = newToken(TokenKind_Operator, token, p, len);
            p += len;
            continue;
        }
        /**
         * Single Letter Operators
         */
        if (ispunct(*p)) {
            assert(*p != '\'' && *p != '"');
            token = newToken(TokenKind_Operator, token, p++, 1);
            continue;
        }
        /**
         * Numbers
         */
        if (isdigit(*p)) {
            char *start = p;
            char *numericalValue = p;
            while (isdigit(*p)) {
                p++;
            }
            long val = strtol(numericalValue, &numericalValue, 10);
            token->integerValue = val;
            token = newToken(TokenKind_Number, token, start, p - start);
            continue;
        }
        /**
         * Identifiers
         */
        if (isalpha(*p)) {
            char *q = p++;
            while (isLetterOrNumber(*p))
                p++;
            token = newToken(TokenKind_Identifier, token, q, p - q);
            continue;
        }

        printf("Unexpected input.");
        exit(1);
    }
    printf("Exiting tokenization step. \n");
}

void OutputTokens() {
    Token *current = head;
    printf("Printing Tokens. \n");
    while (current != NULL) {
        char *value = (char *) malloc(current->length * sizeof(char));
        strncpy(value, current->value, current->length);
        value[current->length] = '\0';
        switch (current->kind) {
            case TokenKind_Identifier:
                printf("Identifier: %s \n", value);
                break;
            case TokenKind_Reserved:
                printf("Reserved Keyword: %s \n", value);
                break;
            case TokenKind_Operator:
                printf("Operator: %s \n", value);
                break;
            case TokenKind_String:
                printf("String: %s \n", value);
                break;
            case TokenKind_Number:
                printf("Number: %s \n", value);
                break;
            default:
                break;
        }
        current = current->next;
    }
    printf("Done printing tokens. \n");
}

Token *GetHeadToken() {
    return head;
}


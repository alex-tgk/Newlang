#include "input.h"
#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <ctype.h>


Token *head = NULL;

static Token *newToken(TokenKind kind, Token *cur, char *str, int len) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->value = str;
    tok->length = len;
    cur->next = tok;
    return tok;
}

static bool startswith(char *p, char *q) {
    return strncmp(p, q, strlen(q)) == 0;
}

static bool isAlphaNumeric(int c) {
    return isalpha(c) || (0 <= c && c <= 9);
}

static bool isKeyword(char *p) {
    for (int i = 0; i < sizeof(ReservedKeywords) / sizeof(*ReservedKeywords); i++) {
        if (startswith(p, ReservedKeywords[i])) {
            return true;
        }
    }
    return false;
}

static bool isWhitespace(int ch) {
    return ch == ' ' || ch == '\n';
}

static int isStringLiteral(int ch) {
    return ch == '\'' || ch == '"' || ch == '`';
}

static Token *allocateNextToken(Token *current) {
    current->next = (Token *) malloc(sizeof(Token));
    current = current->next;
    current->next = NULL;
    return current;
}

static void assertNotEOF(int ch) { assert(ch != EOF); }

void LexOpenFile() {
    head = (Token *) malloc(sizeof *head);
    Token *current = head;
    int ch = 0;
    printf("Beginning tokenization step.\n");
    while ((ch = fgetc(get_file()))) {
        if (ch == EOF) {
            break;
        } else if (isWhitespace(ch)) {
            continue;
        } else if (isStringLiteral(ch)) {
            int closingChar = ch;
            size_t arrSize = 1024 * sizeof(ch);
            char *value = (char *) malloc(arrSize);
            int length = 0;
            for (;;) {
                ch = fgetc(get_file());
                if (ch == EOF || ch == closingChar) {
                    break;
                }
                if (length >= arrSize) {
                    value = (char *) malloc(sizeof(value) * 2);
                }
                value[length++] = (char) ch;
                arrSize++;
            }
            value[length++] = '\0';
            current->value = value;
            current->length = length;
            current->kind = TokenKind_String;
        } else if (ispunct(ch)) {
            assert(ch != '\'' && ch != '"');
            char *value = malloc(2 * sizeof(ch));
            value[0] = (char) ch;
            value[1] = '\0';
            current->value = value;
            current->length = 1;
            current->kind = TokenKind_Operator;
        } else if (isdigit(ch)) {
            int val = ch - '0';
            while (isdigit(ch = fgetc(get_file())))
                val = val * 10 + (ch - '0');
            current->numericValue = val;
            current->kind = TokenKind_Number;
        } else if (isalpha(ch)) {
            char *value = malloc(MaxIdentifierLength);
            int length = 0;
            value[0] = (char) ch;
            while (isalpha(ch) || isdigit(ch)) {
                assert(ch != EOF);
                value[++length] = (char) (ch = fgetc(get_file()));
            }
            value[++length] = '\0';
            current->value = value;
            current->length = length;
            current->kind = isKeyword(value) ? TokenKind_Reserved : TokenKind_Identifier;
        }
        current = allocateNextToken(current);
    }
    printf("Exiting tokenization step. \n");
}

void OutputTokens() {
    Token *current = head;
    printf("Printing Tokens. \n");
    while (current != NULL) {
        switch (current->kind) {
            case TokenKind_Identifier:
                printf("Identifier: %s \n", current->value);
                break;
            case TokenKind_Reserved:
                printf("Reserved Keyword: %s \n", current->value);
                break;
            case TokenKind_Operator:
                printf("Operator: %s \n", current->value);
                break;
            case TokenKind_String:
                printf("String: %s \n", current->value);
                break;
            case TokenKind_Number:
                printf("Number: %ld \n", current->numericValue);
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


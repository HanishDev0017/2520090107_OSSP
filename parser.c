#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100

/* =========================
   TOKEN STRUCTURE
   ========================= */

typedef struct {
    char *value;
    char type[20];
} Token;

/* =========================
   IDENTIFY TOKEN TYPE
   ========================= */

void get_token_type(const char *token, char *type) {

    if (strcmp(token, "|") == 0)
        strcpy(type, "PIPE");

    else if (strcmp(token, ">") == 0)
        strcpy(type, "REDIRECT");

    else if (strcmp(token, "<") == 0)
        strcpy(type, "REDIRECT");

    else if (strcmp(token, "&&") == 0)
        strcpy(type, "OPERATOR");

    else
        strcpy(type, "WORD");
}

/* =========================
   TOKENIZE INPUT
   ========================= */

int tokenize(char *input, Token tokens[]) {

    int count = 0;
    int i = 0;

    while (input[i] != '\0') {

        /* Skip whitespace */
        while (isspace((unsigned char)input[i]))
            i++;

        if (input[i] == '\0')
            break;

        /* Handle special operators */
        if (input[i] == '|' ||
            input[i] == '>' ||
            input[i] == '<') {

            tokens[count].value = malloc(2);

            if (tokens[count].value == NULL)
                exit(1);

            tokens[count].value[0] = input[i];
            tokens[count].value[1] = '\0';

            get_token_type(tokens[count].value,
                           tokens[count].type);

            count++;
            i++;
        }

        /* Handle && */
        else if (input[i] == '&' && input[i + 1] == '&') {

            tokens[count].value = malloc(3);

            if (tokens[count].value == NULL)
                exit(1);

            strcpy(tokens[count].value, "&&");

            get_token_type(tokens[count].value,
                           tokens[count].type);

            count++;
            i += 2;
        }

        /* Handle normal words */
        else {

            int start = i;

            while (input[i] != '\0' &&
                   !isspace((unsigned char)input[i]) &&
                   input[i] != '|' &&
                   input[i] != '>' &&
                   input[i] != '<' &&
                   !(input[i] == '&' &&
                     input[i + 1] == '&')) {

                i++;
            }

            int length = i - start;

            tokens[count].value = malloc(length + 1);

            if (tokens[count].value == NULL)
                exit(1);

            strncpy(tokens[count].value,
                    &input[start],
                    length);

            tokens[count].value[length] = '\0';

            get_token_type(tokens[count].value,
                           tokens[count].type);

            count++;
        }

        if (count >= MAX_TOKENS)
            break;
    }

    return count;
}

/* =========================
   DISPLAY TOKENS
   ========================= */

void print_tokens(Token tokens[], int count) {

    printf("\n========== TOKEN STREAM ==========\n");

    for (int i = 0; i < count; i++) {

        printf("Token %d: %-10s Type: %s\n",
               i + 1,
               tokens[i].value,
               tokens[i].type);
    }
}

/* =========================
   VALIDATE TOKEN STREAM
   ========================= */

int validate_tokens(Token tokens[], int count) {

    if (count == 0) {
        printf("\nEmpty command detected.\n");
        return 0;
    }

    if (strcmp(tokens[0].value, "|") == 0) {
        printf("\nSyntax Error: command cannot start with '|'.\n");
        return 0;
    }

    if (strcmp(tokens[count - 1].value, "|") == 0) {
        printf("\nSyntax Error: command cannot end with '|'.\n");
        return 0;
    }

    for (int i = 0; i < count - 1; i++) {

        if (strcmp(tokens[i].value, "|") == 0 &&
            strcmp(tokens[i + 1].value, "|") == 0) {

            printf("\nSyntax Error: consecutive pipes.\n");
            return 0;
        }

        if ((strcmp(tokens[i].value, ">") == 0 ||
             strcmp(tokens[i].value, "<") == 0) &&
            i + 1 < count &&
            (strcmp(tokens[i + 1].value, "|") == 0 ||
             strcmp(tokens[i + 1].value, ">") == 0 ||
             strcmp(tokens[i + 1].value, "<") == 0)) {

            printf("\nSyntax Error: invalid redirection.\n");
            return 0;
        }
    }

    printf("\nToken stream is VALID.\n");
    return 1;
}

/* =========================
   PARSE TREE
   ========================= */

void generate_parse_tree(Token tokens[], int count) {

    printf("\n========== PARSE TREE ==========\n");

    printf("COMMAND\n");

    for (int i = 0; i < count; i++) {

        if (strcmp(tokens[i].value, "|") == 0) {

            printf("  |\n");
            printf("  +-- PIPE\n");
            printf("  |\n");
            printf("  +-- COMMAND\n");
        }

        else if (strcmp(tokens[i].value, ">") == 0 ||
                 strcmp(tokens[i].value, "<") == 0) {

            printf("  +-- REDIRECTION: %s\n",
                   tokens[i].value);
        }

        else if (strcmp(tokens[i].value, "&&") == 0) {

            printf("  +-- OPERATOR: &&\n");
            printf("  |\n");
            printf("  +-- COMMAND\n");
        }

        else {

            printf("  +-- WORD: %s\n",
                   tokens[i].value);
        }
    }
}

/* =========================
   EXECUTION STRUCTURE
   ========================= */

void execution_structure(Token tokens[], int count) {

    printf("\n======= EXECUTION STRUCTURE =======\n");

    printf("Program / Arguments:\n");

    for (int i = 0; i < count; i++) {

        if (strcmp(tokens[i].value, "|") == 0)
            printf("      [PIPE]\n");

        else if (strcmp(tokens[i].value, ">") == 0)
            printf("      [OUTPUT REDIRECTION]\n");

        else if (strcmp(tokens[i].value, "<") == 0)
            printf("      [INPUT REDIRECTION]\n");

        else if (strcmp(tokens[i].value, "&&") == 0)
            printf("      [AND OPERATOR]\n");

        else
            printf("      ARG: %s\n",
                   tokens[i].value);
    }
}

/* =========================
   FREE TOKENS
   ========================= */

void free_tokens(Token tokens[], int count) {

    for (int i = 0; i < count; i++)
        free(tokens[i].value);
}

/* =========================
   MAIN
   ========================= */

int main() {

    char input[500];

    Token tokens[MAX_TOKENS];

    printf("====================================\n");
    printf("       TOKENIZER AND PARSER\n");
    printf("====================================\n");

    printf("\nEnter a command:\n> ");

    if (fgets(input, sizeof(input), stdin) == NULL)
        return 1;

    int count = tokenize(input, tokens);

    print_tokens(tokens, count);

    if (validate_tokens(tokens, count)) {

        generate_parse_tree(tokens, count);

        execution_structure(tokens, count);
    }

    free_tokens(tokens, count);

    printf("\nMemory released successfully.\n");

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

/* =========================
   LINKED LIST FOR HISTORY
   ========================= */

typedef struct Node {
    char *command;
    struct Node *next;
} Node;

Node *head = NULL;
Node *tail = NULL;

/* Add command to history */
void add_history(const char *cmd) {
    Node *new_node = malloc(sizeof(Node));

    if (new_node == NULL) {
        perror("malloc");
        exit(1);
    }

    new_node->command = malloc(strlen(cmd) + 1);

    if (new_node->command == NULL) {
        perror("malloc");
        free(new_node);
        exit(1);
    }

    strcpy(new_node->command, cmd);
    new_node->next = NULL;

    if (head == NULL) {
        head = tail = new_node;
    } else {
        tail->next = new_node;
        tail = new_node;
    }
}

/* Free linked-list history */
void free_history(void) {
    Node *current = head;

    while (current != NULL) {
        Node *temp = current;
        current = current->next;

        free(temp->command);
        free(temp);
    }

    head = tail = NULL;
}

/* =========================
   TERMINAL RAW MODE
   ========================= */

struct termios original_terminal;

void disable_raw_mode(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal);
}

void enable_raw_mode(void) {
    tcgetattr(STDIN_FILENO, &original_terminal);
    atexit(disable_raw_mode);

    struct termios raw = original_terminal;

    raw.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

/* =========================
   DYNAMIC ARRAY DEMO
   ========================= */

void dynamic_array_demo(void) {
    int capacity = 2;
    int size = 0;

    int *array = malloc(capacity * sizeof(int));

    if (array == NULL) {
        perror("malloc");
        exit(1);
    }

    for (int i = 1; i <= 5; i++) {

        if (size == capacity) {
            capacity *= 2;

            int *temp = realloc(array, capacity * sizeof(int));

            if (temp == NULL) {
                free(array);
                perror("realloc");
                exit(1);
            }

            array = temp;
        }

        array[size++] = i * 10;
    }

    printf("\nDynamic array: ");

    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }

    printf("\nArray resized successfully.\n");

    free(array);
}

/* =========================
   SHOW HISTORY
   ========================= */

void show_history(void) {
    Node *current = head;
    int number = 1;

    printf("\n\nCommand History:\n");

    while (current != NULL) {
        printf("%d: %s\n", number++, current->command);
        current = current->next;
    }
}

/* =========================
   MAIN
   ========================= */

int main(void) {

    enable_raw_mode();

    printf("=====================================\n");
    printf("   COMMAND HISTORY DEMONSTRATION\n");
    printf("=====================================\n");
    printf("Type commands and press ENTER.\n");
    printf("UP    = previous command\n");
    printf("DOWN  = next command\n");
    printf("Type 'history' to show history.\n");
    printf("Type 'array' to test dynamic array.\n");
    printf("Type 'exit' to quit.\n\n");

    char *buffer = NULL;
    size_t capacity = 0;
    size_t length = 0;

    Node *history_position = NULL;

    while (1) {

        printf("> ");
        fflush(stdout);

        length = 0;

        capacity = 16;
        buffer = malloc(capacity);

        if (buffer == NULL) {
            perror("malloc");
            break;
        }

        buffer[0] = '\0';

        while (1) {

            char c;

            if (read(STDIN_FILENO, &c, 1) != 1) {
                continue;
            }

            /* ENTER */
            if (c == '\n' || c == '\r') {
                printf("\n");
                break;
            }

            /* BACKSPACE */
            if (c == 127 || c == 8) {
                if (length > 0) {
                    length--;
                    buffer[length] = '\0';

                    printf("\b \b");
                    fflush(stdout);
                }

                continue;
            }

            /* ESCAPE SEQUENCE */
            if (c == 27) {

                char seq[2];

                if (read(STDIN_FILENO, &seq[0], 1) != 1)
                    continue;

                if (read(STDIN_FILENO, &seq[1], 1) != 1)
                    continue;

                /* UP ARROW: ESC [ A */
                if (seq[0] == '[' && seq[1] == 'A') {

                    if (history_position == NULL) {
                        history_position = tail;
                    } else {
                        Node *current = head;

                        while (current != NULL &&
                               current->next != history_position) {
                            current = current->next;
                        }

                        if (current != NULL)
                            history_position = current;
                    }

                    if (history_position != NULL) {

                        while (length > 0) {
                            printf("\b \b");
                            length--;
                        }

                        strcpy(buffer, history_position->command);
                        length = strlen(buffer);

                        printf("%s", buffer);
                        fflush(stdout);
                    }
                }

                /* DOWN ARROW: ESC [ B */
                else if (seq[0] == '[' && seq[1] == 'B') {

                    if (history_position != NULL) {

                        if (history_position->next != NULL) {
                            history_position = history_position->next;

                            while (length > 0) {
                                printf("\b \b");
                                length--;
                            }

                            strcpy(buffer, history_position->command);
                            length = strlen(buffer);

                            printf("%s", buffer);
                            fflush(stdout);
                        }
                    }
                }

                continue;
            }

            /* NORMAL CHARACTER */

            if (length + 1 >= capacity) {

                capacity *= 2;

                char *temp = realloc(buffer, capacity);

                if (temp == NULL) {
                    free(buffer);
                    perror("realloc");
                    exit(1);
                }

                buffer = temp;
            }

            buffer[length++] = c;
            buffer[length] = '\0';

            putchar(c);
            fflush(stdout);
        }

        history_position = NULL;

        /* Empty command */
        if (length == 0) {
            free(buffer);
            continue;
        }

        /* Save command in linked-list history */
        add_history(buffer);

        if (strcmp(buffer, "history") == 0) {
            show_history();
        }

        else if (strcmp(buffer, "array") == 0) {
            dynamic_array_demo();
        }

        else if (strcmp(buffer, "exit") == 0) {
            free(buffer);
            break;
        }

        else {
            printf("You entered: %s\n", buffer);
        }

        free(buffer);
    }

    free_history();

    printf("\nAll dynamically allocated memory released.\n");

    return 0;
}

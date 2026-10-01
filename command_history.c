#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

typedef struct Node {
    char *command;
    struct Node *next;
} Node;

Node *head = NULL;
Node *tail = NULL;

void add_history(const char *cmd) {
    Node *new_node = malloc(sizeof(Node));

    if (new_node == NULL) exit(1);

    new_node->command = malloc(strlen(cmd) + 1);

    if (new_node->command == NULL) {
        free(new_node);
        exit(1);
    }

    strcpy(new_node->command, cmd);
    new_node->next = NULL;

    if (head == NULL)
        head = tail = new_node;
    else {
        tail->next = new_node;
        tail = new_node;
    }
}

void free_history() {
    Node *temp;

    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->command);
        free(temp);
    }
}

void show_history() {
    Node *temp = head;
    int i = 1;

    printf("\nCommand History:\n");

    while (temp != NULL) {
        printf("%d: %s\n", i++, temp->command);
        temp = temp->next;
    }
}

void dynamic_array_demo() {
    int capacity = 2;
    int size = 0;

    int *arr = malloc(capacity * sizeof(int));

    if (arr == NULL) exit(1);

    for (int i = 1; i <= 5; i++) {

        if (size == capacity) {
            capacity *= 2;

            int *temp = realloc(arr, capacity * sizeof(int));

            if (temp == NULL) {
                free(arr);
                exit(1);
            }

            arr = temp;
        }

        arr[size++] = i * 10;
    }

    printf("\nDynamic Array: ");

    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);

    printf("\nArray resized successfully.\n");

    free(arr);
}

int main() {

    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char *buffer = malloc(16);
    size_t capacity = 16;
    size_t length = 0;

    Node *current_history = NULL;

    printf("=================================\n");
    printf("     COMMAND HISTORY PROGRAM\n");
    printf("=================================\n");
    printf("Type commands and press ENTER\n");
    printf("UP = previous command\n");
    printf("DOWN = next command\n");
    printf("history = show history\n");
    printf("array = dynamic array test\n");
    printf("exit = quit\n\n");

    while (1) {

        printf("> ");
        fflush(stdout);

        length = 0;
        buffer[0] = '\0';
        current_history = NULL;

        while (1) {

            char c;

            if (read(STDIN_FILENO, &c, 1) != 1)
                continue;

            if (c == '\n' || c == '\r') {
                printf("\n");
                break;
            }

            if (c == 127) {

                if (length > 0) {
                    length--;
                    buffer[length] = '\0';
                    printf("\b \b");
                    fflush(stdout);
                }

                continue;
            }

            if (c == 27) {

                char seq1, seq2;

                if (read(STDIN_FILENO, &seq1, 1) != 1)
                    continue;

                if (read(STDIN_FILENO, &seq2, 1) != 1)
                    continue;

                if (seq1 == '[' && seq2 == 'A') {

                    if (current_history == NULL)
                        current_history = tail;
                    else {
                        Node *temp = head;

                        while (temp != NULL &&
                               temp->next != current_history)
                            temp = temp->next;

                        if (temp != NULL)
                            current_history = temp;
                    }

                    if (current_history != NULL) {

                        while (length > 0) {
                            printf("\b \b");
                            length--;
                        }

                        strcpy(buffer, current_history->command);
                        length = strlen(buffer);

                        printf("%s", buffer);
                        fflush(stdout);
                    }
                }

                else if (seq1 == '[' && seq2 == 'B') {

                    if (current_history != NULL &&
                        current_history->next != NULL) {

                        current_history = current_history->next;

                        while (length > 0) {
                            printf("\b \b");
                            length--;
                        }

                        strcpy(buffer, current_history->command);
                        length = strlen(buffer);

                        printf("%s", buffer);
                        fflush(stdout);
                    }
                }

                continue;
            }

            if (length + 1 >= capacity) {

                capacity *= 2;

                char *temp = realloc(buffer, capacity);

                if (temp == NULL) {
                    free(buffer);
                    free_history();
                    exit(1);
                }

                buffer = temp;
            }

            buffer[length++] = c;
            buffer[length] = '\0';

            putchar(c);
            fflush(stdout);
        }

        if (length == 0)
            continue;

        add_history(buffer);

        if (strcmp(buffer, "history") == 0)
            show_history();

        else if (strcmp(buffer, "array") == 0)
            dynamic_array_demo();

        else if (strcmp(buffer, "exit") == 0)
            break;

        else
            printf("You entered: %s\n", buffer);
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    free(buffer);
    free_history();

    printf("\nMemory released successfully.\n");

    return 0;
}

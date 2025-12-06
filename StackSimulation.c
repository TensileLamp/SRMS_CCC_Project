#include <stdio.h>
#include <stdlib.h>

#define MAX 10   // maximum size of stack

int stack[MAX];
int top = -1;

// Function to check if stack is full
int isFull() {
    return top == MAX - 1;
}

// Function to check if stack is empty
int isEmpty() {
    return top == -1;
}

// PUSH Operation
void push(int value) {
    if (isFull()) {
        printf("Stack Overflow! Cannot push %d\n", value);
    } else {
        top++;
        stack[top] = value;
        printf("%d pushed into the stack.\n", value);
    }
}

// POP Operation
int pop() {
    if (isEmpty()) {
        printf("Stack Underflow! Nothing to pop.\n");
        return -1;
    } else {
        int value = stack[top];
        top--;
        printf("%d popped from the stack.\n", value);
        return value;
    }
}

// PEEK Operation
int peek() {
    if (isEmpty()) {
        printf("Stack is empty. No top element.\n");
        return -1;
    } else {
        return stack[top];
    }
}

// DISPLAY Operation
void display() {
    if (isEmpty()) {
        printf("Stack is empty.\n");
    } else {
        printf("Current Stack:\n\n");
        for (int i = top; i >= 0; i--) {
            printf("| %d |\n", stack[i]);
        }
        printf("-----\n");
    }
}

int main() {
    int choice, value;

    while (1) {
        printf("\n======[ STACK MENU ]======\n");
        printf("1. Push\n");
        printf("2. Pop\n");
        printf("3. Peek\n");
        printf("4. Display\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter value to push: ");
                scanf("%d", &value);
                push(value);
                break;

            case 2:
                pop();
                break;

            case 3:
                value = peek();
                if (value != -1)
                    printf("Top element: %d\n", value);
                break;

            case 4:
                display();
                break;

            case 5:
                printf("Exiting program...\n");
                exit(0);

            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}

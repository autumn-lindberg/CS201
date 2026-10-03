#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// NODE
struct Node {
	int data;
	struct Node * next;
};
// constructor
struct Node * New_Node(int data) {
	struct Node * n = malloc(sizeof(struct Node));
	n->data = data;
	n->next = NULL;
	return n;
}
// destructor
void Delete_Node(struct Node * n) {
	free(n);
}

// LINKED LIST
struct LinkedList {
	int length;
	struct Node * head;
};
// constructor
struct LinkedList * New_Linked_List() {
	struct LinkedList * ll = malloc(sizeof(struct LinkedList));
	ll->length = 0;
	ll->head = NULL;
	return ll;
}
// destructor
void Delete_Linked_List(struct LinkedList * ll) {
	struct Node * current = ll->head;
	struct Node * copy;
	while (current != NULL) {
		// copy the pointer to current node
		copy = current;
		// go forward 1
		current = current->next;
		// free the copied node
		Delete_Node(copy);
	}
	// free the overarching list structure
	free(ll);
}
// functions
void push(struct LinkedList * ll, int data) {
	printf("\npushed %d \n\n", data);
	struct Node * newNode = New_Node(data);
	newNode->data = data;
	if (ll->head == NULL) {
		ll->head = newNode;
	}
	else {
		newNode->next = ll->head;
		ll->head = newNode;
	}
	ll->length++;
}
int pop(struct LinkedList * ll) {
	struct Node * headCopy = ll->head;
	if (headCopy == NULL) {
		return INT_MIN;
	}
	else {
		int data = headCopy->data;
		ll->head = ll->head->next;
		free(headCopy);
		return data;
	}
}
int peek(struct LinkedList * ll) {
	if (ll->head == NULL) return INT_MIN;
	return ll->head->data;
}
void print(struct LinkedList * ll) {
	struct Node * current = ll->head;
	if (current == NULL) {
		printf("stack is empty...\n\n");
	}
	else printf("\nCURRENT STACK\n");
	while (current != NULL) {
		printf("%d\n", current->data);
		current = current->next;
	}
	printf("\n");
}

void print_menu() {
	printf("ADD     -  push a number onto the stack\n");
	printf("REMOVE  -  pop a number off the stack\n");
	printf("PEEK    -  see what number is at the top of the stack\n");
	printf("DISPLAY -  show the current stack\n");
	printf("QUIT    -  end the program\n");
	printf("\n");
}

int get_int(char * message) {
	int input;
	printf("%s", message);
	// returns 0 if invalid input
	while (scanf("%d", &input) ==  0) {
		// absorb until newline
		while (getchar() != '\n') {}
		printf("Invalid input. Enter an integer: ");
	}
	// absorb newline for good input before returning
	getchar();
	return input;
}

char * get_string(char * message) {
	int length = 1;
	char current;
	char * input;

	printf("%s", message);
	// start with blank
	input = malloc(length);
	input[0] = '\0';
	do {
		// get input
		current = getc(stdin);
		if (current == '\n') break;
		else {
			// reallocate a bigger space
			input = realloc(input, length + 1);
			// add char to the end
			input[length - 1] = current;
			input[length] = '\0';
			length++;
		}
	} while (1);
	return input;
}

int main() {
	struct LinkedList * ll = New_Linked_List();
	char * choice;
	int input;
	int popped;
	int peeked;

	// MENU
	printf("Welcome to my stack program!\n\n");
	do {
		print_menu();
		choice = get_string("Enter a choice: ");
		if (strcmp(choice, "quit") == 0 || strcmp(choice, "QUIT") == 0) {
			printf("thank you for using my program!\n");
			free(choice);
			Delete_Linked_List(ll);
			return 1;
		}
		else if (strcmp(choice, "add") == 0 || strcmp(choice, "ADD") == 0) {
			input = get_int("Enter an integer: ");
			push(ll, input);
		}
		else if (strcmp(choice, "remove") == 0 || strcmp(choice, "REMOVE") == 0) { 
			popped = pop(ll);
			if (popped == INT_MIN) printf("pop failed. stack is empty.\n");
			else printf("popped %d\n\n", popped);
		}
		else if (strcmp(choice, "display") == 0 || strcmp(choice, "DISPLAY") == 0) {
			print(ll);
		}
		else if (strcmp(choice, "peek") == 0 || strcmp(choice, "PEEK") == 0) {
			peeked = peek(ll);
			if (peeked == INT_MIN) printf("stack is empty...\n\n");
			else printf("top of stack: %d\n\n", peeked);
		}
		else {
			printf("choice not recognized. try again.\n\n");
		}
		free(choice);
	} while (1);

	// test 1
	push(ll, 10);
	push(ll, 20);
	push(ll, 30);
	print(ll);
	printf("pop returned %d\n", pop(ll));
	printf("pop returned %d\n", pop(ll));
	printf("pop returned %d\n", pop(ll));
	print(ll);
	
	// test 2
	
}

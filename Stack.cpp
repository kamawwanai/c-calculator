#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "Stack.h"

#define MAX_LENGTH 2048
#define MAX_ELEMENT_SIZE 1024

Stack create_stack() {
	Stack stack_t;
	stack_t.capacity = MAX_LENGTH;
	stack_t.size = 0;
	stack_t.array = (char**)calloc(MAX_LENGTH,sizeof(char*));
	for (int i = 0; i < MAX_LENGTH; i++) {
		stack_t.array[i] = (char*)calloc(MAX_ELEMENT_SIZE, sizeof(char));
	}
	return stack_t;
}
void delete_stack(Stack* stack_t) {
	for (int i = 0; i < MAX_LENGTH; i++) {
		free(stack_t->array[i]);
	}
	free(stack_t->array);
}

bool is_overflow(Stack stack_t) {
	if (stack_t.size == stack_t.capacity)
		return true;
	return false;
}

 void resize_stack(Stack* stack_t) {
	 stack_t = (char**)realloc(stack_t, stack_t->capacity * 2);
	 stack_t->capacity *= 2;
	 for (int i = stack_t->size; i < stack_t->capacity; i++) {
		 stack_t->array[i] = (char*)calloc(MAX_ELEMENT_SIZE, sizeof(char));
	 }
	 assert(stack_t->array);
}

void push(Stack* stack_t, char* operation) {
	if (is_overflow(*stack_t)) {
		resize_stack(stack_t);
	}
	strcpy(stack_t->array[stack_t->size], operation);
	stack_t->size++;
}

void pop(Stack* stack_t) {
	stack_t->size--;
}

char* tail(Stack stack_t) {
	int last = stack_t.size - 1;
	return stack_t.array[last];
}

Stack add_several_instead_of_one(Stack* stack_t, Stack stack_added, char* elem) {
	int counter = 0;
	for (int i = 0; i < stack_t->size; i++) {
		if (strcmp(elem, stack_t->array[i]) == 0) {
			counter++;
		}
	}
	Stack stack_new = create_stack();
	for (int i = 0; i < stack_t->size; i++) {
		if (strcmp(elem, stack_t->array[i]) == 0) {
			push(&stack_new, "(");
			for (int j = 0; j < stack_added.size; j++) {
				push(&stack_new, stack_added.array[j]);
			}
			push(&stack_new, ")");
		}
		else {
			push(&stack_new, stack_t->array[i]);
		}
	}
	delete_stack(stack_t);
	return stack_new;
}
#pragma once
#include <stdbool.h>

typedef struct{
	int capacity, size;
	char** array;
}Stack;

Stack create_stack();

void delete_stack(Stack* stack_t);

bool is_overflow(Stack stack_t);

void resize_stack(Stack* stack_t);

void push(Stack* stack_t, char* operation);

void pop(Stack* stack_t);

char* tail(Stack stack_t);

Stack add_several_instead_of_one(Stack* stack_t, Stack stack_added, char* elem);
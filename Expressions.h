#pragma once

#include <stdbool.h>
#include "Stack.h"

typedef struct {
	char* name;
	Stack value;
}Variable;

bool check_brackets(char* expr);

char* get_math_expr();

void delete_spaces(char* expr);

Stack split_expr(char* expr);

bool if_unknown_variable(char* str);

bool is_unknown_in_stack(Stack expr);

void get_variables(Stack* infx_expr);

static bool check_alteration_type_of_element(char symbol);

bool in(char* str, char element);

bool is_operator(char* c);

bool is_function(char* c);

bool is_complex(char* c);

int priority(char c);

Stack to_rpn(Stack infx_expr);
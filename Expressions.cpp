#define _CRT_SECURE_NO_WARNINGS

#include "Expressions.h"
#include "Stack.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LENGTH 256
#define MAX_ELEMENT_SIZE 50

typedef enum {
	t_none,
	t_word,
	t_operator,
	t_value
} TypeOfElement;

bool check_brackets(char* expr) {
	int openbrackets = 0, closebrackets = 0;
	for (int i = 0; i < strlen(expr); i++) {
		if (expr[i] == '(') {
			openbrackets++;
		}
		else if (expr[i] == ')') {
			closebrackets++;
		}
	}
	return openbrackets == closebrackets;
}

char* get_math_expr() {
	char* expr=(char*)calloc(MAX_LENGTH, sizeof(char));
	fgets(expr, MAX_LENGTH, stdin);
	return expr;
}

void delete_spaces(char* expr) {
	int i_new = 0;
	int lenofexpr = strlen(expr);
	for (int i = 0; i < lenofexpr; i++) {
		if (expr[i] !=' '&&expr[i]!='\n') {
			expr[i_new++] = expr[i];
		}
	}
	expr[i_new] = '\0';
}

static bool check_alteration_type_of_element(char symbol) {
	static TypeOfElement element_type = t_none;
	if (symbol == '\0' ) {
		element_type = t_none;
	}

	TypeOfElement prev = element_type;
	char operations[] = "+-*/^()";

	if (symbol!='j' && (isalpha(symbol) || (prev == t_word && isdigit(symbol)))) {
		element_type = t_word;
	}
	else if (isdigit(symbol) || symbol == '.' || symbol == 'j') {
		element_type = t_value;
	}
	else if (in(operations, symbol)) {
		element_type = t_operator;
		return true;
	}
	else {
		element_type = t_none;
	}
	return element_type != prev;
}

Stack split_expr(char* expr) {

	delete_spaces(expr);

	Stack stack = create_stack();
	stack.size++;

	int sizeofcurrentelement = 0;

	int n = strlen(expr);

	for (int i = 0; i < n; i++) {
		if (check_alteration_type_of_element(expr[i]) && sizeofcurrentelement != 0) {
			stack.size++;
			sizeofcurrentelement = 0;
		}
		if (expr[i] == '-' && (i == 0 || expr[i - 1] == '(')) {
			stack.array[stack.size - 1][sizeofcurrentelement] = '0';
			stack.size++;
			sizeofcurrentelement = 0;
		}
		stack.array[stack.size - 1][sizeofcurrentelement++] = expr[i];
		for (int i = 0; i < stack.size; i++) {
			printf("%s ", stack.array[i]);
		}
	}

	return stack;
}

bool is_double(char* str)
{
	unsigned i = 0;
	unsigned length = strlen(str);
	int dec_count = 0;

	if (length == 0)
		return false;
	if (*str == '-') {
		if (length == 1)
			return false;
		i++;
	}
	if (str[i] == '.')
		return false;
	for (; i < length; i++) {
		if (str[i] == '.') {
			if (dec_count > 0 || !isdigit(str[i + 1]))
				return false;
			dec_count++;
		}
		else if (!isdigit(str[i]))
			return false;
	}
	return true;
}

bool if_unknown_variable(char* str) {
	if (is_complex(str) ||
		is_operator(str) ||
		is_function(str) ||
		strcmp(str,"PI")==0 ||
		strcmp(str, "j") == 0 ||
		strcmp(str, "e") == 0 ||
		strcmp(str, "(") == 0 ||
		strcmp(str, ")") == 0)
		return false;
	else {
		return true;
	}
}

bool is_unknown_in_stack(Stack expr) {
	for (int i = 0; i < expr.size; i++) {
		if (if_unknown_variable(expr.array[i])) {
			return true;
		}
	}
	return false;
}

void get_variables(Stack* infx_expr) {

	char* str = (char*)calloc(MAX_LENGTH, sizeof(char));

	Variable* variables = (Variable*)calloc(MAX_ELEMENT_SIZE, sizeof(Variable));
	int size = 0;

	while (fgets(str, MAX_LENGTH, stdin) != NULL) {

		delete_spaces(str);

		variables[size].name = (char*)calloc(MAX_ELEMENT_SIZE, sizeof(char));

		char* name_of_variable = (char*)calloc(MAX_ELEMENT_SIZE, sizeof(char));
		int size_of_name = 0;

		variables[size].value = create_stack();

		char* value_of_variable = (char*)calloc(MAX_LENGTH - MAX_ELEMENT_SIZE, sizeof(char));
		int size_of_value = 0;

		int size_of_str = strlen(str);
		bool flag = true;

		for (int i = 0; i < size_of_str; i++) {
			if (str[i] == '=') {
				flag = false;
				continue;
			}
			else if (flag) {
				name_of_variable[size_of_name] = str[i];
				size_of_name++;
			}
			else {
				value_of_variable[size_of_value] = str[i];
				size_of_value++;
			}
		}
		name_of_variable[size_of_name] = '\0';
		value_of_variable[size_of_value] = '\0';

		strcpy(variables[size].name, name_of_variable);
		check_alteration_type_of_element('\0');
		variables[size].value = split_expr(value_of_variable);
		size++;
	}
	int itterations = 0;
	while (is_unknown_in_stack(*infx_expr)) {
		for (int i = 0; i < infx_expr->size; i++) {
			if (if_unknown_variable(infx_expr->array[i])) {
				for (int j = 0; j < size; j++) {
					if (strcmp(variables[j].name,infx_expr->array[i])==0) {
						*infx_expr = add_several_instead_of_one(infx_expr, variables[j].value, variables[j].name);
						for (int i = 0; i < infx_expr->size; i++) {
							printf("%s ", infx_expr->array[i]);
						}
						printf("\n");
					}
				}
				itterations++;
			}
		}
		if (itterations >= 10000) {
			printf("Many unknown variables");
			_Exit(EXIT_SUCCESS);
		}
	}
}

bool in(char* str, char element) {
	for (int i = 0; i < strlen(str); i++) {
		if (element == str[i])
			return true;
	}
	return false;
}

bool is_operator(char* c) {
	const char* operators[] = { "+","-","/","*","^" };
	for (int i = 0; i < 5; i++) {
		if (strcmp(operators[i], c) == 0) {
			return true;
		}
	}
	return false;
}

bool is_function(char* c) {
	const char* functions[] = { "sin","cos","exp","ln","sqrt","real","imag","mag","phase" };
	for (int i = 0; i < 9; i++) {
		if (strcmp(functions[i], c) == 0) {
			return true;
		}
	}
	return false;
}

int priority(char* c) {
	if (strcmp(c, "+") == 0 || strcmp(c, "-") == 0)
		return 1;
	else if (strcmp(c, "*") == 0 || strcmp(c, "/") == 0)
		return 2;
	else if (strcmp(c, "^") == 0 || is_function(c))
		return 3;
	else
		return -1;
}

Stack to_rpn(Stack infx_expr) {

	Stack postfix_expr = create_stack();

	Stack stack = create_stack();

	for (int i = 0; i < infx_expr.size; i++) {
		if (!infx_expr.array[i] || !tail(stack)) {
			continue;
		}
		if (is_operator(infx_expr.array[i])) {
			while (stack.size != 0 && priority(tail(stack)) >= priority(infx_expr.array[i])) {
				push(&postfix_expr, tail(stack));
				pop(&stack);
			}
			push(&stack, infx_expr.array[i]);
		}
		else if (strcmp(infx_expr.array[i], "(") == 0) {
			push(&stack, "(");
		}
		else if (strcmp(infx_expr.array[i], ")") == 0) {
			while (strcmp(tail(stack), "(") != 0) {
				push(&postfix_expr, tail(stack));
				pop(&stack);
			}
			pop(&stack);
		}
		else if (is_function(infx_expr.array[i])) {
			push(&stack, infx_expr.array[i]);
		}
		else {
			push(&postfix_expr, infx_expr.array[i]);
		}
	}
	while (stack.size != 0) {
		push(&postfix_expr, tail(stack));
		pop(&stack);
	}
	delete_stack(&stack);
	return postfix_expr;
}

bool is_complex(char* c) {
	if (is_double(c)) {
		return true;
	}
	int n = strlen(c);
	for (int i = 0; i < n; ++i) {
		if (!isdigit(c[i]) && c[i] != 'j' && c[i] != '.') {
			return false;
		}
	}
	return true;
}
#include "Expressions.h"
#include <stdio.h>
#include "Stack.h"
#include <stdlib.h>
#include <complex.h>
#include "Calculate.h"

int main() {
	printf("Enter a expression; For example: a+b\n");
	printf("If there are unknown variables enter them like this: a=7.12\n");
	printf("Finish typing with Ctrl+Z and Enter\n");
	char* expr = get_math_expr();
	if (!check_brackets(expr)) {
		printf("Error: misspelling\n");
		return 0;
	}
	Stack math_expr = split_expr(expr);
	get_variables(&math_expr);
	printf("\n");
	Stack rpn_expr = to_rpn(math_expr);
	printf("\nCalculate result: ");
	_Dcomplex result = calculate_result(rpn_expr);
	printf("%lf + %lf j", creal(result),cimag(result));
	free(expr);
	delete_stack(&math_expr);
	delete_stack(&rpn_expr);
	return 0;
}
#include "Expressions.h"
#include "Stack.h"
#include "Calculate.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <complex.h>
#include <math.h>

#define PI 3.14159265358979323846
#define e exp(1)

_Dcomplex string_to_complex(char* string) {
    int n = strlen(string);
    bool is_imag = false;
    int j_pos = 0;
    for (int i = 0; i < n; ++i) {
        if (string[i] == 'j') {
            is_imag = true;
            j_pos = i;
            break;
        }
    }

    double real_part = 0;
    double imag_part = 0;

    if (!is_imag) {
        real_part = strtod(string, NULL);
    } else if(j_pos == 0) {
        imag_part = 1;
    } else {
        imag_part = strtod(string, NULL);
    }
	_Dcomplex out = {real_part, imag_part};
    return out;
}



_Dcomplex operation(char* operator, _Dcomplex a, _Dcomplex b) {
	if (strcmp(operator,"+") == 0) {
		return (_Dcomplex){creal(a) + creal(b), cimag(a) + cimag(b)};
	}
	else if (strcmp(operator,"-") == 0) {
		return (_Dcomplex){creal(a) - creal(b), cimag(a) - cimag(b)};
	}
	else if (strcmp(operator,"*") == 0) {
		return _Cmulcc(a, b);
	}
	else if (strcmp(operator,"/") == 0) {
		if (creal(b) == 0 && cimag(b)==0) {
			printf("Error: division by zero");
			_Exit(EXIT_SUCCESS);
		}
		_Dcomplex power = {-1.0, 0};
		return _Cmulcc(a, cpow(b, power));
	}
	else if (strcmp(operator,"^") == 0) {
		return cpow(a,b);
	}
}

_Dcomplex function(char* func, _Dcomplex a) {
	if (strcmp(func, "exp") == 0) {
		return cexp(a);
	}
	else if (strcmp(func, "ln") == 0) {
		return clog(a);
	}
	else if (strcmp(func, "cos") == 0) {
		return ccos(a);
	}
	else if (strcmp(func, "sin") == 0) {
		return csin(a);
	}
	else if (strcmp(func, "sqrt") == 0) {
		return csqrt(a);
	} else if (strcmp(func, "real") == 0) {
		return (_Dcomplex){creal(a),0.0 };
	} else if (strcmp(func, "imag") == 0) {
		return (_Dcomplex) {0.0, cimag(a)};
	} else if (strcmp(func, "mag") == 0) {
		return (_Dcomplex) { norm(a), 0.0 };
	} else if (strcmp(func, "phase") == 0) {
		return (_Dcomplex) { carg(a), 0.0 };
	}
}

_Dcomplex calculate_result(Stack expr) {
	CalculationStack calcstack;
	calcstack.value = (_Dcomplex*)calloc(expr.size, sizeof(_Dcomplex));
	calcstack.size = 0;
	for (int i = 0; i < expr.size; i++) {
		if (is_operator(expr.array[i])) {
			if (calcstack.size < 2) {
				printf("Error of calculation\n");
				_Exit(EXIT_SUCCESS);
			}
			calcstack.value[calcstack.size - 2] = 
				operation(expr.array[i],calcstack.value[calcstack.size - 2], calcstack.value[calcstack.size - 1]);
			calcstack.size--;
		}
		else if (is_function(expr.array[i])) {
			if (calcstack.size < 1) {
				printf("Error of calculation\n");
				_Exit(EXIT_SUCCESS);
			}
			calcstack.value[calcstack.size - 1] = function(expr.array[i], calcstack.value[calcstack.size - 1]);
		}
		else if (strcmp(expr.array[i], "PI") == 0) {
			_Dcomplex pi_complex = {PI, 0.0};
			calcstack.value[calcstack.size] = pi_complex;
			calcstack.size++;
		}
		else if (strcmp(expr.array[i], "e") == 0) {
			_Dcomplex e_complex = {e, 0.0};
			calcstack.value[calcstack.size] = e_complex;
			calcstack.size++;
		}
		else {
			calcstack.value[calcstack.size] = string_to_complex(expr.array[i]);
			calcstack.size++;
		}
	}
	if (calcstack.size != 1) {
		printf("Error of calculation\n");
		_Exit(EXIT_SUCCESS);
	}
	_Dcomplex answer = calcstack.value[calcstack.size - 1];
	return answer;
}
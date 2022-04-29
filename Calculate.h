#pragma once
#include <complex.h>

typedef struct {
	int size;
	_Dcomplex *value;
}CalculationStack;

_Dcomplex operation(char* operator, _Dcomplex a, _Dcomplex b);

_Dcomplex function(char* func, _Dcomplex a);

_Dcomplex calculate_result(Stack expr);

_Dcomplex string_to_complex(char* string);
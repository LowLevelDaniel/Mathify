#ifdef __cplusplus
#pragma once
#endif

#ifndef __MATHIFY_STRING_IMPLEMENTATION
#define __MATHIFY_STRING_IMPLEMENTATION

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "mathify.h"

bool stoi(struct mathify *obj, long long int *val, int base);
bool stou(struct mathify *obj, unsigned long long int *val, int base);
bool stof(struct mathify *obj, long double *val, int base);

#ifdef __cplusplus
}
#endif
#endif // __MATHIFY_STRING_IMPLEMENTATION
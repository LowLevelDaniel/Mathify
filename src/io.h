#ifndef XPU_IO_IMPLEMENTED
#define XPU_IO_IMPLEMENTED

#define SNL "\n"
#define NL '\n'

#include <stdio.h>

#define LOG(msg, ...) printf(msg SNL, ##__VA_ARGS__)
#define ERR(msg, ...) printf(msg SNL, ##__VA_ARGS__)
#define WARN(msg, ...) printf("WARNING: " msg SNL, ##__VA_ARGS__)

#ifdef __cplusplus
extern "C"
#endif
char getnext(FILE *srcf);

#endif // XPU_IO_IMPLEMENTED
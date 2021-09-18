#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include "app_fatfs.h"

#ifndef ARRAY_H
#define ARRAY_H
typedef FIL alloc_type;

typedef struct alloc
{
    size_t size;
    alloc_type *data;
    bool *empty;
    alloc_type *(*get)(struct alloc *this, size_t index);
    void (*del)(struct alloc *this, size_t index);
    size_t (*add)(struct alloc *this);
} Alloc;
struct alloc NewAlloc();
#endif
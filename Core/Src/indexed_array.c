#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include "indexed_array.h"

static size_t add_(struct alloc *this)
{
    if (this->size)
    {
        size_t i = 0;
        for (; i < this->size; i++)
            if (this->empty[i] == true)
                break;
        if (i == this->size)
        {
            this->size++;
            this->data = realloc(this->data, this->size * sizeof(alloc_type));
            this->empty = realloc(this->empty, this->size * sizeof(bool));
            this->empty[i] = false;
        }
        else
            this->empty[i] = false;
        return i;
    }
    else
    {
        this->size = 1;
        this->data = malloc(sizeof(alloc_type));
        this->empty = malloc(sizeof(bool));
        this->empty[0] = false;
        return 0;
    }
}

static alloc_type *get_(struct alloc *this, size_t index)
{
    if (index >= this->size)
        return NULL;
    return &(this->data[index]);
}

static void del_(struct alloc *this, size_t index)
{
    if (index >= this->size)
        return;
    this->empty[index] = true;
    if (index++ == this->size)
    {
        this->size--;
        this->data = realloc(this->data, this->size * sizeof(alloc_type));
        this->empty = realloc(this->empty, this->size * sizeof(bool));
    }
}

struct alloc NewAlloc()
{
    return (struct alloc){.size = 0, .empty = NULL, .data = NULL, .del = &del_, .get = &get_, .add = &add_};
}

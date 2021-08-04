#ifndef __FREE_Z_H
#define __FREE_Z_H
#include <stdio.h>
#include <string.h>
#include <malloc.h>

/* gcc -Wl,--wrap=free -Wl,--wrap=realloc */

void* __real_realloc(void *, size_t);

//zero memory even on error...
void* __wrap_realloc(void *ptr, size_t sz) {
    memset(ptr,0,malloc_usable_size(ptr));
    void *ret = __real_realloc(ptr,sz);
    if(!ret)free(ptr);
    return ret;
}

void __real_free(void *);

void __wrap_free(void *ptr) {
    memset(ptr,0,malloc_usable_size(ptr));
    //__real_free(ptr);
    ptr = NULL;
}
#endif
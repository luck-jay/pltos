#ifndef __PLT_MEM_H__
#define __PLT_MEM_H__

#include <plt/types.h>

void mem_init(void *begin_addr, void *end_addr);

void *mem_alloc(size_t size);

void mem_free(void *ptr);

void *mem_realloc(void *ptr, size_t size);

#define malloc(size)      mem_alloc(size)
#define free(p)           mem_free(p)
#define realloc(p, size)  mem_realloc(p, size)

#endif /* __PLT_MEM_H__ */

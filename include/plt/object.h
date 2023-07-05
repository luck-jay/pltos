#ifndef __PLT_OBJECT_H__
#define __PLT_OBJECT_H__

#include <plt/kernel.h>
#include <stdarg.h>
#include <plt/list.h>
#include "ref.h"

#define OBJECT_STATIC_MEM  0
#define OBJECT_DYNAMIC_MEM 1

enum object_type {
	OBJECT_TYPE_DRIVER,
	OBJECT_TYPE_BUS,
	OBJECT_TYPE_DEVICE,
	OBJECT_TYPE_UNKNOWN,
};

struct object {
	struct list list;
	enum object_type obj_t;
	// char *name;
	char name[SYS_NAME_MAX_SIZE];
	int dynamic_mem:1;
	struct ref ref;
};

static inline const char *object_name(const struct object *obj)
{
	return obj->name;
}

int object_set_name(struct object *obj,
						const char *fmt, va_list vargs);
void object_init_and_add(struct object *obj, const char *fmt, ...);
err_t object_creat_and_add(struct object *obj, const char *fmt, ...);
struct object *object_find(const char *name, struct list *head);
struct object *object_type_find(const char *name, const enum object_type obj_t);
void object_deinit(struct object *obj);
void object_destroy(struct object *obj);
void object_set_obj_type(struct object *obj, enum object_type obj_t);

#endif /* __PLT_OBJECT_H__ */

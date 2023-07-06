#include <stdio.h>
#include <string.h>
#include <plt/kernel.h>
#include <plt/object.h>
#include <plt/memory.h>

/* object对象列表 */
static struct list _obj_list[] = {
	LIST_INIT(_obj_list[OBJECT_TYPE_DRIVER]),
	LIST_INIT(_obj_list[OBJECT_TYPE_BUS]),
	LIST_INIT(_obj_list[OBJECT_TYPE_DEVICE]),
	LIST_INIT(_obj_list[OBJECT_TYPE_UNKNOWN]),
};

int object_set_name(struct object *obj,
						const char *fmt, va_list vargs)
{
	int n;

	// obj->name = (char *)malloc(SYS_NAME_MAX_SIZE);
	// if (obj->name == NULL) {
	// 	n = -1;
	// 	goto __err;
	// }

	n = vsnprintf(obj->name, SYS_NAME_MAX_SIZE, fmt, vargs);
	if (n >= SYS_NAME_MAX_SIZE)
		log_d("Object names are too long and will be truncated!");

// __err:
	return n;
}

/**
 * @brief 将一个定义的object对象初始化，并将该object对象添加到框架
 *
 * @param obj 定义的一个未初始化的object对象
 *
 * @param fmt object对象的名字
 */
void object_init_and_add(struct object *obj, const char *fmt, ...)
{
	va_list args;

	/* param check */
	assert(obj != NULL);

	if (fmt != NULL) {
		va_start(args, fmt);
		object_set_name(obj, fmt, args);
		va_end(args);
	}

	obj->dynamic_mem = OBJECT_STATIC_MEM;

	list_add_tail(&obj->list, &_obj_list[obj->obj_t]);

	init_ref(&obj->ref);

	log_d("Successfully added %s object to kernel", obj->name);
}

/**
 * @brief 创建一个object对象，并将该object对象添加到框架
 *
 * @param obj 指向object的指针，但并没有分配内存空间
 *
 * @param fmt object对象的名字
 *
 * @return 返回成功失败标志
 */
err_t object_creat_and_add(struct object *obj, const char *fmt, ...)
{
	err_t ret = 0;
	va_list args;

	obj = (struct object *)malloc(sizeof(struct object));
	if (obj == NULL) {
		ret = -ENOMEM;
		goto __nomem;
	}

	obj->dynamic_mem = OBJECT_DYNAMIC_MEM;
	obj->obj_t = OBJECT_TYPE_UNKNOWN;
	init_ref(&obj->ref);

	va_start(args, fmt);
	ret = object_set_name(obj, fmt, args);
	va_end(args);
	if (ret < 0) {
		ret = -ENOMEM;
		goto __noname;
	}

	list_add_tail(&obj->list, &_obj_list[obj->obj_t]);

	log_d("Successfully created and added the %s \
			object to the kernel", obj->name);

	return 0;

__noname:
	free(obj);
__nomem:
	log_w("Insufficient memory to create object objects!");

	return ret;
}

/**
 * @brief 通过链表头和名字查找object对象
 *
 * @param name object对象名字
 *
 * @param head object对象链表名字
 *
 * @return 返回查找到的object对象，如果不能查找到则返回NULL
*/
struct object *object_find(const char *name, struct list *head)
{
	struct object *obj;

	list_for_each_entry(obj, head, list) {
		if (strncmp(obj->name, name, SYS_NAME_MAX_SIZE) == 0)
			return obj;
	}

	return NULL;
}

/**
 * @brief 通过名字和类型找到一个对应的object对象
 *
 * @param name 需要寻找的object对象名字
 *
 * @param obj_t 需要寻找的object对象属于什么类型 eg: OBJECT_TYPE_DRIVER
 *              不能使用 OBJECT_TYPE_NUKNOWN
 *
 * @return object
 */
struct object *object_type_find(const char *name, const enum object_type obj_t)
{
	/* param check */
	assert(obj_t < OBJECT_TYPE_UNKNOWN);

	return object_find(name, &_obj_list[obj_t]);
}

/**
 * @brief 从框架中移除一个object对象
 *
 * @param obj 需要移除的object对象
 */
void object_deinit(struct object *obj)
{
	assert(obj != NULL);

	log_d("Remove a %s object from the kernel", obj->name);
	list_del(&obj->list);

	if (obj->dynamic_mem == OBJECT_DYNAMIC_MEM)
		log_w("For the wrong call, you should use object_destroy!");
}

/**
 * @brief 销毁一个object对象
 *
 * @param obj 需要销毁的object对象
 */
void object_destroy(struct object *obj)
{
	assert(obj != NULL);

	log_d("Remove a %s object from the kernel", obj->name);
	list_del(&obj->list);

	if (obj->dynamic_mem == OBJECT_DYNAMIC_MEM)
		free(obj);
	else
		log_w("For the wrong call, you should use object_deinit!");
}

/**
 * @brief 为object对象添加一个类型
 *
 * @param obj 需要添加类型的对象
 *
 * @param obj_t 为object添加的具体类型
 */
void object_set_obj_type(struct object *obj, enum object_type obj_t)
{
	/* param check */
	assert(obj != NULL);
	assert(obj_t < OBJECT_TYPE_UNKNOWN);

	obj->obj_t = obj_t;

	list_move_tail(&obj->list, &_obj_list[obj->obj_t]);
}

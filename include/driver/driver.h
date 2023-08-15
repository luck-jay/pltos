#ifndef __DRIVER_DRIVER_H__
#define __DRIVER_DRIVER_H__

#include <plt/object.h>

static inline struct object *find_bus_by_name(const char *name)
{
	return object_type_find(name, OBJECT_TYPE_BUS);
}

static inline struct object *find_driver_by_name(const char *name)
{
	return object_type_find(name, OBJECT_TYPE_DRIVER);
}

#endif /* __DRIVER_DRIVER_H__ */

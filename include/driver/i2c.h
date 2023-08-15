#ifndef __DRIVER_I2C_H__
#define __DRIVER_I2C_H__

#include <plt/object.h>
#include <plt/types.h>

#define I2C_CLIENT_FLAG_SUB_ADDR_16BIT  0x01
#define I2C_CLIENT_FLAG_SUB_ADDR_8BIT   0x02
#define I2C_CLIENT_FLAG_SUB_ADDR_IGN    0x04

#define I2C_BUS_FLAG_BUSY  0x01

#define I2C_DIRECTION_SEND  0x01
#define I2C_DIRECTION_READ  0x02

struct i2c_bus;
struct i2c_client;

struct i2c_msg {
	uint8_t *buf;
	uint8_t direction;
	size_t size;
	uint16_t addr;
};

struct i2c_ops {
	int (*transfer) (struct i2c_client *client, struct i2c_msg *msg, int num);
};

struct i2c_bus {
	struct object parent;
	const char *label;
	const struct i2c_ops *ops;
	int speed;
	uint16_t addr;
	int timeout;
	uint16_t flags;
};

struct i2c_client {
	struct object parent;
	struct i2c_bus *bus;
	const char *label;
	uint16_t flags;
	uint16_t addr;
};

int i2c_bus_register(struct i2c_bus *bus, uint16_t flags);
void i2c_bus_unregister(struct i2c_bus *bus);
int i2c_bus_attach_client(struct i2c_client *client, const char *bus_name);
int i2c_bus_transfer(struct i2c_client *client, struct i2c_msg *msgs, int num);

static inline struct i2c_bus *object_to_i2c_bus(struct object *obj)
{
	return container_of(obj, struct i2c_bus, parent);
}

static inline struct i2c_client *object_to_i2c_client(struct object *obj)
{
	return container_of(obj, struct i2c_client, parent);
}

#endif /* __DRIVER_I2C_H__ */

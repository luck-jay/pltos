#include <plt/kernel.h>
#include <driver/i2c.h>

/**
 * @brief 注册一个i2c总线
 *
 * @param bus 需要注册的总线
 *
 * @param flags 一些标志位
 */
int i2c_bus_register(struct i2c_bus *bus, uint16_t flags)
{
	bus->parent.obj_t = OBJECT_TYPE_BUS;
    object_init_and_add(&bus->parent, bus->label);
    bus->flags = flags;
    return 0;
}

/**
 * @brief 注销一个i2c总线
 *
 * @param bus 需要注销的总线
 */
void i2c_bus_unregister(struct i2c_bus *bus)
{
	object_deinit(&bus->parent);
}

/**
 * @brief 将i2c client绑定要i2c bus上
 *
 * @param client 需要绑定的i2c客户端
 *
 * @param bus_name 需要绑定的总线名字
 *
 * @return < 0则是失败
 */
int i2c_bus_attach_client(struct i2c_client *client, const char *bus_name)
{
	struct object *bus_obj;

	bus_obj = object_type_find(bus_name, OBJECT_TYPE_BUS);
	if (bus_obj == NULL) {
		log_w("Bus %s not found.", bus_name);
		return -1;
	}

	client->bus = object_to_i2c_bus(bus_obj);

	client->parent.obj_t = OBJECT_TYPE_DRIVER;
	object_init_and_add(&client->parent, client->label);

	return 0;
}

/**
 * @brief i2c总线数据传输函数
 *
 * @param client 需要传输数据的i2c客户端
 *
 * @param msgs 需要传输的数据列表
 *
 * @param num 需要传输的数据个数
 *
 * @return 实际传输的数据个数, 或错误代码
 */
int i2c_bus_transfer(struct i2c_client *client, struct i2c_msg *msgs, int num)
{
	int ret = 0;
	struct i2c_bus *bus;

	if (client == NULL)
		return -EINVAL;

	if (client->bus == NULL)
		return -EFAULT;

	if (num == 0)
		return num;

	bus = client->bus;

	ret = bus->ops->transfer(client, msgs, num);

	return ret;
}

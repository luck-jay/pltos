#include <driver/spi.h>

/**
 * @brief 注册一个spi总线
 *
 * @param spi 需要注册的总线
 *
 * @param flags 一些标志位
 */
int spi_bus_register(struct spi_bus *bus, uint16_t flags)
{
	bus->parent.obj_t = OBJECT_TYPE_BUS;
    object_init_and_add(&bus->parent, bus->label);
    bus->flags = flags;
    return 0;
}

/**
 * @brief 注销一个spi总线
 *
 * @param bus 需要注销的总线
 */
void spi_bus_unregister(struct spi_bus *bus)
{
	object_deinit(&bus->parent);
}

/**
 * @brief 将spi client绑定要spi bus上
 *
 * @param client 需要绑定的spi客户端
 *
 * @param bus_name 需要绑定的总线名字
 *
 * @return < 0则是失败
 */
int spi_bus_attach_client(struct spi_client *client, const char *bus_name)
{
	struct object *bus_obj;
	
	bus_obj = object_type_find(bus_name, OBJECT_TYPE_BUS);
	if (bus_obj == NULL) {
		log_w("Bus %s not found.", bus_name);
		return -1;
	}
	
	client->bus = object_to_spi_bus(bus_obj);
	
	client->parent.obj_t = OBJECT_TYPE_DRIVER;
	object_init_and_add(&client->parent, client->label);
	
	return 0;
}

/**
 * @brief spi总线数据传输函数
 *
 * @param client 需要传输数据的spi客户端
 *
 * @param msgs 需要传输的数据列表
 *
 * @param num 需要传输的数据个数
 *
 * @return 实际传输的数据个数, 或错误代码
 */
int spi_bus_transfer(struct spi_client *client, uint8_t *buf, size_t size)
{
	int ret = 0;
	struct spi_bus *bus;

	if (client == NULL)
		return -EINVAL;
	
	if (client->bus == NULL)
		return -EFAULT;
	
	if (size == 0)
		return size;
	
	bus = client->bus;
	
	ret = bus->ops->transfer(client, buf, size);
	
	return ret;
}

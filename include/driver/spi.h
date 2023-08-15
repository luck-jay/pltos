#ifndef __DRIVER_SPI_H__
#define __DRIVER_SPI_H__

#include <plt/kernel.h>
#include <plt/object.h>

#define SPI_CS_DISABLE 0
#define SPI_CS_ENABLE  1

struct spi_bus;

struct spi_client {
	struct object parent;
	const char *label;
	void (*cs_pin) (uint8_t enable);
	struct spi_bus *bus;
};

struct spi_ops {
	int (*transfer) (struct spi_client *client, uint8_t *buf, size_t size);
};

struct spi_bus {
	struct object parent;
	const char *label;
	const struct spi_ops *ops;
	uint16_t flags;
};

int spi_bus_register(struct spi_bus *bus, uint16_t flags);
void spi_bus_unregister(struct spi_bus *bus);
int spi_bus_attach_client(struct spi_client *client, const char *bus_name);
int spi_bus_transfer(struct spi_client *client, uint8_t *buf, size_t size);

static inline struct spi_bus *object_to_spi_bus(struct object *obj)
{
	return container_of(obj, struct spi_bus, parent);
}

static inline struct spi_client *object_to_spi_client(struct object *obj)
{
	return container_of(obj, struct spi_client, parent);
}

#endif /* __DRIVER_SPI_H__ */

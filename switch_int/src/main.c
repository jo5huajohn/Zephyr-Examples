#include <zephyr.h>
#include <drivers/gpio.h>
#include <sys/printk.h>

#define LED_NODE    DT_ALIAS(led)
#define SW_NODE     DT_ALIAS(sw)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
static const struct gpio_dt_spec sw = GPIO_DT_SPEC_GET(SW_NODE, gpios);
static struct gpio_callback sw_cb;

int init(void);
void button(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

void main(void)
{
    if (init() < 0)
    {
        return;
    }

    while (1)
    {
    }
}

int init(void)
{
    if (!device_is_ready(led.port))
    {
        return -1;
    }

    if (!device_is_ready(sw.port))
    {
        return -1;
    }

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT) < 0)
    {
        return -1;
    }

    if (gpio_pin_configure_dt(&sw, GPIO_INPUT | GPIO_PULL_DOWN) < 0)
    {
        return -1;
    }

    if (gpio_pin_interrupt_configure_dt(&sw, GPIO_INT_EDGE_RISING) < 0)
    {
        return -1;
    }

    gpio_init_callback(&sw_cb, button, BIT(sw.pin));

    if (gpio_add_callback(sw.port, &sw_cb) < 0)
    {
        return -1;
    }

    return 0;
}

void button(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    printk("Button Pressed\n");
}
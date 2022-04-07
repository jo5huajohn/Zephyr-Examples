#include <zephyr.h>
#include <drivers/gpio.h>
#include <sys/printk.h>

int cnt = 0;

static const struct gpio_dt_spec e1_A = GPIO_DT_SPEC_GET(DT_ALIAS(e1a), gpios);
static const struct gpio_dt_spec e1_B = GPIO_DT_SPEC_GET(DT_ALIAS(e1b), gpios);
static struct gpio_callback encoder_cb; /* Callback Structure for the interrupt */

int init(void);
void encoder_interrupt(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

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
    if (!device_is_ready(e1_A.port))
    {
        return -1;
    }

    if (!device_is_ready(e1_B.port))
    {
        return -1;
    }

    if (gpio_pin_configure_dt(&e1_A, GPIO_INPUT | GPIO_PULL_DOWN) < 0)
    {
        return -1;
    }

    /* Configure interrupt to happen on active edge. */
    if (gpio_pin_interrupt_configure_dt(&e1_A, GPIO_INT_EDGE_TO_ACTIVE))
    {
        return -1;
    }

    gpio_init_callback(&encoder_cb, encoder_interrupt, BIT(e1_A.pin));

    if (gpio_add_callback(e1_A.port, &encoder_cb) < 0)
    {
        return -1;
    }

    if (gpio_pin_configure_dt(&e1_B, GPIO_INPUT | GPIO_PULL_DOWN) < 0)
    {
        return -1;
    }

    return 0;
}

void encoder_interrupt(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    (gpio_pin_get_dt(&e1_A) != gpio_pin_get_dt(&e1_B)) ? cnt++ : cnt--;
    printk("%d\n", cnt);
}
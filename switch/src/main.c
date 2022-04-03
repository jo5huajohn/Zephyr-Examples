#include <zephyr.h>
#include <drivers/gpio.h>
#include <sys/printk.h>

#define LED0_NODE    DT_ALIAS(led)
#define SW_NODE     DT_ALIAS(sw)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec sw = GPIO_DT_SPEC_GET(SW_NODE, gpios);

int init(void);

void main(void)
{
    int new_state = 0;
    int prev_state = 0;
    
    if (init() < 0)
    {
        return;
    }

    while (1)
    {
        new_state = gpio_pin_get_dt(&sw);

        if (new_state && !prev_state)
        {
            gpio_pin_toggle_dt(&led);
        }

        prev_state = new_state;
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

    return 0;
}

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
    
    if (init() < 0) /* Ensures that all the drivers are ready and configured. */
    {
        return;
    }

    while (1)
    {
        new_state = gpio_pin_get_dt(&sw); /* Get the logical level of the GPIO input pin. */ 

        if (new_state && !prev_state)
        {
            gpio_pin_toggle_dt(&led); /* Toggle the GPIO output pin (LED). */
        }

        prev_state = new_state;
    }
}

int init(void)
{
    if (!device_is_ready(led.port))
    {
        printk("LED Port is not ready.\n");
        return -1;
    }

    if (!device_is_ready(sw.port))
    {
        printk("Switch Port is not ready.\n");
        return -1;
    }

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT) < 0)
    {
        printk("Could not configure LED pin.\n");
        return -1;
    }

    if (gpio_pin_configure_dt(&sw, GPIO_INPUT | GPIO_PULL_DOWN) < 0)
    {
        printk("Could not configure switch pin.]n");
        return -1;
    }

    return 0;
}

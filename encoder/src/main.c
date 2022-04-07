#include <zephyr.h>
#include <drivers/gpio.h>
#include <sys/printk.h>

#define E1_A_NODE   DT_ALIAS(e1a)
#define E1_B_NODE   DT_ALIAS(e1b)

int aState;
int aLastState;
int cnt = 0;

static const struct gpio_dt_spec e1_A = GPIO_DT_SPEC_GET(E1_A_NODE, gpios);
static const struct gpio_dt_spec e1_B = GPIO_DT_SPEC_GET(E1_B_NODE, gpios);

int init(void);

void main(void)
{
    if (init() < 0)
    {
        return;
    }

    while (1)
    {
        aState = gpio_pin_get_dt(&e1_A);    /* Latest state */

        if (aState != aLastState)
        {
            (gpio_pin_get_dt(&e1_B) != aState) ? cnt++ : cnt--;
            printk("Counter: %d\n", cnt);
        }
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

    if (gpio_pin_configure_dt(&e1_B, GPIO_INPUT | GPIO_PULL_DOWN) < 0)
    {
        return -1;
    }

    aLastState = gpio_pin_get_dt(&e1_A);

    return 0;
}
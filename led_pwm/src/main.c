#include <zephyr.h>
#include <drivers/pwm.h>
#include <device.h>
#include <sys/printk.h>

#define PWM_LED_NODE    DT_ALIAS(pwmled)
#define PWM_CTLR        DT_PWMS_CTLR(PWM_LED_NODE)
#define PWM_CHANNEL     DT_PWMS_CHANNEL(PWM_LED_NODE)
#define PWM_FLAGS       DT_PWMS_FLAGS(PWM_LED_NODE) 

#define FREQ_IN_HZ      5
#define PERIOD_IN_USEC  (USEC_PER_SEC / FREQ_IN_HZ)
#define NUM_OF_STEPS    50
#define STEP_IN_USEC    (PERIOD_IN_USEC / NUM_OF_STEPS)
#define SLEEP_IN_MSEC   10


int init(const struct device *dev);

void main(void)
{
    int dir = 1;
    int pulse_width = 0;

    const struct device *pwm = DEVICE_DT_GET(PWM_CTLR);

    if (init(pwm) < 0)
    {
        printk("PWM device could not be initialised.");
        return;
    }

    while (1)
    {
        printk("Pulse Width: %d\n", pulse_width);
        printk("Period: %d\n", PERIOD_IN_USEC);

        /* Set the period of the PWM signal along with any flags that were declared. */
        if (pwm_pin_set_usec(pwm, PWM_CHANNEL, PERIOD_IN_USEC, pulse_width, PWM_FLAGS) < 0)
        {
            printk("Failed to set pulse width\n");

            return;
        }

        /* If dir = 1, increment pulse_width by STEP_IN_USEC microseconds.
         * If pulse_width is greater than or equal to PERIOD_IN_USEC, set
         * pulse width to PERIOD_IN_USEC - STEP_IN_USEC and set dir to 0
         */
        if (dir)
        {
            pulse_width += STEP_IN_USEC;

            if (pulse_width >= PERIOD_IN_USEC)
            {
                pulse_width = PERIOD_IN_USEC - STEP_IN_USEC;
                dir = 0;
            }
        }

        /* While pulse_width is greater than STEP_IN_USEC, decrement pulse_width
         * by STEP_IN_USEC. Else set pulse_width to STEP_IN_USEC and set dir to 1.
         */
        else
        {
            if (pulse_width >= STEP_IN_USEC)
            {
                pulse_width -= STEP_IN_USEC;
            }

            else
            {
                pulse_width = STEP_IN_USEC;
                dir = 1;
            }
        }

        k_msleep(SLEEP_IN_MSEC);
    }
}

int init(const struct device *dev)
{
    if (!device_is_ready(dev))
    {
        printk("PWM device %s is not ready", dev->name);

        return -1;
    }

    return 0;
}
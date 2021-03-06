#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/* mraa header */
#include "mraa/gpio.h"

#define GPIO_PIN 8

void
int_handler(void* args)
{
    mraa_gpio_context g = (mraa_gpio_context)args;
    fprintf(stdout, "ISR triggered %d\n", mraa_gpio_read(g));
}

int
main()
{
    mraa_result_t status = MRAA_SUCCESS;
    mraa_gpio_context gpio;

    /* initialize mraa for the platform (not needed most of the times) */
    mraa_init();

    //! [Interesting]
    /* initialize GPIO pin */
    gpio = mraa_gpio_init(GPIO_PIN);
    if (gpio == NULL) {
        fprintf(stderr, "Failed to initialize GPIO %d\n", GPIO_PIN);
        mraa_deinit();
        return EXIT_FAILURE;
    }

    /* set GPIO to input */
    status = mraa_gpio_dir(gpio, MRAA_GPIO_IN);
    if (status != MRAA_SUCCESS) {
        goto err_exit;
    }

    /* configure ISR for GPIO */
    status = mraa_gpio_isr(gpio, MRAA_GPIO_EDGE_BOTH, &int_handler, gpio);
    if (status != MRAA_SUCCESS) {
        goto err_exit;
    }

    /* wait 30 seconds isr trigger */
    sleep(30);

    /* close GPIO */
    mraa_gpio_close(gpio);

    //! [Interesting]
    /* deinitialize mraa for the platform (not needed most of the times) */
    mraa_deinit();

    return EXIT_SUCCESS;

err_exit:
    mraa_result_print(status);

    /* deinitialize mraa for the platform (not needed most of the times) */
    mraa_deinit();

    return EXIT_FAILURE;
}

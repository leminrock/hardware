/* standard headers */
#include <stdlib.h>
#include <unistd.h>

/* mraa header */
#include "mraa/gpio.h"

#define GPIO_PIN 3

int counter = 0;

void int_handler(void* args)
{
    //fprintf(stdout, "ISR triggered\n\n");
    printf("RELEASED %d \n", counter);
    counter++;
    sleep(2);
    printf("passato");
}

int main()
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

    status = mraa_gpio_edge_mode(gpio, MRAA_GPIO_ACTIVE_LOW);
    if (status != MRAA_SUCCESS) {
    	goto err_exit;
    }

    /* configure ISR for GPIO */
    status = mraa_gpio_isr(gpio, MRAA_GPIO_EDGE_RISING, &int_handler, NULL);
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

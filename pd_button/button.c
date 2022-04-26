#include "m_pd.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/* mraa header */
#include "mraa/gpio.h"

static t_class* button_class;

typedef struct _button {
    t_object x_obj;
    volatile sig_atomic_t flag;
    mraa_result_t status;
    mraa_gpio_context gpio_1;
    int counter;
    int pin;
    int old_value;
} t_button;

void interrupt_handler_routine(void* args);

// callback func
void interrupt_handler_routine(void* args)
{
    t_button* x = (t_button*)args;

    int value = mraa_gpio_read(x->gpio_1);

    if (value != x->old_value) {
        if (!value && x->old_value) {
            post("PRESSED %d TIMES ON PIN %d", ++x->counter, x->pin);
        }
        /*
        else {
            post("\t\tRELEASED");
        }
        */
        x->old_value = value;
    }
}

/* gestisce l'uscita */

void sig_handler(t_button* x, t_int signum)
{
    if (signum == SIGINT) {
        post("Exiting...\n");
        x->flag = 0;
    }
}

void button_free(t_button* x)
{
    x->status = mraa_gpio_isr_exit(x->gpio_1);
    x->status = mraa_gpio_close(x->gpio_1);
    mraa_deinit();
}

void* button_new(t_floatarg f)
{
    t_button* x = (t_button*)pd_new(button_class);
    x->flag = 1;

    x->status = MRAA_SUCCESS;

    /* install signal handler */
    // signal(SIGINT, sig_handler);

    /* initialize mraa for the platform (not needed most of the times) */
    mraa_init();

    int pin_number = (int)f;
    x->gpio_1 = mraa_gpio_init(pin_number);

    if (x->gpio_1 == NULL) {
        post("Failed to initialize GPIO %d", pin_number);
        mraa_deinit();
    }

    // set GPIO to input
    x->status = mraa_gpio_dir(x->gpio_1, MRAA_GPIO_IN);

    if (x->status != MRAA_SUCCESS)
        post("ERROR ON SETTING DIRECTION");

    x->status
        = mraa_gpio_isr(x->gpio_1, MRAA_GPIO_EDGE_BOTH, &interrupt_handler_routine, (void*)x);

    if (x->status != MRAA_SUCCESS)
        post("WRONG ISR BINDING");

    x->counter = 0;
    x->pin = (int)f;
    x->old_value = -1;
    post("PIN NUMBER IS %d", x->pin);
    post("object correctly initialized...");
    post("cleck per secs %ld", CLOCKS_PER_SEC);
    return (void*)x;
}

void button_setup(void)
{
    button_class = class_new(gensym("button"), (t_newmethod)button_new, (t_method)button_free, sizeof(t_button), CLASS_DEFAULT, A_DEFFLOAT, 0);
}

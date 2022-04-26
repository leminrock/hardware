#include "m_pd.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/* mraa header */
#include "mraa/gpio.h"

/* gpio declaration */
#define GPIO_PIN_1 8
#define DEBOUNCE 4000

static t_class* button_class;

typedef struct _button {
    t_object x_obj;
    volatile sig_atomic_t flag;
    mraa_result_t status;
    mraa_gpio_context gpio_1;
    int counter;
    clock_t start_t;
    int pin;
    int old_value;
} t_button;

void int_handler(void* args);

// callback func
void int_handler(void* args)
{
    t_button* x = (t_button*)args;

    /*
    clock_t t = clock();

    if ((t - x->start_t) > DEBOUNCE) {
        x->counter++;
        post("pin value: %d,\tcounter: %d,\tpin_number: %d", mraa_gpio_read(x->gpio_1), x->counter, x->pin);
        x->start_t = t;
    }
    */
    int value = mraa_gpio_read(x->gpio_1);

    if (value != x->old_value) {
        if (!value && x->old_value) {
            post("PRESSED");
        } else {
            post("\t\tRELEASED");
        }
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

    /* release gpio's */
    x->status = mraa_gpio_isr_exit(x->gpio_1);
    x->status = mraa_gpio_close(x->gpio_1);

    //! [Interesting]
    /* deinitialize mraa for the platform (not needed most of the times) */
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

    //! [Interesting]
    /* initialize GPIO pin */
    x->gpio_1 = mraa_gpio_init((int)f);

    if (x->gpio_1 == NULL) {
        post("Failed to initialize GPIO %d", (int)f);
        mraa_deinit();
    }

    /* set GPIO to output */
    x->status = mraa_gpio_dir(x->gpio_1, MRAA_GPIO_IN);
    if (x->status != MRAA_SUCCESS)
        post("ERROR ON SETTING DIRECTION");
    else
        post("RIGHT DIRECTION");

    x->status
        = mraa_gpio_isr(x->gpio_1, MRAA_GPIO_EDGE_BOTH, &int_handler, (t_button*)x);
    if (x->status != MRAA_SUCCESS)
        post("WRONG ISR BINDING");

    x->counter = 0;
    x->start_t = clock();
    x->pin = (int)f;
    x->old_value = -1;
    post("PIN NUMBER IS %d", x->pin);
    post("object correctly initialized...");
    post("cleck per secs %ld", CLOCKS_PER_SEC);
    return (void*)x;
}

void button_setup(void)
{
    // button_class = class_new(gensym("button"), (t_newmethod)button_new, (t_method)button_free, sizeof(t_button), CLASS_DEFAULT, 0);
    button_class = class_new(gensym("button"), (t_newmethod)button_new, (t_method)button_free, sizeof(t_button), CLASS_DEFAULT, A_DEFFLOAT, 0);
    // class_addbang(button_class, button_bang);
}

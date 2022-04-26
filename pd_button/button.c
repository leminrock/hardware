#include "m_pd.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* mraa header */
#include "mraa/gpio.h"

/* gpio declaration */
#define GPIO_PIN_1 8

static t_class* button_class;

typedef struct _button {
    t_object x_obj;
    volatile sig_atomic_t flag;
    t_int state;
    mraa_result_t status;
    mraa_gpio_context gpio_1;
} t_button;

void int_handler(void* args)
{
    // fprintf(stdout, "ISR triggered\n\n");
    post("bang");
    // sleep(0.1);
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
    x->status = mraa_gpio_close(x->gpio_1);

    //! [Interesting]
    /* deinitialize mraa for the platform (not needed most of the times) */
    mraa_deinit();
}

/*
void button_bang(t_button* x)
{
    //(void)x; // silence unused variable warning
    x->state = 1 - x->state;
    x->status = mraa_gpio_write(x->gpio_1, x->state);
    if (x->state == 1)
        post("ACCESA");
    else
        post("SPENTA");
}
*/

void* button_new(void)
{
    t_button* x = (t_button*)pd_new(button_class);
    x->flag = 1;
    x->state = 0;

    x->status = MRAA_SUCCESS;

    /* install signal handler */
    // signal(SIGINT, sig_handler);

    /* initialize mraa for the platform (not needed most of the times) */
    mraa_init();

    //! [Interesting]
    /* initialize GPIO pin */
    x->gpio_1 = mraa_gpio_init(GPIO_PIN_1);

    /* set GPIO to output */
    x->status = mraa_gpio_dir(x->gpio_1, MRAA_GPIO_IN);
    mraa_gpio_isr(x->gpio_1, MRAA_GPIO_EDGE_RISING, &int_handler, NULL);

    /*
     while (flag) {
         state = 1 - state;


         usleep(50000);
     }*/

    return (void*)x;
}

void button_setup(void)
{
    // button_class = class_new(gensym("button"), (t_newmethod)button_new, (t_method)button_free, sizeof(t_button), CLASS_DEFAULT, 0);
    button_class = class_new(gensym("button"), (t_newmethod)button_new, (t_method)button_free, sizeof(t_button), 0, A_GIMME, 0);
    // class_addbang(button_class, button_bang);
}

#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>

#define NUM_LEDS 3
#define NUM_BUTTONS 2
#define DEBOUNCE_MS 250

static struct priv {
    struct gpio_desc *led[NUM_LEDS], *button[NUM_BUTTONS];
    int current_pos;
    unsigned long last_time;
};

static irqreturn_t left_button_handler(int irq, void *priv_data)
{
    struct priv *my_data = (struct priv *) priv_data;

    if(jiffies_to_msecs(jiffies) - my_data->last_time < DEBOUNCE_MS) return IRQ_HANDLED;

    gpiod_set_value(my_data->led[my_data->current_pos], 0);

    if(my_data->current_pos > 0) my_data->current_pos--;
    else my_data->current_pos = NUM_LEDS-1;

    gpiod_set_value(my_data->led[my_data->current_pos], 1);

    my_data->last_time = jiffies_to_msecs(jiffies);
    pr_info("Left Interrupted\n");

    return IRQ_HANDLED;
}

static irqreturn_t right_button_handler(int irq, void *priv_data)
{
    struct priv *my_data = (struct priv *) priv_data;

    if(jiffies_to_msecs(jiffies) - my_data->last_time < DEBOUNCE_MS) return IRQ_HANDLED;

    gpiod_set_value(my_data->led[my_data->current_pos], 0);

    if(my_data->current_pos < NUM_LEDS-1) my_data->current_pos++;
    else my_data->current_pos = 0;

    gpiod_set_value(my_data->led[my_data->current_pos], 1);

    my_data->last_time = jiffies_to_msecs(jiffies);
    pr_info("Right Interrupted\n");

    return IRQ_HANDLED;
}

static int my_probe(struct platform_device *pdev)
{
    dev_info(&pdev->dev, "Hello World\n");

    struct priv *my_data = devm_kzalloc(&pdev->dev, sizeof(struct priv), GFP_KERNEL);
    if(!my_data) return -ENOMEM;

    for(int i=0; i<NUM_LEDS; ++i)
    {
        my_data->led[i] = devm_gpiod_get_index(&pdev->dev, "led", i, GPIOD_OUT_LOW);
        if(IS_ERR(my_data->led[i]))
        {
            dev_err(&pdev->dev, "Failed to get LED GPIO: %ld\n", PTR_ERR(my_data->led[i]));
            return PTR_ERR(my_data->led[i]);
        }
        dev_info(&pdev->dev, "LED %d state: %d\n", i, gpiod_get_value(my_data->led[i]));
    }
    for(int i=0; i<NUM_BUTTONS; ++i)
    {
        my_data->button[i] = devm_gpiod_get_index(&pdev->dev, "button", i, GPIOD_IN);
        if(IS_ERR(my_data->button[i]))
        {
            dev_err(&pdev->dev, "Failed to get button GPIO: %ld\n", PTR_ERR(my_data->button[i]));
            return PTR_ERR(my_data->button[i]);
        }
        dev_info(&pdev->dev, "Button %d state: %d\n", i, gpiod_get_value(my_data->button[i]));
    }
    my_data->current_pos = 0;
    my_data->last_time = jiffies_to_msecs(jiffies);

    // LEFT button
    int irq = gpiod_to_irq(my_data->button[1]);
    if(irq < 0) {
        dev_err(&pdev->dev, "Failed to get IRQ number for the button GPIO %d\n", irq);
        return irq;
    }
    irq = devm_request_irq(&pdev->dev, irq, left_button_handler, IRQF_TRIGGER_FALLING, "Left button", my_data);
    if (irq) {
        dev_err(&pdev->dev, "IRQ request failed\n");
        return irq;
    }

    // RIGHT button
    irq = gpiod_to_irq(my_data->button[0]);
    if(irq < 0) {
        dev_err(&pdev->dev, "Failed to get IRQ number for the button GPIO %d\n", irq);
        return irq;
    }
    irq = devm_request_irq(&pdev->dev, irq, right_button_handler, IRQF_TRIGGER_FALLING, "Right button", my_data);
    if (irq) {
        dev_err(&pdev->dev, "IRQ request failed\n");
        return irq;
    }

    return 0;
}

static void my_remove(struct platform_device *pdev)
{
    dev_info(&pdev->dev, "Goodbye\n");
}

static struct of_device_id my_match[] = {
    {.compatible = "custom,mydriver"},
    {}
};
MODULE_DEVICE_TABLE(of, my_match);

static struct platform_driver my_driver = {
    .probe = my_probe,
    .remove = my_remove,
    .driver = {
        .name = "my_driver",
        .of_match_table = my_match
    }
};

module_platform_driver(my_driver);

MODULE_LICENSE("GPL");
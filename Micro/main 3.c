#include<stdio.h>
#include<freertos/FreeRTOS.h>//Libreria que nos permite usar los delay
#include<freertos/task.h>
#include<driver/gpio.h> //libreria que nos permite el manejo dde kis oerifericos GPIO
#include<driver/adc.h>
#include <math.h>



#define LED 2
#define bt 17

adc1_channel_t adc_pot = ADC1_CHANNEL_6;

void init_esp(void);

void app_main() {

init_esp();
 
    float tiempo = 300/portTICK_PERIOD_MS; 
    int state = 0;
    int State_bt;
    int State_bt_anterior = 0;
    float voltaje = 0;

    while (1)
    {
        
        
        State_bt = gpio_get_level(bt); 
        
        if (State_bt == 1 && State_bt_anterior == 0)
        {
            state = !state;
        }
        

        if (state == 1)
        {
            int pot = adc1_get_raw(adc_pot); 
            voltaje = (pot*3300)/4095;
            voltaje = voltaje / 1000;
            gpio_set_level(LED, 1); 
            printf("\n ADC = %d", pot); 
            printf("    Voltaje = %.2f", voltaje);
            vTaskDelay(tiempo); 
        }
        else
        {
            gpio_set_level(LED, 0);
            printf("\nAPAGADO.");
        }
        
        
    }
    vTaskDelay(tiempo); 
    
}

void init_esp(void){

    gpio_config_t io_config;

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(adc_pot, ADC_ATTEN_DB_12);

    io_config.mode = GPIO_MODE_INPUT; 
    io_config.pin_bit_mask = (1 << bt); 
                                       
    io_config.pull_down_en = GPIO_PULLDOWN_ONLY; 
    io_config.pull_up_en = GPIO_PULLUP_DISABLE; 
    io_config.intr_type = GPIO_INTR_DISABLE; 
    gpio_config(&io_config);

    io_config.mode = GPIO_MODE_OUTPUT; 
    io_config.pin_bit_mask = (1 << LED); 
                                        
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE; 
    io_config.pull_up_en = GPIO_PULLUP_DISABLE; 
    io_config.intr_type = GPIO_INTR_DISABLE; 
    gpio_config(&io_config);
}
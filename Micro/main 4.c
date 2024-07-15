#include<freertos/FreeRTOS.h>
#include<stdio.h>
#include<freertos/task.h>
#include<driver/gpio.h> 
#include<driver/adc.h>
#include<driver/ledc.h>
#include <math.h>


#define LED 2
adc1_channel_t adc_pot = ADC1_CHANNEL_6;
ledc_channel_config_t PWM;
void init_esp(void);

void app_main() {


    init_esp();
 
    float tiempo = 100/portTICK_PERIOD_MS; 

    float voltaje = 0;

    while (1)
    {

            int pot = adc1_get_raw(adc_pot); 
            voltaje = (pot*3300)/4095;
            voltaje = voltaje / 1000;
            printf("\n ADC = %d", pot); 
            printf("    Voltaje = %.2f", voltaje);

            ledc_set_duty(PWM.speed_mode, PWM.channel, pot);
            ledc_update_duty(PWM.speed_mode, PWM.channel);

            vTaskDelay(tiempo); 
        
        
    }
    
}

void init_esp(void){

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(adc_pot, ADC_ATTEN_DB_12);

    ledc_timer_config_t timer = {

        .duty_resolution = LEDC_TIMER_12_BIT,
        .freq_hz = 1000,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .clk_cfg = LEDC_APB_CLK
    };

    ledc_timer_config(&timer);

    PWM.channel = LEDC_CHANNEL_0;
    PWM.duty = 0;
    PWM.gpio_num = LED;
    PWM.hpoint = 0;
    PWM.timer_sel = LEDC_TIMER_0;
    PWM.speed_mode = LEDC_HIGH_SPEED_MODE;

    ledc_channel_config(&PWM);
}


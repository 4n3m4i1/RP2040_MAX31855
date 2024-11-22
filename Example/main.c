#ifndef PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS
#define PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS (5000)
#endif


#include <stdio.h>
#include <pico/stdlib.h>
#include "../src/MAX31855_2040.h"

/*
    11-22-2024  Joseph A. De Vico

    Simple Test program to read some temps.
    All degrees C
*/

#define CSN     1
#define SPIRX   0
#define SCK     2

uint16_t hacky_fixed_point_converter(uint16_t in){
    uint16_t frac = in & 0x000F;
    uint16_t retval = 0;

    if(in & (1u << 3)) retval += 5000;
    if(in & (1u << 2)) retval += 2500;
    if(in & (1u << 1)) retval += 1250;
    if(in & (1u << 0)) retval +=  625;

    return retval;
}

void main(){

    stdio_init_all();
    //stdio_usb_init();

    busy_wait_ms(2000);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

//    struct MAX31855_DATA max;
//
//    MAX31855_SPI_INIT(&max, spi0, (1000u * 1000u), CSN, SPIRX, SCK);
//    MAX31855_INIT_DELAY();

    

    printf("Hello :)\n");

    gpio_put(PICO_DEFAULT_LED_PIN, 0);

    while(1){
//        MAX31855_Read_Blocking(spi0, &max);
//
//        uint16_t tcpl_frac = hacky_fixed_point_converter(*((uint16_t *)&max.thermocouple_temp));
//        uint16_t case_frac = hacky_fixed_point_converter(*((uint16_t *)&max.case_temp));
//
//        printf("TCPL: %4d.%d\tAMB: %4d.%d\n", MAX31855_FIX_2_INT(max.thermocouple_temp), tcpl_frac, MAX31855_FIX_2_INT(max.case_temp), case_frac);
        printf("Swap\n");
        gpio_put(PICO_DEFAULT_LED_PIN, ~gpio_get_out_level(PICO_DEFAULT_LED_PIN));


        busy_wait_ms(500);
    }

}
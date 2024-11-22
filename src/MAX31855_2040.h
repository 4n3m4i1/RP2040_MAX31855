#ifndef MAX31855_2040_h
#define MAX31855_2040_h

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <inttypes.h>

/*
    11-22-2024 Joseph A. De Vico

    MAX31855 SPI Thermocouple Library for RP2040/2350 Using SDK

    This converter continuously converts, and will only update
        output data registers when CS is HIGH.

    We can obtain new data every 100ms (70ms nominal, 100ms MAX, Citation: Thermal Characteristics in Datasheet).
*/

#define _MAX_31855_MAX_CVT_TIME_MS      100
#define _MAX_31855_NOM_CVT_TIME_MS      70

/*
    Fixed point temperature data is stored in the following format:

        DDDD DDDD DDDD . FFFF

    This is a signed Q16.4 representation.
*/

// Fixed Point Radix Position
#define MAX31855_RADIX_POS      4
// Fixed Point Conversion to Int (Discards Fraction)
#define MAX31855_FIX_2_INT(a)   (a >> MAX31855_RADIX_POS)
// Int Conversion to Fixed Point
#define MAX31855_INT_2_FIX(a)   (a << MAX31855_RADIX_POS)


// Bitfield shift position
enum MAX31855_FAULTS {
    MAX31855_FAULT_OPEN_CIRCUIT,
    MAX31855_FAULT_INPUTS_SHORT_TO_GND,
    MAX31855_FAULT_INPUTS_SHORT_TO_VCC,
    MAX31855_FAULT_PRESENT = 7
};

// Input data bit positions
enum MAX31855_BITS {
    MAX31855_OPEN_CIRCUIT,
    MAX31855_SHORT_TO_GND,
    MAX31855_SHORT_TO_VCC,
    MAX31855_RESERVED_0,
    MAX31855_CASE_TEMP_LSB,
    MAX31855_CASE_TEMP_MSB = 14,
    MAX31855_CASE_TEMP_SIGN,
    MAX31855_FAULT,
    MAX31855_RESERVED_1,
    MAX31855_TCPL_TEMP_LSB,
    MAX31855_TCPL_TEMP_MSB = 30,
    MAX31855_TCPL_TEMP_SIGN
};

union MAX_INGEST {
    uint32_t raw;
    uint8_t raw_bytes[4];
};


struct MAX31855_DATA {
    union MAX_INGEST ingest;
    int16_t thermocouple_temp;
    int16_t case_temp;
    uint8_t fault_bitfield;

    uint8_t cs_pin;
};


uint MAX31855_SPI_INIT(struct MAX31855_DATA *max_data, spi_inst_t *spi, uint baudrate, uint CS_PIN, uint SPI_RX_PIN, uint SPI_SCK_PIN);

void MAX31855_INIT_DELAY();

void __MAX31855_Convert_Raw_Read(struct MAX31855_DATA *max_data);

void MAX31855_Read_Blocking(spi_inst_t *spi, struct MAX31855_DATA *max_data);


#endif
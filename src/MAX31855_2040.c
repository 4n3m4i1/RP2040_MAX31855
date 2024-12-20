#include "MAX31855_2040.h"


// Initialize SPI Peripheral and Pins
uint MAX31855_SPI_INIT(struct MAX31855_DATA *max_data, spi_inst_t *spi, uint baudrate,
                        uint CS_PIN, uint SPI_RX_PIN, uint SPI_SCK_PIN){
    
    uint ret_baud = spi_init(spi, baudrate);
    // Should be noted the endianness is not corrected with MSB or LSB first on here, annoying :(
    spi_set_format(spi, 8, 0, 0, SPI_MSB_FIRST);

    gpio_init_mask(CS_PIN | SPI_RX_PIN | SPI_SCK_PIN);
    
    gpio_pull_up(CS_PIN);
    gpio_set_dir(CS_PIN, GPIO_OUT);
    gpio_put(CS_PIN, 1);
    
    gpio_set_function(SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_RX_PIN, GPIO_FUNC_SPI);

    max_data->cs_pin = CS_PIN;

    return ret_baud;
}

// Wait for MAX31855 to fully initialize and perform garbage conversions
void MAX31855_INIT_DELAY(){
    busy_wait_ms(200);
}

// Convert Raw Read Data into Useful Values
void __MAX31855_Convert_Raw_Read(struct MAX31855_DATA *max_data){
    // Fixed Point
    max_data->thermocouple_temp = (int16_t)((max_data->ingest.raw >> 16) & 0xFFFC);

    // Fixed Point
    max_data->case_temp = (int16_t)(max_data->ingest.raw & 0xFFF0) >> 4;

    // Error bitfield
    max_data->fault_bitfield = (uint8_t)(((max_data->ingest.raw & (1u << 16)) >> 12) & (max_data->ingest.raw & 0x07));
};

// Blocking Read of Full 32 Bit Insternal Memory
void MAX31855_Read_Blocking(spi_inst_t *spi, struct MAX31855_DATA *max_data){
    uint32_t reg_read;
    gpio_put(max_data->cs_pin, 0);
    spi_read_blocking(spi, 0x00, (uint8_t *)&reg_read, sizeof(uint32_t));
    gpio_put(max_data->cs_pin, 1);

    // Flip bytes to correct endianness
#ifdef M0_PLUS
#warning "Compiling for M0+ Architecture!"
    __asm
    (
        "REV %0, %1" : "=&r" (max_data->ingest.raw) : "r" (reg_read)
    );
#else
#warning "Compiling for Generic Architecture!"
    max_data->ingest.raw = reg_read;

    uint8_t tmp = max_data->ingest.raw_bytes[0];
    max_data->ingest.raw_bytes[0] = max_data->ingest.raw_bytes[3];
    max_data->ingest.raw_bytes[3] = tmp;
    
    tmp = max_data->ingest.raw_bytes[1];
    max_data->ingest.raw_bytes[1] = max_data->ingest.raw_bytes[2];
    max_data->ingest.raw_bytes[2] = tmp;
#endif
    

    __MAX31855_Convert_Raw_Read(max_data);
}


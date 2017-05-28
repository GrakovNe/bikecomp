void ds_1307_init(void);
void i2c_bus_start(void);
void i2c_bus_stop(void);
uint8_t i2c_bus_send_byte(uint8_t data);
uint8_t i2c_bus_read_byte(uint8_t *data,uint8_t ack);
uint8_t ds_1307_read(uint8_t address,uint8_t *data);
uint8_t ds_1307_write(uint8_t address,uint8_t data);

void ds_1307_read_time(int* hours, int* minutes, int* seconds);
void ds_1307_write_time(int hours, int minutes, int seconds);
/* 
 * i2c haberlesmesi başlık dosyası
 * 
 * 
 */

void i2c_init();
void i2c_start();
void i2c_write(uint8_t slave_add, uint8_t reg_add, uint8_t val);
uint8_t i2c_read(uint8_t slave_add, uint8_t reg_add);

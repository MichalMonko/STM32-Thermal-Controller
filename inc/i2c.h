#define NO_ERROR_CODE 0
#define BUS_ERROR_CODE 1
#define ACK_FAIL_CODE 2
#define ARBITRATION_LOST_CODE 3

unsigned char i2c_check_errors(void);
unsigned char i2c_send_data(const unsigned char base_address, unsigned char * data, unsigned char num);
unsigned char i2c_send_byte(const unsigned char base_address, unsigned char data);

//uint8_t read_data(const uint8_t register, uint8_t * data, uint num) {

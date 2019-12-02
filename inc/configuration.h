#ifndef  __CONFIGURATION_H
#define  __CONFIGURATION_H

void enable_clocks(void);
void configure_timers(void);
void configure_i2c(void);
void configure_spi(void);
void configure_gpio(void);
void configure_interrupts(void);
void configure_for_transmitter(void);
void configure_for_receiver(void);
void disable_interrupts(void);
void enable_interrupts(void);

#endif

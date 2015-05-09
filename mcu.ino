void mcu_reduce_features() {
  power_twi_disable();
  power_adc_disable();
  power_timer1_disable();
  power_timer2_disable();
  power_timer3_disable();

  Xadow.greenLed(LEDOFF);
}

void mcu_disable_usbserial() {
  power_usart1_disable();
  power_usb_disable();
  Xadow.greenLed(LEDOFF);

  // red led is also RX led, so when we disable
  // usb serial we should shut it down as well
  Xadow.redLed(LEDOFF);
}

void mcu_enable_usbserial() {
  power_usart1_enable();
  power_usb_enable();
  Xadow.greenLed(LEDON);
}

/**
 * Returns voltage in .1 volt resolution.
 * 42 is therefore 4.2 volts
 */
uint8_t mcu_get_battery_voltage() {
  power_adc_enable();
  delay(1);
  uint8_t ret = Xadow.getBatVol()*10;
  power_adc_disable();
  return ret;
}

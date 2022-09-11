# arduino_pro_mini_3p3V_to_max19777_3wire_spi
 Arduino Mini 3.3V 8MHz bitbanging the 3-wire CISO connection to the Max19777 to aquire 12b ADC readings

# Setup #
* Controller = Arduino Pro Mini 3.3V
  * Arduino Clock = 8MHz
* Peripheral = MAX19777 ADC on Eval Board

# Measurement Results #
* Precision (12bit) = 0.8mV/bit
* Accuracy ≈ -0.8%
* System Measurement Period ≈ 33us
  * Note: Arduino commands have significant code overhead (e.g. digitalwrite(...) = 50 cycles)
* System Max Sampling ≈ 30.3kHz

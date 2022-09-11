/*
 * Arduino Mini 3.3V 8MHz bitbanging the 3-wire CISO connection to the Max19777 to aquire 12b ADC readings
 */

#include <Arduino.h>

#define SCK 13
#define SDI 12
// #define SDO 11 // Not used
#define CS_B 10
#define CH_SEL 9

unsigned long loop_cnt = 0;

#define SAMPLE_CYCLES 16
// Creates a string output of the binary input
void print_binary(uint16_t data_in) {
  for (int16_t binary_idx = SAMPLE_CYCLES-1; binary_idx >= 0; binary_idx--){
    Serial.print("  ");
    Serial.write(bitRead(data_in, binary_idx) ? '1' : '0');
  }
}

// Simple for loop that toggles the clock then inspects the CISO pin
double bitBangData(uint16_t* _rx_low, uint16_t* _rx_high)  // This function transmit the data via bitbanging
{
  double execution_time_us = 0;
  digitalWrite(SCK, HIGH);   
  // delayMicroseconds(1); // t2 > 5ns
  digitalWrite(CS_B, LOW);        // SS low
  // delayMicroseconds(1); // t2 > 5ns

  unsigned long time_start = micros();
  for(int16_t idx = SAMPLE_CYCLES-1; idx >= 2; idx--)
  {
    digitalWrite(SCK, LOW);                   // SCK low
    // delayMicroseconds(1); // t2 > 5ns
    digitalWrite(SCK, HIGH);
    bitWrite((*_rx_high), idx, digitalRead(SDI)); // Capture MISO
  } 
  unsigned long time_end = micros();
  digitalWrite(CS_B, HIGH);
  execution_time_us = (time_end-time_start)/((double)SAMPLE_CYCLES);
  return execution_time_us;        // Return the received data
}

void setup() {
	Serial.begin(9600);
  
  pinMode(SDI, INPUT);
  pinMode(SCK, OUTPUT);
  pinMode(CS_B, OUTPUT);
  pinMode(CH_SEL, OUTPUT);
  digitalWrite(CS_B, HIGH);
  digitalWrite(CH_SEL, LOW);
}

#define VDD 3.3F
#define BITS_RESOLUTION 12U
void loop() {

  uint16_t rx_low = 0;
  uint16_t rx_high = 0;
  uint16_t rx_high_12b = 0;
  double execution_time_us = 0;

  char a_in_ch[] = "AIN_CH1";
  
  // Probing each of the Max19777's two channels in turn
  if(loop_cnt%2 == 0){
    strcpy(a_in_ch, "AIN_CH1");
    digitalWrite(CH_SEL, LOW);
  }
  else{
    strcpy(a_in_ch, "AIN_CH2");
    digitalWrite(CH_SEL, HIGH);
  }

  execution_time_us = bitBangData(&rx_low, &rx_high); // data transmission

  // Formatting the data and displaying via serial connection
  
  //sprintf(char_buffer, "CNT = %0d| 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16", (uint16_t) loop_cnt);
  //Serial.println(char_buffer);
  Serial.print("CNT = ");
  if(loop_cnt < 10){
    Serial.print("0");
  }
  Serial.print(loop_cnt);
  Serial.println("|  16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01");
  rx_high_12b = ((int)rx_high) >> 3U;
  Serial.print(a_in_ch);
  Serial.print(" = ");
  print_binary(rx_high_12b);
  Serial.println("");
  Serial.print(a_in_ch);
  Serial.print(" = ");
  Serial.print(rx_high_12b, DEC);
  Serial.print("| ");
  Serial.print(rx_high_12b*VDD/((double)(2UL<<(BITS_RESOLUTION-1))), DEC);
  Serial.println("V");
  //sprintf(int_outs, "RX_LOW = %0d| RX_HIGH = %0d", rx_low, rx_high);
  //Serial.println(int_outs);
  Serial.print("Execution Time (us) = ");
  Serial.println(execution_time_us);
  Serial.println("");
  loop_cnt++;

  delay(5000);
}
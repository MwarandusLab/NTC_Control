#include <math.h>

int Buzzer = 12;
int Red_1 = 7;
int Green_1 = 8;
int Red_2 = 3;
int Green_2 = 4;
int Red_3 = 5;
int Green_3 = 6;
int Relay = 11;
const int thermistor_output = A1;

bool aboveThreshold = false;
bool belowThreshold = false;
bool risingTemp = false;
bool fallingTemp = false;

double previousTemp = 0;

void setup() {
  Serial.begin(9600);
  pinMode(Buzzer, OUTPUT);
  pinMode(Red_1, OUTPUT);
  pinMode(Green_1, OUTPUT);
  pinMode(Red_2, OUTPUT);
  pinMode(Green_2, OUTPUT);
  pinMode(Red_3, OUTPUT);
  pinMode(Green_3, OUTPUT);
  pinMode(Relay, OUTPUT);

  digitalWrite(Relay, HIGH);
  digitalWrite(Red_1, LOW);
  digitalWrite(Green_1, LOW);
  digitalWrite(Red_2, LOW);
  digitalWrite(Green_2, LOW);
  digitalWrite(Red_3, LOW);
  digitalWrite(Green_3, LOW);
}
void loop() {
  int thermistor_adc_val;
  double output_voltage, thermistor_resistance, therm_res_ln, temperature; 
  thermistor_adc_val = analogRead(thermistor_output);
  output_voltage = ( (thermistor_adc_val * 5.0) / 1023.0 );
  thermistor_resistance = ( ( 5 * ( 10.0 / output_voltage ) ) - 10 ); /* Resistance in kilo ohms */
  thermistor_resistance = thermistor_resistance * 1000 ; /* Resistance in ohms   */
  therm_res_ln = log(thermistor_resistance);
  temperature = ( 1 / ( 0.001129148 + ( 0.000234125 * therm_res_ln ) + ( 0.0000000876741 * therm_res_ln * therm_res_ln * therm_res_ln ) ) ); /* Temperature in Kelvin */
  temperature = temperature - 273.15; /* Temperature in degree Celsius */
  Serial.print("Temperature in degree Celsius = ");
  Serial.print(temperature);
  Serial.print("\t\t");
  Serial.print("Resistance in ohms = ");
  Serial.print(thermistor_resistance);
  Serial.print("\n\n");

  if (temperature > previousTemp) {
    risingTemp = true;
    fallingTemp = false;
  } else if (temperature < previousTemp) {
    risingTemp = false;
    fallingTemp = true;
  } else {
    risingTemp = false;
    fallingTemp = false;
  }

  previousTemp = temperature;

  if (temperature > 40 && risingTemp) {
    digitalWrite(Red_1, LOW);
    digitalWrite(Red_2, HIGH);
    digitalWrite(Red_3, LOW);
    digitalWrite(Green_1, HIGH);
    digitalWrite(Green_2, LOW);
    digitalWrite(Green_3, LOW);
    digitalWrite(Buzzer, HIGH);
    digitalWrite(Relay, LOW);
  } else if (temperature > 40 && fallingTemp) {
    digitalWrite(Red_1, LOW);
    digitalWrite(Red_2, HIGH);
    digitalWrite(Red_3, HIGH);
    digitalWrite(Green_1, LOW);
    digitalWrite(Green_2, LOW);
    digitalWrite(Green_3, LOW);
    digitalWrite(Buzzer, HIGH);
    digitalWrite(Relay, LOW);
  } else if (temperature < 40 && risingTemp) {
    digitalWrite(Red_1, LOW);
    digitalWrite(Red_2, LOW);
    digitalWrite(Red_3, LOW);
    digitalWrite(Green_1, HIGH);
    digitalWrite(Green_2, HIGH);
    digitalWrite(Green_3, LOW);
    digitalWrite(Buzzer, LOW);
    digitalWrite(Relay, HIGH);
  } else if (temperature < 40 && fallingTemp) {
    digitalWrite(Red_1, LOW);
    digitalWrite(Red_2, LOW);
    digitalWrite(Red_3, HIGH);
    digitalWrite(Green_1, LOW);
    digitalWrite(Green_2, HIGH);
    digitalWrite(Green_3, LOW);
    digitalWrite(Buzzer, LOW);
    digitalWrite(Relay, HIGH);
  }

  delay(1000);
}

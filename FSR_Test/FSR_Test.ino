/* FSR testing sketch.
Connect one end of FSR to power, the other end to Analog 0.
Then connect one end of a 10K resistor from Analog 0 to ground
Connect LED from pin 11 through a 220 ohm resister to ground

For more information see www.ladyada.net/learn/sensors/fsr.html */

int fsrPin = 0; // the FSR and 10K pulldown are connected to a0
int fsrReading; // the analog reading from the FSR resistor divider
int fsrVoltage; // the analog reading converted to voltage

int LEDpinRed = 11; // Pressure
int LEDbrightness; // brightness of LED

int LEDpinGreen = 12; // No pressure

unsigned long fsrResistance; // The voltage converted to resistance, can be very big so make "long"
unsigned long fsrConductance;
long fsrForce; // Finally, the resistance converted to force

void setup(void) {
    Serial.begin(9600); // We'll send debugging information via the Serial monitor
    pinMode(LEDpinGreen, OUTPUT);
}

void loop(void) {
    fsrReading = analogRead(fsrPin);
    Serial.print("Analog reading = ");
    Serial.println(fsrReading);
    // analog voltage reading ranges from about 0 to 1023 which maps to 0V to 5V (= 5000mV)
    fsrVoltage = map(fsrReading, 0, 1023, 0, 5000);
    Serial.print("Voltage reading in mV = ");
    Serial.println(fsrVoltage);
    if (fsrVoltage == 0) {
        Serial.println("No pressure");
        digitalWrite(LEDpinGreen, HIGH);
    } else {
        digitalWrite(LEDpinGreen, LOW);
        // The voltage = Vcc * R / (R + FSR) where R = 10K and Vcc = 5V
        // so FSR = ((Vcc - V) * R) / V yay math!
        fsrResistance = 5000 - fsrVoltage; // fsrVoltage is in millivolts so 5V = 5000mV
        fsrResistance *= 10000; // 10K resistor
        fsrResistance /= fsrVoltage;
        Serial.print("FSR resistance in ohms = ");
        Serial.println(fsrResistance);
        fsrConductance = 1000000; // we measure in micromhos so
        fsrConductance /= fsrResistance;
        Serial.print("Conductance in microMhos: ");
        Serial.println(fsrConductance);
        // Use the two FSR guide graphs to approximate the force
        if (fsrConductance <= 1000) {
            fsrForce = fsrConductance / 80;
            Serial.print("Force in Newtons: ");
            Serial.println(fsrForce);
        } else {
            fsrForce = fsrConductance - 1000;
            fsrForce /= 30;
            Serial.print("Force in Newtons: ");
            Serial.println(fsrForce);
            // 1 Newton = 0.224809 Pound-force
        }
    }
    Serial.println("--------------------");
    
    // we'll need to change the range from the analog reading (0-1023) down to the range
    // used by analogWrite (0-255) with map!
    LEDbrightness = map(fsrReading, 0, 1023, 0, 255);
    // LED gets brighter the harder you press
    analogWrite(LEDpinRed, LEDbrightness);
    
    delay(1000);
}

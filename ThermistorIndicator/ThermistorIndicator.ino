// include the TinkerKit library
#include <TinkerKit.h>

// declare TK modules
TKButton button(I0);
TKThermistor therm(I2);
TKPotentiometer pot(I1);
TKLed ledGreen(O1);
TKLed ledRed(O0);

// define temperature related variables 
float lowPass = 22;       // lower temp boundary
float highPass = 30;      // upper temp boundary
float limit = 0;          // limit for determining the LED to be used
float blinkPause;         // pause rate between two blinks

bool pause = false;       // boolean for storing if we want to pause the measure

float C;                  // variable for storing the measured temperature via the thermistor
float P;                  // variable for storing the measured pot value

void setup()
{
  // initialize serial communications at 9600 bps
  Serial.begin(9600);
}

void loop()
{

  if (!pause) {           // step into if the measure pause is false

    // read the analog inputs
    C = therm.readCelsius();
    P = pot.read();

    // set the blink frequency - value of the pause between two blinks (50 to 950)
    if (C >= highPass) blinkPause = 50;
    if (C <= lowPass) blinkPause = 950;
    if (C > lowPass && C < highPass) {
      float x = (highPass - lowPass) - (C - lowPass);
      blinkPause = (x / (highPass - lowPass)) * 900 + 50;
    }

    // calculate the limit through the pot per mille
    limit = (P / 1000) * (highPass - lowPass) + lowPass;
    
    /*Serial.print("Temp: ");
    Serial.print(C);
    Serial.print("\t");
    Serial.print("Limit: ");
    Serial.print(limit);
    Serial.print("\n");*/

    // log the measured temperature and the limit - used for plotting the output
    Serial.print(C); Serial.print("\t"); 
    Serial.print(limit); Serial.println("");

    // execute the led blinks
    if (C >= limit) {
      ledGreen.off();
      ledRed.on();
      delay(50);
      ledRed.off();
      delay(blinkPause);
    } else {
      ledRed.off();
      ledGreen.on();
      delay(50);
      ledGreen.off();
      delay(blinkPause);
    }
    
  } 
  else {        // measure pause is on - execute the pause led blink routine 
    Serial.println("Button held!"); 
    ledRed.off();
    ledGreen.on();
    delay(50);
    ledGreen.off();
    ledRed.on();
    delay(50);
  }

  // determine if the measure pause is on or off
  if (button.pressed() && !pause) pause = true;
  else if (button.released() && pause) pause = false;
  
}

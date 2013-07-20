/**
* @author Mike Almond - @mikedotalmond - https://github.com/mikedotalmond
* 
* PulseInZero and PulseInOne provide a non-blocking implementation 
* similar to the Arduino pulseIn() function ...
* 
* Code execution will pause when reading the duration of a pulse on a pin using pulseIn()
* These libraries each use an interrupt to monitor a pin and measure pulse lengths without 
* pausing code execution during the pulse.
* 
* PulseInZero uses interrupt 0 (pin 2 on arduino uno)
* PulseInOne uses interrupt 1 (pin 3 on an arduino uno)
* See http://arduino.cc/en/Reference/AttachInterrupt for more on the Arduino hardware interrupts
* 
* Connect a button, or anything else that produces didital pulses, 
* to the pin for interrupt 0 (pin 2 on an arduino uno). 
*
*
* Run the program and check the Serial Monitor while making some pulses.
* The duration of a pulse (in microseconds) is logged when it ends.
* 
* The library currently only looks for LOW-->HIGH-->LOW pulses:  __|RISING->->->FALLING|__
*/

#include <PulseInZero.h>


void setup() {

  Serial.begin(9600);

  // set up PulseInZero, pass in the callback function to be triggered when a pulse completes.
  PulseInZero::setup(pulseInComplete);
  
  // use PulseInZero::begin() in place of pulseIn(), and pulseInComplete will fire when a pulse completes.
  PulseInZero::begin();
  /* Calling PulseInZero::begin() will make it watch for a rising edge on a pin indefinitely - there is no timeout.
   * You can stop listening and detach the interrupt by calling PulseInZero::abandon();
   * 
   * After a pulse is detected and the pulseInComplete callback fires you will need to  
   * call PulseInZero::begin() again if you want to watch for another pulse. 
   * (I'm considering a contunious-capture mode that will keep reporting pulse durations without
   * having to call begin again each time...)
   */
  
}


void loop() {
  
  // do stuff...  
}


/**
* Pulse complete callback hanlder for PulseInZero
* @param duration - pulse length in microseconds
*/
void pulseInComplete(unsigned long duration){
  // note: if you're detecting a lot of pulses a second it's probably best to remove the serial prints... 
  // (this function is triggered by an interrupt so the serial buffer can fill up and cause the program to hang)
  Serial.print("pulse complete - duration: ");
  Serial.print(duration);
  Serial.println(" us");
  
  PulseInZero::begin();  // Start listening again...  
  
}

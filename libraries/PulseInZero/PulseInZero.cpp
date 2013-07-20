/*Copyright (c) 2013 Mike Almond - @mikedotalmond - https://github.com/mikedotalmond

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.*/

/**
* @author Mike Almond - https://github.com/mikedotalmond
* 
* Use a hardware interrupt to emulate the Arduino::pulseIn functionality without pausing code execution while waiting for the pulse
* see http://arduino.cc/en/Reference/Interrupts
* */

#include "Arduino.h"
#include "PulseInZero.h"

// initialise static vars
bool PulseInZero::state = false;
bool PulseInZero::active = false;

void (*PulseInZero::onComplete)(unsigned long) = NULL;


/**
* 
*
**/	
void PulseInZero::setup(void (*pulseComplete)(unsigned long)){	
	onComplete 	= pulseComplete;
	active 		= false;
}


/**
* @public
* Start listening on the desired interrupt for a pulse
*/
void PulseInZero::begin(){
	state		= false;
	active 		= true;
	attachInterrupt(0, pinChange, RISING);
}


/**
* @public
* There's no timeout with this class: it will listen indefinitely for a change on the interrupt pin.
* Use abandon to stop waiting for a pulse.
*/
void PulseInZero::abandon(){
	if(active){
		state =  active = false;
		detachInterrupt(0);
	}
}


/**
* @static 
* interrupt handler - called whenever the interrupt pin state changes
*/
void PulseInZero::pinChange(){
	
	static unsigned long pulseStart = 0;
	
	state = !state;
	
	if(state){ 
		// interrupt pin has changed, a pulse has started
		pulseStart = micros(); // store the current microseconds
		attachInterrupt(0, pinChange, FALLING); // now wait for the falling edge
	} else {  	
		
		// pulse complete, detach the interrupt...
		detachInterrupt(0);
		
		// pin state changed again - pulse ended
		unsigned long duration = micros() - pulseStart; // get the pulse length
		
		active = false;
		onComplete(duration);
	}
}
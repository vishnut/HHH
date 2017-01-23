#include "RASTemp.h"
#include <RASLib/inc/common.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/adc.h>
#include <RASLib/inc/linesensor.h>

tBoolean led_on;
static tMotor *Motors[4];
static tBoolean initialized = false;
static tADC *adc[4];
static tLineSensor *gls;

//Flashes blue and red LEDs
void blink(void) {
    SetPin(PIN_F1, led_on);
    SetPin(PIN_F3, !led_on);

    led_on = !led_on;
}


//Turns on Red LED
void ledRed(void){
	setPin(PIN_F1,true);
	setPin(PIN_F3, false);
}

//Turns on Blue LED
void ledBlue(void){
	setPin(PIN_F1,false);
	setPin(PIN_F3, true);
}

//Initializes motors and IR Sebsors
void initMotorsSensors(void) {
    if (!initialized) {
      initialized = true;
      
      Motors[0] = InitializeServoMotor(PIN_B6, false);
      Motors[1] = InitializeServoMotor(PIN_B7, false);
      Motors[2] = InitializeServoMotor(PIN_C4, false);
      Motors[3] = InitializeServoMotor(PIN_C5, false);
   
      adc[0] = InitializeADC(PIN_D0);
      adc[1] = InitializeADC(PIN_D1);
      adc[2] = InitializeADC(PIN_D2);
      adc[3] = InitializeADC(PIN_D3);

      gls = InitializeGPIOLineSensor(
        PIN_B5, 
        PIN_D0, 
        PIN_D1, 
        PIN_D2, 
        PIN_D3, 
        PIN_E0, 
        PIN_C6, 
        PIN_C7
        );
    }
}



int main(void) { 
     Printf("\nRunningo\n");
     blink();   //turn on one led
     initMotorsSensors(); //initialize all
    // runMotor();
     irr(); //motor-sensor pairing
     while(1){
	wallfollow(.15f);
     }
   // runMotor();	
   // irr();	
   // circle();
   // runMotor();
}

//follows wall with set distance d
void wallfollow(int d){
	int t=ADCRead(adc[0]);
	if(t<d){
		setm(.25f,.75f);
	}
	else if(t==d){
		setm(.5f,.5f);
	}
	else{
		setm(.75f,.25f);
	}
}

//Flashes LED for certain IR values
void irrun(void){
	int t=0;
	while(1){
	Printf(
	    "IR values:  %1.3f  %1.3f  %1.3f  %1.3f\r",
            ADCRead(adc[0]),
            ADCRead(adc[1]),
            ADCRead(adc[2]),
            ADCRead(adc[3])
	);

	if(ADCRead(adc[0]) < 0)
		blink();
	}	
}


//Runs motors if IR returns values less than constant dist
void irr(void){
	float dist= 800.0f;
	while(1){
		if(72 / ADCRead(adc[0]) < dist)
			runMotor();
		else
			stopMotors();
	}
}

//Turns on all motors equallt
void runMotor(void){
	float left = 0, right = 0, speed = .75f, accel = .01f;
	left = speed;
	right = speed;
	SetMotor(Motors[0], left);
	SetMotor(Motors[1], left);
	SetMotor(Motors[2], right);
	SetMotor(Motors[3], right);
}

//Some messed up shape
void circle(void){	
	float left = 0, right = 0, speed = .75f, accel = .01f;
	left = speed;
	right = speed;
	int t = 0;	
	SetMotor(Motors[0], left);
	SetMotor(Motors[1], left);
	SetMotor(Motors[2], right);
	SetMotor(Motors[3], right);
	while(t<1500){
		t++;
		Printf("forward");
	}
	left-=speed;
	t=0;
	setm(left,right);
	while(t<1500){
		t++;
		Printf("right");
	}
	left+=speed;
	right-=speed;
	setm(left,right);
	t=0;
	while(t<1500){
		t++;
		Printf("left");
	}
	left=-speed;
	right=-speed;
	setm(left,right);
	t=0;
	while(t<3000){
		t++;
		Printf("back");
	}
}


//Moves in previously set direction for certain amount of time
void go(int t){
	while(t>0){
		t--;
		Printf("moving");
	}
}


//Creates a figure 8 pattern
void figeight(int k){	
	float left = 0, right = 0, speed = .75f, accel = .01f;
	left=speed;
	right=speed;
	int t=0;
	setm(left,right);
	while(t<100){
		t++;
		Printf("forward");
	}
	t=0;
	right-=speed;
	setm(left,right);
	while(t<500){
		t++;
		Printf("right");
	}
	t=0;
	right+=speed;
	left-=speed;
	while(t<1000){
		t++;
		Printf("left");
	}
	t=0;
	left+=speed;
	while(t<2000){
		t++;
		Printf("diagonal");
	}
	t=0;
	right-=speed;
	setm(left,right);
	while(t<5000){
		t++;
		Printf("right");	
	}
	if(k==0){
		k++;
		figeight(k);
	}
}

//Set motors with left and right variables
void setm(float left, float right){
	
	SetMotor(Motors[0], left);
	SetMotor(Motors[1], left);
	SetMotor(Motors[2], right);
	SetMotor(Motors[3], right);
}

//Stop all motors
void stopMotors(void){
	float left = 0, right = 0;
        SetMotor(Motors[0], left);
        SetMotor(Motors[1], left);
        SetMotor(Motors[2], right);
        SetMotor(Motors[3], right);
}

void gpioLineSensorDemo(void) {
    Printf("Press any key to quit\n");
  
    // loop as long as the user doesn't press a key 
    while (!KeyWasPressed()) {
        int i;
        float line[8];
   
        // put the values of the line sensor into the 'line' array 
        LineSensorReadArray(gls, line);
        Printf("Line Sensor: [");
    
        for (i = 0; i < 8; i++) {
            Printf("%.2f ", line[i]);
        }
    
        Printf("\b]        \r");
    }
  
    Printf("\n"); 
}


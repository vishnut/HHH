#include "RASTemp.h"
#include <RASLib/inc/common.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/servo.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/adc.h>
#include <RASLib/inc/linesensor.h>
#include <RASLib/inc/time.h>
#include <StellarisWare/driverlib/gpio.h>

tBoolean led_on;
static tMotor *Motors[4];
static tBoolean initialized = false;
static tADC *adc[4];
static tLineSensor *gls;
static int turn;
static int team;
static tServo *marservo;
static tServo *pingservo;

//Flashes blue and red LEDs
void blink(void) {
    SetPin(PIN_F1, led_on);
    SetPin(PIN_F3, !led_on);

    led_on = !led_on;
}

void competition(void){
	runRoller();
//	float wdist = .10f;
//	while(ADCRead(adc[0]) > .20f){
//		linefollow();
//	}	
//	setm(.75f,.25f);
	float i = .20f;
	while(i<.70f){
		while(1){
			wallfollow(.20f,ADCRead(adc[0]));
		}	
		i+=.10f;
	}
}

void runRoller(void){}

int  online(){ 
        float line[8];
	LineSensorReadArray(gls, line);
	int total = 0;
	int i=0;
	for (i = 0; i < 8; i++) {
        	Printf("%.2f ", line[i]);
		if(line[i] > 1000)
		    total+=1;
	}
	if(total > 5){
		ledGreen();
		return 1;
	}
	else
		return 0;
}

//Turns on Red LED
void ledRed(void){
	SetPin(PIN_F1,true);
	SetPin(PIN_F2, false);
	SetPin(PIN_F3, false);
}

//Turns on Blue LED
void ledBlue(void){
	SetPin(PIN_F1,false);
	SetPin(PIN_F2, true);
	SetPin(PIN_F3, false);
}

//Turns on Green LED
void ledGreen(void){
	SetPin(PIN_F1, false);
	SetPin(PIN_F2, false);
	SetPin(PIN_F3, true);
}

//Initializes motors and IR Sebsors
void initMotorsSensors(void) {
    if (!initialized) {
      initialized = true;
      
      Motors[0] = InitializeServoMotor(PIN_B6, false);
      Motors[1] = InitializeServoMotor(PIN_B7, false);
      Motors[2] = InitializeServoMotor(PIN_C4, false);
      Motors[3] = InitializeServoMotor(PIN_C5, false);

      marservo = InitializeServo(PIN_B2);
      pingservo = InitializeServo(PIN_F3);
      SetServo(marservo,0.1f);
      SetServo(pingservo,0.1f);

      GPIOPadConfigSet(PORT_VAL(PIN_B6), PIN_VAL(PIN_B6), GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
      GPIOPadConfigSet(PORT_VAL(PIN_B7), PIN_VAL(PIN_B7), GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
      GPIOPadConfigSet(PORT_VAL(PIN_C4), PIN_VAL(PIN_C4), GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
      GPIOPadConfigSet(PORT_VAL(PIN_C5), PIN_VAL(PIN_C5), GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
   
      adc[0] = InitializeADC(PIN_D0);
      adc[1] = InitializeADC(PIN_D1);
      adc[2] = InitializeADC(PIN_D2);
      adc[3] = InitializeADC(PIN_D3);
      turn=0;
      gls = InitializeGPIOLineSensor(
        PIN_B5, 
        PIN_B0, 
        PIN_B1, 
        PIN_E4, 
        PIN_E5, 
        PIN_B4, 
        PIN_A5, 
        PIN_A6
        );
    }
}

//follows wall with set distance d
void wallfollow(float d,float t){
	if(t<d){
		if(t<(d/2)){
			setm(0.0f,0.9f);
			ledBlue();
		}
		else if(t<(d*2/3)){
			setm(0.2f,0.8f);
			ledBlue();
		}
		else{
			setm(0.4f,0.7f);
			ledBlue();
		}
	}
	else{
		if(t==d){
			setm(.5f,.5f);
			ledGreen();
		}
		else if(t>(d*2)){
			setm(0.9f,0.0f);
			ledRed();
		}
		else if(t>(d*3/2)){
			setm(0.8f,0.2f);
			ledRed();
		}
		else{
			setm(0.7f,0.4f);
			ledRed();
		}
	}
}

//Turns on all motors equallt
void runMotor(void){
	float left = 0, right = 0, speed =  1.0f, accel = .01f;
	left = -speed;
	right = speed;
	SetMotor(Motors[0], left);
	SetMotor(Motors[1], left);
	SetMotor(Motors[2], right);
	SetMotor(Motors[3], right);
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
	left=-left;	
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


void lineFollo(void){
	int i;
	float line[8];
	float total = 0.0f;
	float tot=0.0f;
	LineSensorReadArray(gls,line);	
	for (i = 0; i < 4; i++) {
		
		total+=((i*1.0f))*line[i];
	}

	for (i = 4; i < 8; i++) {
		tot+=((i*1.0f))*line[i];
	}
	if(total > 1000)
	{
		if(tot<1000){
			setm(.5f,.1f);
			ledRed();
		}
		else{
			setm(.2f,.2f);
			ledGreen();
		}
	}	
	else{
		if(tot > 1000){
			setm(.1f,.5f);
			ledBlue();
		}
		else{
			setm(.2f,.2f);
			ledGreen();
		}
	}

}


void lineFollow(void) {
        int i;
        float line[8];
   	float total = 0.0f; 
        LineSensorReadArray(gls, line);
        Printf("Line Sensor: [");
	        for (i = 0; i < 8; i++) {
        	    Printf("%.2f ", line[i]);
		    	if(line[i] < 1000){
				total+=((i*1.0f)-3.5f)*line[i];
			}
			else{
				total+=((i*1.0)-3.5f)*5;
			}
       		 }
        Printf("\b]        \r");
	if(total == 0){
		turn=0;
		ledGreen();
		setm(.25f,.3f);
	}
	else if(total < 0){
		if(total > (-20.0f)){
			if(turn==0){
				ledGreen();
				setm(.25f,.30f);
			}
			else{
				turn=0;
			}
		}
		else if(turn ==1){
			ledBlue();
		if (total < (-30))
		{	
			ledBlue();
			setm(.45f, .00f);
		}
		else if(total > (-25))
		{	
			setm(.35f,.00f);
			ledBlue();
		}
		else
		{	
			setm(.25f, .00f);
			ledBlue();
		}
		}
		else{
			turn=1;
		}
	}
	else{
		if(total < (20.0f)){
			if(turn==0){
				ledGreen();
				setm(.25f,.30f);
			}
			else{
				turn=0;
			}
		}
		else{
			ledRed();
			if(turn==2){
			if(total > 30)
			{	
				ledRed();
				setm(.00f,.45f);
			}
			else if(total < 25)
			{	
				setm(.00f,.35f);
				ledRed();
			}
			else
			{	
				setm(.00f,.25f);
				ledRed();
			}
		}
		else{
			turn=2;
		}
		}
	}
    Printf("\n"); 
}

void edgefollow(){
        float line[8];
        LineSensorReadArray(gls, line);
	if(line[0]>100)	{
		setm(.2f,.2f);ledGreen();}
	else if(line[7]>100){
		setm(.6f,-.1f);ledBlue();}
	else{
		setm(-.1f,.6f);ledRed();}
	

}


int turnline(void){
        float line[8];
	LineSensorReadArray(gls, line);
	int i=0;	
	int total = 0;
	for (i = 0; i < 8; i++) {
		if(line[i] > 1000)
		    total+=1;
	}
	return total;
}

void turnback(float w){
	setm(-.5f,.5f);
	Wait(4.0f);
	setm(-0.3f,-.3f);
	Wait(w);
	setm(0.0f,0.0f);
	if(team==1){
		team=0;
		ledBlue();
	}
	else{
		team=1;
		ledRed();
	}
	Wait(2.0f);

}

void squaredance(void){
	int p=0;
	while(p<4){
	setm(.5f,.5f);
	if(ADCRead(adc[2])>.4f){
		Wait(3.0f);
		ledRed();
		setm(0.0f,0.5f);
		Wait(3.45f);
		setm(.5f,.5f);
		Wait(2.0f);
		p+=1;
	}
	else{
		ledBlue();
	}
	
	}
	setm(0.0f,0.0f);
}

void feight(void){
	while(1){
	setm(.5f,.5f);
	if(ADCRead(adc[2])>.4f){
		ledRed();
		Wait(3.0f);
		setm(0.0f,0.7f);
		Wait(3.0f);
		setm(0.5f,0.5f);
		Wait(2.0f);
		setm(0.0f,0.7f);
		Wait(3.0f);
		setm(0.5f,0.5f);
		Wait(2.0f);
	}
	else{
		ledBlue();
	}
	}

}

void score(int k){
	//if ir yes and ir no, drop flaps
	setm(0.2f,0.2f);
	Wait(1.0f);
	setm(0.0f,0.0f);
	if(k==0)
	SetServo(marservo,0.5f);
	else
	SetServo(pingservo,0.5f);
	Wait(2.0f);
	SetServo(marservo,0.0f);	
	SetServo(pingservo,0.0f);
}

void tryservo(void){
	Wait(2.0f);
	SetServo(marservo,0.5f);
	Wait(2.0f);
	SetServo(marservo,0.1f);
}


int main(void) { 
        Printf("\nRunningo\n");
        ledBlue();
	initMotorsSensors(); //initialize all
	tryservo();	
	runMotor();
	squaredance();
	feight();


        float d=.30f;
	int next=0;	
	while(next<2){
		edgefollow();
		if(ADCRead(adc[2])>.50f)
			next+=1;
		else
			next=0;
	}
	setm(-.5f,.5f);
	Wait(3.0f);
	setm(-0.3f,-0.3f);
	Wait(4.0f);
	setm(0.0f,0.0f);
	Wait(2.0f);
	setm(0.0f,0.8f);
	ledBlue();
	Wait(1.0f);
	int k=0;
	while(d>0){
		if(d>.15f){
			while(online()<1){	
				wallfollow(d,ADCRead(adc[2]));
			}
		}
		else{
			while(k<6){
				blink();
				while(online()<1){
					wallfollow(d,ADCRead(adc[2]));
				}
				k+=1;
			}
			k=0;
		}
		if(online()==1){
			setm(.5f,.5f);
			Wait(0.6f);
			turnback(5.0f);
			setm(0.0f,0.8f);
			Wait(1.0f);
		}		
		d-=.05f;
    	}
	setm(0.0f,0.0f);

}


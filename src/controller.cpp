#include "controller.h"

MCONTROL MotorA;

CONTROLLER::CONTROLLER(uint8_t ardpinUP, uint8_t ardpinLEFT,
        uint8_t ardpinDOWN,uint8_t ardpinRIGHT,
        uint8_t ardpinJX, uint8_t ardpinJY){
            
    // Setup buttons
    btnUP = OneButton(13, true, true);
    btnLEFT = OneButton(12, true, true);
    btnDOWN = OneButton(11, true, true);
    btnRIGHT = OneButton(7, true, true);
    joyX = ardpinJX;
    joyY = ardpinJY;
}


void CONTROLLER::
attachMotor(uint8_t ardpinENA, uint8_t ardpinIN1, uint8_t ardpinIN2)
{   
    MotorA = MCONTROL(ardpinENA, ardpinIN1, ardpinIN2);
    MotorA.setup();
}


void CONTROLLER::
attachCommands()
{
   btnUP.attachClick(clickUP);
   btnLEFT.attachClick(clickLEFT);
   btnDOWN.attachClick(clickDOWN);
   btnRIGHT.attachClick(clickRIGHT);
   _joyCommand = joystickMOVE;
}


void CONTROLLER::
controlLoop()
{
    // Update Button States
    btnUP.tick();
    btnLEFT.tick();
    btnDOWN.tick();
    btnRIGHT.tick();

    // Update Joystick states
    _joyCommand(analogRead(joyX), analogRead(joyY));
}


void clickUP()
{
    MotorA.forward();
}


void clickLEFT()
{
    MotorA.halt();
}


void clickDOWN()
{
    MotorA.reverse();
}


void clickRIGHT()
{
    MotorA.halt();
}


int prevJoyState = 1;
void joystickMOVE(int analogX, int analogY){

    // Divide x joystick to 3 different states
    // 0 - Forward 
    // 1 - Halt 
    // 2 - Reverse
    int xstate = map(analogX, 0, 1023, 0, 2);
    //int ystate = map(analogY, 0, 1023, 0, 2);

    // Dont run commands if we are still on the same joystick state
    // Prevent commands running more than once
    if (xstate == prevJoyState)
    {
        return;
    }

    switch (xstate)
    {
    case 0:
        MotorA.forward();
        break;

    case 1:
        MotorA.halt();
        break;

    case 2:
        MotorA.reverse();
        break;
    }

    // Update prev joy state
    prevJoyState = xstate;
}
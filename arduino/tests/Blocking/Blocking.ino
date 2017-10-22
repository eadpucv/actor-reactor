// Blocking.pde
// -*- mode: C++ -*-
//
// Shows how to use the blocking call runToNewPosition
// Which sets a new target position and then waits until the stepper has 
// achieved it.
//
// Copyright (C) 2009 Mike McCauley
// $Id: Blocking.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>

//AccelStepper stepper(AccelStepper::DRIVER, 9, 8); // CLK+(paso) -> pin 8 --- CW+(direccion) -> pin 9
AccelStepper stepper(2, 9, 8); 


void setup()
{  
  stepper.setMaxSpeed(1000.0);
  stepper.setSpeed(50.0);
  stepper.setAcceleration(8000.0);
}

void loop()
{    
    stepper.runToNewPosition(-2400);
    stepper.runToNewPosition(2400);
}

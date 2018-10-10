#include <stdio.h>
#include "MyroC.h"
#include "eSpeakPackage.h"

   int count = 0;
   int leftturns = 0;
   int rightturns = 0;
   int sensorReadingLeft = 0;
   int sensorReadingRight = 0;

// "The Wiggle" - performed after every 10 moves
void dance()
{
  eSpeakTalk("Check out my dance moves, kids. I call this the wiggle.");//announcing dance
  for (int dancecount = 0; dancecount < 3; dancecount++)                //loops dance 3 times
    {
      rTurnRight (0.8, 0.6); /* turn right at 80% speed                         
                               for 6/10 seconds */
      rTurnLeft  (1.2, 0.4); /* turn left  at 120% speed                        
                                for 4/10 seconds */
      rForward (0.75, 0.8);  /* move forward at 3/4 speed                       
                                for 0.8 seconds */
      rBackward (0.5, 1.2); /* move forward at 1/2 speed                       
                                for 1.2 seconds */
    }
}

void startup() //initialize and announce connections
{
  eSpeakTalk("Connecting to robot");//anounce robot connection
  printf ("Program to respond to a detected obstacle\n");//declare program intention
  rConnect ("/dev/rfcomm0");//connect to robot
}
//makes and records a right turn for the robot 
void objleft(){
   eSpeakTalk("object left");
   rTurnRight (1.5, 0.6);
   sensorReadingLeft = rGetIRTxt ("left", 3); //refreshes left sensor reading
   leftturns = 0;
   rightturns++;
   count++;
}
//makes and records a left turn for the robot
void objright(){
   eSpeakTalk("object right");
   rTurnLeft (1.5, 0.6);
   sensorReadingRight = rGetIRTxt ("right", 3);
   rightturns = 0;
   leftturns++;
   count++;
}
// produces beep and right turn
void rightbeep(){
         eSpeakTalk("turning right");
         rMotors(-1,1);
         rBeep (2,500);
         rightturns = 0;
         count++;
}
//produces beep and left turn
void leftbeep(){
         eSpeakTalk("turning left");
         rMotors(1,-1);
         rBeep (2,800);
         leftturns = 0;
         count++;
}
// main program; does magic
int main ()
{
   eSpeakConnect(); // connecting to espeak
   startup();
   while (1) // always true; loops forever
     {
         sensorReadingLeft = rGetIRTxt ("left", 3); //refreshes sensor readings 
         sensorReadingRight = rGetIRTxt ("right", 3);
     for(int runs = 0; runs < 3; runs++)  // runs loop three times
       {
         if (sensorReadingLeft) //turns right when object is to the left
       {
       objleft();
       }
     else
       {
         rightturns = 0; //makes sure rightturn count is consecutive
       }
         if(rightturns == 2) // checks for two consecutive right turns
       {
         rightbeep();
       }
     if (sensorReadingRight) //turns left when object is to the right
       {
         objright();
       }
     else
       {
         leftturns = 0; //makes sure leftturn count is consecutive
       }
     if(leftturns == 2) // checks for two consecutive left turns
       {
         leftbeep();
       }
     if(count > 9) //dances after every 10 turns
       {
         dance();
         count = 0; // resets turn count
       }
     }
     eSpeakTalk("forward");
     rMotors(1,1); // makes robot move forward when no object is detected by sensors
   }

   eSpeakDisconnect(); // disconnects from espeak
   rDisconnect(); // disconnects robot
   printf ("Processing completed\n");
   return 0; // checks that program ran without error
}

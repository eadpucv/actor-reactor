/*
 *  soften value
 */


float soften(float newVal){
   return (newVal * A + last_motor_pos * B);
}


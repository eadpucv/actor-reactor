/*
 *  soften value
 */


float soften(float newVal){
   return (newVal * 0.02 + last_motor_pos * 0.98);
}


/*
 *  soften value
 */


int soften(float newVal){
   return (newVal * A + last_motor_pos * B);
}


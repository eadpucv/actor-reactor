/*
 *  soften value
 */


float soften(float new_motor_pos){
   return (new_motor_pos * A + last_motor_pos * B);
}


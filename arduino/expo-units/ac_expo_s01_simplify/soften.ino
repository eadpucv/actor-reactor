/*
 *  soften value
 */


float soften(float newVal){
   
   // return (newVal * 0.02 + last_motor_pos * 0.98);
   return (newVal * 0.02 + acum_sonar_read * 0.98);
   
}


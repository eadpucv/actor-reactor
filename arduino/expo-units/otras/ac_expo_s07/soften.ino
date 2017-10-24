/*
 *  soften value
 */


float soften(float newVal, float oldVal){

   return (newVal * 0.02 + oldVal * 0.98);
   
}


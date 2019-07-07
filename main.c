#include "pso.h"
#include <math.h>

// Remove?
#include <float.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef M_PI
// PI (Taken from boost library)
#define M_PI (double)(3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651e+00)
#endif

#ifndef M_E
// Euler's constant (taken from boost library)
#define M_E (double)(2.71828182845904523536028747135266249775724709369995957496696762772407663035354759457138217852516642742746639193e+00)
#endif

bool less_than(double a, double b){
    return a < b;
}

double ackleys_function(double x, double y){
    return -20 * pow(M_E, -0.2*sqrt(0.5*(x*x + y*y))) - pow(M_E, 0.5*(cos(2*M_PI*x)+cos(2*M_PI*y))) + M_E + 20;
}

double ackleys_function_n(double *pos){
    double x = pos[0];
    double y = pos[1];
    return -20 * pow(M_E, -0.2*sqrt(0.5*(x*x + y*y))) - pow(M_E, 0.5*(cos(2*M_PI*x)+cos(2*M_PI*y))) + M_E + 20;
}



int main(int argc, char *argv[]){
   pso_init();
   double bounds[][2] = {{-50.0, 50.0}, {-50., 50.}};
   
   //double *res = pso3dim(ackleys_function, bounds, less_than, 20, 1000000);
   //printf("[%.*e, %.*e]\n", DECIMAL_DIG, res[0], DECIMAL_DIG, res[1]);
   //free(res);
   
   //TPSOxy res = pso3dim_static(ackleys_function, bounds, less_than, 1000000);
   //printf("[%.*e, %.*e]\n", DECIMAL_DIG, res.x, DECIMAL_DIG, res.y);
   
   double *res = psondim(ackleys_function_n, bounds, 3, less_than, 20, 1000000);
   printf("[%.*e, %.*e]\n", DECIMAL_DIG, res[0], DECIMAL_DIG, res[1]);
   free(res);
   
   return 0;
}

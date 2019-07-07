#include "pso.h"
#include <math.h>

// Remove?
#include <float.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: Make them more precise
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E 2.71828182845904523536
#endif

bool less_than(double a, double b){
    return a < b;
}

double ackleys_function(double x, double y){
    return -20 * pow(M_E, -0.2*sqrt(0.5*(x*x + y*y))) - pow(M_E, 0.5*(cos(2*M_PI*x)+cos(2*M_PI*y))) + M_E + 20;
}

int main(int argc, char *argv[]){
   pso_init();
   double bounds[2][2] = {{-50.0, 50.0}, {-50., 50.}};
   double *res = pso3dim(ackleys_function, bounds, less_than, 20, 100);
   printf("[%.*e, %.*e]\n", DECIMAL_DIG, res[0], DECIMAL_DIG, res[1]);
   free(res);
   return 0;
}

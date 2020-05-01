/**
 * @file main.c
 * @author Marek Sedláček
 * @date July 2019
 * 
 * @brief File for testing performance of pso
 * module implementation.
 * 
 * This code was made for my bachelor's thesis at
 * Brno University of Technology
 */

#include "pso.h"
#include <math.h>

#ifndef M_PI
// PI (Taken from boost library)
// https://www.boost.org/doc/libs/1_56_0/boost/math/constants/constants.hpp
#define M_PI (double)(3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651e+00)
#endif

#ifndef M_E
// Euler's constant (taken from boost library)
// https://www.boost.org/doc/libs/1_56_0/boost/math/constants/constants.hpp
#define M_E (double)(2.71828182845904523536028747135266249775724709369995957496696762772407663035354759457138217852516642742746639193e+00)
#endif

/**
 * Fitness function for finding functions minimum
 */
bool less_than(double a, double b){
    return a < b;
}

/**
 * Ackley's function
 * @param x x coordinate
 * @param y y coordinate
 */
double ackleys_function(double x, double y){
    return -20 * pow(M_E, -0.2*sqrt(0.5*(x*x + y*y))) - pow(M_E, 0.5*(cos(2*M_PI*x)+cos(2*M_PI*y))) + M_E + 20;
}

/**
 * Ackley's function implementation for psondim function
 * @param pos array of coordinates 
 */
double ackleys_function_n(double *pos){
    double x = pos[0];
    double y = pos[1];
    return -20 * pow(M_E, -0.2*sqrt(0.5*(x*x + y*y))) - pow(M_E, 0.5*(cos(2*M_PI*x)+cos(2*M_PI*y))) + M_E + 20;
}

int main(int argc, char *argv[]){
#define ITERS 5000  // Amount of iterations
    pso_init();
    double bounds[][2] = {{-50.0, 50.0}, {-50., 50.}};
    
    TPSOxy res = pso3dim_static_opt(ackleys_function, bounds, less_than, ITERS);
   
    // Test returned value so that optimized version is forced to execute pso3dim_static_opt function
    return res.x > 2 ? 1 : 0;
}

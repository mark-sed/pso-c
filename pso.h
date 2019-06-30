#ifndef _PSO_H_
#define _PSO_H_

#include <stdbool.h>
#include <stdarg.h>

/**
 * Return type for statical PSO
 */
typedef struct {
    double x;  //< X coordinate
    double y;  //< Y coordinate
} TPSOxy;

/**
 * Fitness function
 */
typedef bool (* fit_func)(double, double);

/**
 * 3 dimensional function
 */
typedef double (* func3dim)(double, double);

/**
 * N dimansional function
 */
typedef double (* funcndim)(double, ...);

/**
 * Particle swarm optimization algorithm for 3 dimensional functions
 */
double* pso3dim(func3dim function, double bounds[3], fit_func fitness, unsigned int particle_am, unsigned long max_iter);

/**
 * Particle swarm optimization algorithm for n dimensional functions
 */
double* psondim(func3dim function, double *bounds, unsigned short dimensions, fit_func fitness, unsigned int particle_am, unsigned long max_iter);

/**
 * Particle swarm optimization algorithm that doesn't use dynamical
 * allocation. Can be used only for 2 dimensional functions.
 */
TPSOxy pso3dim_static(func3dim function, double bounds[3], fit_func fitness, unsigned int particle_am, unsigned long max_iter);

#endif //_PSO_H_

#include "pso.h"
#include "stddef.h"

/**
 * Particle swarm optimization algorithm for 3 dimensional functions
 */
double* pso3dim(func3dim function, double bounds[3], fit_func fitness, unsigned int particle_am, unsigned long max_iter){
    return NULL;
}


/**
 * Particle swarm optimization algorithm for n dimensional functions
 */
double* psondim(func3dim function, double *bounds, unsigned short dimensions, fit_func fitness, unsigned int particle_am, unsigned long max_iter){
    return NULL;
}

/**
 * Particle swarm optimization algorithm that doesn't use dynamical
 * allocation. Can be used only for 2 dimensional functions.
 */
TPSOxy pso3dim_static(func3dim function, double bounds[3], fit_func fitness, unsigned int particle_am, unsigned long max_iter){
    return (TPSOxy){0.0, 0.0};
}

#ifndef _PSO_H_
#define _PSO_H_

#include <stdbool.h>
#include <stdarg.h>

#define ASSERT_ALLOCATION  //< If this is defined, every allocation will be checked if it was successful

#define PSO3DIM_STATIC_PARTICLES 20  //< How many particles will be used in pso3dim_static function

#define COEFF_W  0.50  //< Inertia coefficient (should be in range of <0.4, 0.9>)
#define COEFF_CP 2.05  //< Cognitive coefficient (should be a little bit above 2)
#define COEFF_CG 2.05  //< Social coefficient (should have same or similar value as cognitive coefficient)

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


#ifdef ASSERT_ALLOCATION
/**
 * Error handling function
 */
void __attribute__((weak)) error_handler();
#endif //ASSERT_ALLOCATION

/**
 * Initializer function for PSO module
 * @warning This function should be called only once before any other PSO function is called
 * @note This function calls srand function
 */
void pso_init();

/**
 * Particle swarm optimization algorithm for 3 dimensional functions
 */
double* pso3dim(func3dim function, double bounds[2][2], fit_func fitness, unsigned int particle_am, unsigned long max_iter);

/**
 * Particle swarm optimization algorithm for n dimensional functions
 */
double* psondim(func3dim function, double *bounds[2], unsigned short dimensions, fit_func fitness, unsigned int particle_am, unsigned long max_iter);

/**
 * Particle swarm optimization algorithm that doesn't use dynamical
 * allocation. Can be used only for 2 dimensional functions.
 */
TPSOxy pso3dim_static(func3dim function, double bounds[2][2], fit_func fitness, unsigned long max_iter);

#endif //_PSO_H_

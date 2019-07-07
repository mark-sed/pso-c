/**
 * @file pso.h
 * @author Marek Sedláček
 * @email mr.mareksedlacek@gmail.com, xsedla1b@fit.vutbr.cz
 * @date July 2019
 * 
 * @brief Header file for PSO module
 *
 * This header file contains functions and type declarations
 * for Particle Swarm Optimization (PSO) module.
 * PSO module is able optimize functions using PSO algorithm.
 *
 * If macro `ASSERT_ALLOCATION` is defined all allocations
 * will be checked if they were successfull also an error
 * handler function `error_handler` will be declared. This
 * functions is defined as weak and no-return in pso.c module
 * (containing only `exit(1)` instruction) and can be 
 * overriden by re-declaration as non-weak function in other 
 * module.
 *
 */

#ifndef _PSO_H_
#define _PSO_H_

#include <stdbool.h>

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
 * Parameters are 2 values to be compared
 * @return true if 1st argument is better than 2nd argument
 */
typedef bool (* fit_func)(double, double);

/**
 * 3 dimensional function
 * Parameters are x and y value
 * @return function value at passed in coordinates
 */
typedef double (* func3dim)(double, double);

/**
 * N dimansional function
 * Parameter is array of coordinates
 * @return function value at passed in coordinates
 */
typedef double (* funcndim)(double *);


#ifdef ASSERT_ALLOCATION
/**
 * Error handler function
 * @note This function is defined in pso.c as weak and
 *       can be redifined elsewhere
 */
void __attribute__((weak)) error_handler();

#endif // ASSERT_ALLOCATION

/**
 * Initializer function for PSO module
 * @warning This function should be called only once before any other PSO function is called
 * @note This function calls srand function
 */
void pso_init();

/**
 * Particle swarm optimization algorithm for 3 dimensional functions
 * @param function Function in which is optimization done
 * @param bounds Bounds of the function in which will be the function optimized.
 *               this should be 2 arrays of 2 values where the 1st one is
 *               the minimum and second one is the maximum. E.g.: for `x in <0, 5> &
 *               y in <-10, 10>` the bounds should be `{{0.0, 5.0}, {-10.0, 10.0}}`.
 * @param fitness Fitness functions that determinates if passed in value is better
 *                than other passed in value
 * @param particle_am The amount of particles to use for optimization (10 to 20 is
 *                    usually good enough amount for most functions)
 * @param max_iter The amount of iterations that should be done.
 *                 More results in better precision but longer calculation.
 * @return Array with 2 doubles - the best found x and y coordinates.
 */
double* pso3dim(func3dim function, double bounds[2][2], fit_func fitness, unsigned int particle_am, unsigned long max_iter);

/**
 * Particle swarm optimization algorithm for n dimensional functions
 * @param function Function in which is optimization done
 * @param bounds Bounds of the function in which will be the function optimized.
 *               this should be n arrays (where n is the amount of dimensions of
 *               the optimized function) of 2 values where the 1st one is the
 *               minimum and second one is the maximum. E.g.: for `x in <0, 5> &
 *               y in <-10, 10>` the bounds should be `{{0.0, 5.0}, {-10.0, 10.0}}`.
 * @param dimensions The dimensions of optimized function.
 *                   Dimensions means how many coordinates are there in the optimized
 *                   function. E.g.: z = x^2 + y is 3 dimensional - it has 2 variables
 *                   plus the result (3 dimensions).
 * @param fitness Fitness functions that determinates if passed in value is better
 *                than other passed in value
 * @param particle_am The amount of particles to use for optimization (10 to 20 is
 *                    usually good enough amount for most functions)
 * @param max_iter The amount of iterations that should be done.
 *                 More results in better precision but longer calculation.
 * @return Array with n (n = dimensions - 1) doubles - the best found coordinates.
 */
double* psondim(funcndim function, double bounds[][2], unsigned short dimensions, fit_func fitness, unsigned int particle_am, unsigned long max_iter);

/**
 * Particle swarm optimization algorithm for 3 dimensional functions that does not use dynamical allocation
 * @param function Function in which is optimization done
 * @param bounds Bounds of the function in which will be the function optimized.
 *               this should be 2 arrays of 2 values where the 1st one is
 *               the minimum and second one is the maximum. E.g.: for `x in <0, 5> &
 *               y in <-10, 10>` the bounds should be `{{0.0, 5.0}, {-10.0, 10.0}}`.
 * @param fitness Fitness functions that determinates if passed in value is better
 *                than other passed in value
 * @param max_iter The amount of iterations that should be done.
 *                 More results in better precision but longer calculation.
 * @return Struct with 2 doubles - the best found x and y coordinates.
 * @note The amount of particles is determinated by the value of `PSO3DIM_STATIC_PARTICLES` macro
 */
TPSOxy pso3dim_static(func3dim function, double bounds[2][2], fit_func fitness, unsigned long max_iter);

#endif //_PSO_H_

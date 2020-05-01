/**
 * @file pso.c
 * @author Marek Sedláček
 * @date July 2019
 * 
 * @brief Source file for PSO module
 *
 * This file contains function definitions for Particle
 * Swarm Optimization (PSO) module.
 * PSO module is able optimize functions using PSO algorithm.
 * 
 * This code was made for my bachelor's thesis at
 * Brno University of Technology
 */

#include "pso.h"
#include "stddef.h"
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include <string.h>

/**
 * Macro for generating random double in passed in range.
 * Inspired by https://ubuntuforums.org/showthread.php?t=1717717&p=10618266#post10618266
 * 
 * @param min Minimal value of generated number
 * @param max Maximal value of generated number
 * @return random double in range of <min, max>
 * @note casts are done so that this function can take also integers and 
 *       parentheses are included in case of arguments being expressions.
 */
#define random_double(min, max) (double)((double)(min) + (rand()/(RAND_MAX/((double)(max)-(double)(min)))))

/**
 * 3 dimensional particle struct
 */
typedef struct {
    double velocity[2];  //< Velocity for each dimension
    double position[2];  //< Position in each dimension
    double best_pos[2];  //< Best position
    double best_val;     //< Value of the best position
} TParticle3Dim;

/**
 * Particle struct
 */
typedef struct {
    double *velocity;  //< Velocity for each dimension
    double *position;  //< Position in each dimension
    double *best_pos;  //< Best position
    double best_val;   //< Value of the best position
} TParticle;


#ifdef ASSERT_ALLOCATION
/**
 * Error handler function
 * @note This function is weak and can be redifined elsewhere
 */
_Noreturn void __attribute__((weak)) error_handler(){
    exit(1);
}
#endif //ASSERT_ALLOCATION

/**
 * Initializer function for PSO module
 * @warning This function should be called only once before any other PSO function is called
 * @note This function calls srand function
 */
void pso_init(){
    // Initializing pseudo-random generator
    srand(time(NULL));
}

/**
 * Initializes starting attributes for a particle
 * @param p Particle to be initialized
 * @param bounds Function bounds
 */
static void init_particle3dim(TParticle3Dim *p, double bounds[2][2]){
    // Set values for every dimension
    // Random velocity from -1 to 1
    p->velocity[0] = random_double(-1, 1);
    p->velocity[1] = random_double(-1, 1);
    // Random position from minimal possible to maximal and set best position to current
    p->best_pos[0] = p->position[0] = random_double(bounds[0][0], bounds[0][1]);
    p->best_pos[1] = p->position[1] = random_double(bounds[1][0], bounds[1][1]);
}

/**
 * Update velocity and position of a particle based on best global best position found
 * @param p Particle to be updated
 * @param bounds Function bounds
 * @param best_pos Global best position
 */
static void update_particle3dim(TParticle3Dim *p, double bounds[2][2], double best_pos[2]){
    // Random coefficient pre-multiplied by cognitive/social coefficient
    double rp = random_double(0, 1) * COEFF_CP;
    double rg = random_double(0, 1) * COEFF_CG;

    // Calculate new velocity for both dimensions
    // Calculating with non-dependent values first to not slow down processing pipeline
    // Differences are pre-calculated to avoid calculating them twice
    double pos_diff0 = best_pos[0] - p->position[0];
    double pos_diff1 = best_pos[1] - p->position[1];
    p->velocity[0] = COEFF_W * p->velocity[0] + rp * pos_diff0 + rg * pos_diff0;
    p->velocity[1] = COEFF_W * p->velocity[1] + rp * pos_diff1 + rg * pos_diff1;

    // Calculate new position
    // Adjust if new position is out of bounds
    p->position[0] += p->velocity[0];
    p->position[1] += p->velocity[1];

    // Check bounds for 1st dimension
    if(p->position[0] < bounds[0][0]){
        p->position[0] = bounds[0][0];
    }
    else if(p->position[0] > bounds[0][1]){
        p->position[0] = bounds[0][1];
    }

    // Check bounds for 2nd dimension
    if(p->position[1] < bounds[1][0]){
        p->position[1] = bounds[1][0];
    }
    else if(p->position[1] > bounds[1][1]){
        p->position[1] = bounds[1][1];
    }
}

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
double *pso3dim(func3dim function, double bounds[2][2], fit_func fitness, unsigned int particle_am, unsigned long max_iter){
    // Creating array of particles
    TParticle3Dim *swarm = malloc(sizeof(TParticle3Dim)*particle_am);
#ifdef ASSERT_ALLOCATION
    if(!swarm){
        error_handler();
    }
#endif // ASSERT_ALLOCATION
    // Set each particle's attributes
    for(unsigned int i = 0; i < particle_am; i++){
        init_particle3dim(&(swarm[i]), bounds);
    }

    double *best_pos = malloc(sizeof(double) * 2);  // Global best position
#ifdef ASSERT_ALLOCATION
    if(!best_pos){
        free(swarm);
        error_handler();
    }
#endif // ASSERT_ALLOCATION
    double best_value = DBL_MAX;  // Global best value (for best position)

    for(unsigned long i = 0; i < max_iter; i++){
        for(unsigned int a = 0; a < particle_am; a++){
            // Evaluate current position of the current particle
            double value = function(swarm[a].position[0], swarm[a].position[1]);
            // Check if this is new personal best value
            if(fitness(value, swarm[a].best_val) || i == 0){
                // Save the personal best position and value
                swarm[a].best_val = value;
                swarm[a].best_pos[0] = swarm[a].position[0];
                swarm[a].best_pos[1] = swarm[a].position[1];
                // Now check if the value is better than global best value
                // This can be inside this if statement because any global best
                //   has to have better or same fitness function value than 
                //   any personal best
                if(fitness(value, best_value) || best_value == DBL_MAX){
                    best_value = value;
                    best_pos[0] = swarm[a].position[0];
                    best_pos[1] = swarm[a].position[1];
                }
            }
        }
        // Updating the velocity and position of particles
        for(unsigned int a = 0; a < particle_am; a++){
            update_particle3dim(&(swarm[a]), bounds, best_pos);      
        }
    }
    
    // Free allocated memory
    free(swarm);
    return best_pos;
}

/**
 * Initializes n dimensional particle
 * @param p Particle to be initialized
 * @param bounds Function bounds
 * @param coords How many coordinates need to be initialized (dimensions - 1)
 */
static void init_particlendim(TParticle *p, double bounds[][2], unsigned short coords){
   // Set random velocity and position for every dimension
   for(unsigned short i = 0; i < coords; i++){
        p->velocity[i] = random_double(-1, 1);
        p->position[i] = random_double(bounds[i][0], bounds[i][1]);
   }
}

/**
 * Update n dimensional particle's velocity and position
 * @param p Particle to be initialized
 * @param bounds Function bounds
 * @param best_pos Best global position
 * @param coords How many coordinates need to be initialized (dimensions - 1)
 */
static void update_particlendim(TParticle *p, double bounds[][2], double *best_pos, unsigned short coords){
    // Random coefficient pre-multiplied by cognitive/social coefficient
    double rp = random_double(0, 1) * COEFF_CP;
    double rg = random_double(0, 1) * COEFF_CG;

    // Calculate new velocity for all dimensions
    // Differences are pre-calculated to avoid calculating them twice
    for(unsigned short i = 0; i < coords; i++){
        double pos_diff = best_pos[i] - p->position[i];
        p->velocity[i] = COEFF_W * p->velocity[i] + rp * pos_diff + rg * pos_diff;

        // Update position of the particle
        p->position[i] += p->velocity[i];

        // Check if new position is still in bounds
        if(p->position[i] < bounds[i][0]){
            p->position[i] = bounds[i][0];
        }
        else if(p->position[i] > bounds[i][1]){
            p->position[i] = bounds[i][1];
        }
    }
}

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
double* psondim(funcndim function, double bounds[][2], unsigned short dimensions, fit_func fitness, unsigned int particle_am, unsigned long max_iter){
    // Adjust dimensions (eg.: 3 dimensions means only 2 coordinates will be saved - 3rd will be the result of this functions)
    dimensions--; 
    // Create swarm
    TParticle *swarm = malloc(sizeof(TParticle)*particle_am);
#ifdef ASSERT_ALLOCATION
    if(!swarm){
        error_handler();
    }
#endif // ASSERT_ALLOCATION
    // Allocate particle attributes to correct dimensions
    // and set each particle's attributes
    for(unsigned int i = 0; i < particle_am; i++){
        // Allocate space for velocity for all dimensions
        swarm[i].velocity = malloc(sizeof(double)*dimensions);
#ifdef ASSERT_ALLOCATION
        if(!swarm[i].velocity){
            // All previously allocated attributes have to be freed
            for(unsigned int a = 0; a < i; a++){
                free(swarm[a].velocity);
                free(swarm[a].position);
                free(swarm[a].best_pos);
            }
            // Free the array itself
            free(swarm);
            // Call error handler
            error_handler();
        }
#endif // ASSERT_ALLOCATION
        
        // Allocate space for position for all dimensions
        swarm[i].position = malloc(sizeof(double)*dimensions);
#ifdef ASSERT_ALLOCATION
        if(!swarm[i].position){
            // All previously allocated attributes have to be freed
            free(swarm[i].velocity);
            for(unsigned int a = 0; a < i; a++){
                free(swarm[a].velocity);
                free(swarm[a].position);
                free(swarm[a].best_pos);
            }
            // Free the array itself
            free(swarm);
            // Call error handler
            error_handler();
        }
#endif // ASSERT_ALLOCATION

        // Allocate space for best position for all dimensions
        swarm[i].best_pos = malloc(sizeof(double)*dimensions);
#ifdef ASSERT_ALLOCATION
        if(!swarm[i].best_pos){
            // All previously allocated attributes have to be freed
            free(swarm[i].velocity);
            free(swarm[i].position);
            for(unsigned int a = 0; a < i; a++){
                free(swarm[a].velocity);
                free(swarm[a].position);
                free(swarm[a].best_pos);
            }
            // Free the array itself
            free(swarm);
            // Call error handler
            error_handler();
        }
#endif // ASSERT_ALLOCATION

        // Initialize the particle
        init_particlendim(&(swarm[i]), bounds, dimensions);
    }

    double *best_pos = malloc(sizeof(double) * dimensions);  // Global best position
#ifdef ASSERT_ALLOCATION
    if(!best_pos){
    	// Free all previously allocated resources
        for(unsigned int i = 0; i < particle_am; i++){
            free(swarm[i].velocity);
            free(swarm[i].position);
            free(swarm[i].best_pos);
        }
        free(swarm);
        error_handler();
    }
#endif // ASSERT_ALLOCATION
    double best_value = DBL_MAX;  // Global best value (for best position)

    for(unsigned long i = 0; i < max_iter; i++){
        for(unsigned int a = 0; a < particle_am; a++){
            // Evaluate current position of the current particle
            double value = function(swarm[a].position);
            // Check if this is new personal best value
            if(fitness(value, swarm[a].best_val) || i == 0){
                // Save the personal best position and value
                swarm[a].best_val = value;
                // Copying position into personal best position
                //   memory does not overlap, so memcpy is used
                memcpy(swarm[a].best_pos, swarm[a].position, sizeof(double)*dimensions);
                // Now check if the value is better than global best value
                // This can be inside this if statement because any global best
                //   has to have better or same fitness function value than
                //   any personal best
                if(fitness(value, best_value) || best_value == DBL_MAX){
                    best_value = value;
                    memcpy(best_pos, swarm[a].position, sizeof(double)*dimensions);
                }
            }
        }
        // Updating the velocity and position of particles
        for(unsigned int a = 0; a < particle_am; a++){
            update_particlendim(&(swarm[a]), bounds, best_pos, dimensions);
        }
    }

    // Free allocated memory
    for(unsigned int i = 0; i < particle_am; i++){
        free(swarm[i].velocity);
        free(swarm[i].position);
        free(swarm[i].best_pos);
    }
    free(swarm);
    return best_pos;
}

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
TPSOxy pso3dim_static(func3dim function, double bounds[2][2], fit_func fitness, unsigned long max_iter){
    // Create array of particles (swarm)
    TParticle3Dim swarm[PSO3DIM_STATIC_PARTICLES];
    // Initialize the particles
    for(unsigned int i = 0; i < PSO3DIM_STATIC_PARTICLES; i++){
        init_particle3dim(&(swarm[i]), bounds);
    }

    double best_pos[2] = {0.0, 0.0};
    double best_value = DBL_MAX;

    for(unsigned long i = 0; i < max_iter; i++){
        for(unsigned int a = 0; a < PSO3DIM_STATIC_PARTICLES; a++){
            // Evaluate current position of the current particle
            double value = function(swarm[a].position[0], swarm[a].position[1]);
            // Check if this is new personal best value
            if(fitness(value, swarm[a].best_val) || i == 0){
                // Save the personal best position and value
                swarm[a].best_val = value;
                swarm[a].best_pos[0] = swarm[a].position[0];
                swarm[a].best_pos[1] = swarm[a].position[1];
                // Now check if the value is better than global best value
                // This can be inside this if statement because any global best
                //   has to have better or same fitness function value than
                //   any personal best
                if(fitness(value, best_value) || best_value == DBL_MAX){
                    best_value = value;
                    best_pos[0] = swarm[a].position[0];
                    best_pos[1] = swarm[a].position[1];
                }
            }
        }
        // Updating the velocity and position of particles
        for(unsigned int a = 0; a < PSO3DIM_STATIC_PARTICLES; a++){
            update_particle3dim(&(swarm[a]), bounds, best_pos);
        }
    }

    return (TPSOxy){best_pos[0], best_pos[1]};
}

/**
 * Particle swarm optimization algorithm for 3 dimensional functions that does not use dynamical allocation
 * Optimized by not using as many function calls
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
TPSOxy pso3dim_static_opt(func3dim function, double bounds[2][2], fit_func fitness, unsigned long max_iter){
    // Create array of particles (swarm)
    TParticle3Dim swarm[PSO3DIM_STATIC_PARTICLES];
    // Initialize the particles
    for(unsigned int i = 0; i < PSO3DIM_STATIC_PARTICLES; i++){
    	TParticle3Dim *p = &(swarm[i]);
        // Set values for every dimension
	    // Random velocity from -1 to 1
	    p->velocity[0] = random_double(-1, 1);
	    p->velocity[1] = random_double(-1, 1);
	    // Random position from minimal possible to maximal and set best position to current
	    p->best_pos[0] = p->position[0] = random_double(bounds[0][0], bounds[0][1]);
	    p->best_pos[1] = p->position[1] = random_double(bounds[1][0], bounds[1][1]);
    }

    double best_pos[2] = {0.0, 0.0};
    double best_value = DBL_MAX;

    for(unsigned long i = 0; i < max_iter; i++){
        for(unsigned int a = 0; a < PSO3DIM_STATIC_PARTICLES; a++){
            // Evaluate current position of the current particle
            double value = function(swarm[a].position[0], swarm[a].position[1]);
            // Check if this is new personal best value
            if(fitness(value, swarm[a].best_val) || i == 0){
                // Save the personal best position and value
                swarm[a].best_val = value;
                swarm[a].best_pos[0] = swarm[a].position[0];
                swarm[a].best_pos[1] = swarm[a].position[1];
                // Now check if the value is better than global best value
                // This can be inside this if statement because any global best
                //   has to have better or same fitness function value than
                //   any personal best
                if(fitness(value, best_value) || best_value == DBL_MAX){
                    best_value = value;
                    best_pos[0] = swarm[a].position[0];
                    best_pos[1] = swarm[a].position[1];
                }
            }
        }
        // Updating the velocity and position of particles
        for(unsigned int a = 0; a < PSO3DIM_STATIC_PARTICLES; a++){
        	TParticle3Dim *p = &(swarm[a]);
            // Random coefficient pre-multiplied by cognitive/social coefficient
		    double rp = random_double(0, 1) * COEFF_CP;
		    double rg = random_double(0, 1) * COEFF_CG;

		    // Calculate new velocity for both dimensions
		    // Calculating with non-dependent values first to not slow down processing pipeline
		    // Differences are pre-calculated to avoid calculating them twice
		    double pos_diff0 = best_pos[0] - p->position[0];
		    double pos_diff1 = best_pos[1] - p->position[1];
		    p->velocity[0] = COEFF_W * p->velocity[0] + rp * pos_diff0 + rg * pos_diff0;
		    p->velocity[1] = COEFF_W * p->velocity[1] + rp * pos_diff1 + rg * pos_diff1;

		    // Calculate new position
		    // Adjust if new position is out of bounds
		    p->position[0] += p->velocity[0];
		    p->position[1] += p->velocity[1];

		    // Check bounds for 1st dimension
		    if(p->position[0] < bounds[0][0]){
		        p->position[0] = bounds[0][0];
		    }
		    else if(p->position[0] > bounds[0][1]){
		        p->position[0] = bounds[0][1];
		    }

		    // Check bounds for 2nd dimension
		    if(p->position[1] < bounds[1][0]){
		        p->position[1] = bounds[1][0];
		    }
		    else if(p->position[1] > bounds[1][1]){
		        p->position[1] = bounds[1][1];
		    }
        }
    }

    return (TPSOxy){best_pos[0], best_pos[1]};
}

#include "pso.h"
#include "stddef.h"
#include <stdlib.h>
#include <time.h>
#include <float.h>

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
 * Error handling function
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
 * Particle swarm optimization algorithm for n dimensional functions
 */
double* psondim(func3dim function, double *bounds[2], unsigned short dimensions, fit_func fitness, unsigned int particle_am, unsigned long max_iter){
    return NULL;
}

/**
 * Particle swarm optimization algorithm that doesn't use dynamical
 * allocation. Can be used only for 2 dimensional functions.
 */
TPSOxy pso3dim_static(func3dim function, double bounds[2][2], fit_func fitness, unsigned long max_iter){
    // Create array of particles (swarm)
    TParticle3Dim swarm[PSO3DIM_STATIC_PARTICLES];
    // Initialize the particles
    for(unsigned int i = 0; i < PSO3DIM_STATIC_PARTICLES; i++){
        init_particle3dim(&(swarm[i]), bounds);
    }

    double best_pos[2];
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

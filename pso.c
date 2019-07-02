#include "pso.h"
#include "stddef.h"
#include <stdlib.h>
#include <time.h>

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
    double velocity[3];  //< Velocity for each dimension
    double position[3];  //< Position in each dimension
    double best_pos[3];  //< Best position
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
_Noreturn void error_handler(){
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
static void init_particle3dim(TParticle3Dim *p){
    // TODO
}

/**
 * Particle swarm optimization algorithm for 3 dimensional functions
 */
double* pso3dim(func3dim function, double bounds[3], fit_func fitness, unsigned int particle_am, unsigned long max_iter){
    // Creating array of particles
    TParticle3Dim *swarm = malloc(sizeof(TParticle3Dim)*particle_am);
#ifdef ASSERT_ALLOCATION
    if(!swarm){
        error_handler();
    }
#endif //ASSERT_ALLOCATION
    // Set each particle's attributes


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

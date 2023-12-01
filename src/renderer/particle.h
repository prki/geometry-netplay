#ifndef _PARTICLE_H_
#define _PARTICLE_H_
#include <stdlib.h>

#include "../math/shapes.h"  // [TODO] I don't like this - doesn't feel natural architecture for C to move dependency across tree structure - should rather be an include folder? or some kind of library linking anyways?

// Initially for testing purposes, particles will be drawn as single points.
// Later on, they will be implemented as proper shapes with textures.
typedef struct Particle {
  vec2d direction;
  double velocity;
  int is_active;
  int decay_rate;
  int alpha;
  vec2d position;
} Particle;

void disable_particle(Particle* particle);
void enable_particle(Particle* particle);
void initialize_particle(Particle* particle);
void update_particle(Particle* particle);

typedef struct ParticlePool {
  Particle* particles;
  size_t pool_size;
} ParticlePool;

ParticlePool* new_particle_pool(void);
void destroy_particle_pool(ParticlePool* pool);
Particle* get_inactive_particle(ParticlePool* pool);
void update_particles(ParticlePool* pool);

#endif  // _PARTICLE_H_

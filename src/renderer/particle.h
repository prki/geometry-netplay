#ifndef _PARTICLE_H_
#define _PARTICLE_H_
#include <stdlib.h>

#include "../math/vector.h"

#define PARTICLE_TIME_TO_LIVE 300  // ms

// Initially for testing purposes, particles will be drawn as single points.
// Later on, they will be implemented as proper shapes with textures.
typedef struct Particle {
  vec2d direction;
  double velocity;
  int is_active;
  double time_to_live;
  double time_alive;
  // double decay_rate;
  // double alpha;
  vec2d position;
} Particle;

void disable_particle(Particle* particle);
void enable_particle(Particle* particle);
void initialize_particle(Particle* particle);
void update_particle(Particle* particle, double delta_time);

typedef struct ParticlePool {
  Particle* particles;
  size_t pool_size;
} ParticlePool;

ParticlePool* new_particle_pool(void);
void destroy_particle_pool(ParticlePool* pool);
Particle* get_inactive_particle(ParticlePool* pool);
void update_particles(ParticlePool* pool, double delta_time);

#endif  // _PARTICLE_H_

#include "particle.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../math/vector.h"

void disable_particle(Particle* particle) { particle->is_active = 0; }

void enable_particle(Particle* particle) { particle->is_active = 1; }

// Initialization function for particles with "sensible zeros". Note that
// direction and velocity are set to a NAN vector and a NAN, respectively.
void initialize_particle(Particle* particle) {
  particle->direction = (vec2d){.x = NAN, .y = NAN};
  particle->velocity = NAN;
  particle->is_active = 0;
  particle->time_to_live = 0;
  particle->time_alive = 0;
}

void update_particle(Particle* particle, double delta_time) {
  if (particle->is_active == 0) {
    return;
  }

  particle->position = (vec2d){
      .x = particle->position.x + particle->direction.x * particle->velocity,
      .y = particle->position.y + particle->direction.y * particle->velocity};
  particle->time_alive += delta_time * 1000.0;
  if (particle->time_alive >= particle->time_to_live) {
    disable_particle(particle);
  }
}

ParticlePool* new_particle_pool(void) {
  size_t pool_size = 5000;

  Particle* particles = malloc(pool_size * sizeof(Particle));
  if (particles == NULL) {
    printf("[ERROR] malloc fail new particle pool particles\n");
    return NULL;
  }

  for (size_t i = 0; i < pool_size; i++) {
    initialize_particle(&particles[i]);
  }

  ParticlePool* pool = malloc(sizeof(ParticlePool));
  if (pool == NULL) {
    printf("[ERROR] malloc fail ParticlePool\n");
    free(particles);
    return NULL;
  }

  pool->particles = particles;
  pool->pool_size = pool_size;

  return pool;
}

void destroy_particle_pool(ParticlePool* pool) {
  if (pool != NULL) {
    if (pool->particles != NULL) {
      free(pool->particles);
    }
    free(pool);
  }
}

Particle* get_inactive_particle(ParticlePool* pool) {
  if (pool == NULL) {
    printf(
        "[WARN] Attempted to get inactive particle from pool, but pool was "
        "NULL\n");
    return NULL;
  }

  for (size_t i = 0; i < pool->pool_size; i++) {
    if (pool->particles[i].is_active == 0) {
      return &pool->particles[i];
    }
  }

  printf("[WARN] No inactive particle found in pool\n");
  return NULL;
}

void update_particles(ParticlePool* pool, double delta_time) {
  for (size_t i = 0; i < pool->pool_size; i++) {
    update_particle(&pool->particles[i], delta_time);
  }
}

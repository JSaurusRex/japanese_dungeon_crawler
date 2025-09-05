#ifndef PARTICLES_H
#define PARTICLES_H

#include "battle.h"

typedef enum {PARTICLE_DUST, PARTICLE_BLOOD, NUM_OF_PARTICLES} Particle_type;

typedef struct
{
    bool used;
    Vector2 position;
    Vector2 velocity;
    float gravity;
    float lifetime;
    float damping;
    Particle_type type;
} Particle;

#define MAX_PARTICLES 100

void emit_particles_explosion(Particle_type type, float lifetime, Vector2 position, Vector2 velocity, float gravity, float damping, int amount);
void add_particle(Vector2 position, Vector2 velocity, float gravity, Particle_type type, float lifetime, float damping);
void remove_particle(int particle);
void draw_particles();
void clear_all_particles();

typedef struct
{
    bool used;
    float lifetime;
    int amount;
    Element element;
    Vector2 position;
} sDamageNumberParticle;

#define MAX_DAMAGE_NUMBER_PARTICLES 100
void add_damage_number_particle(Vector2 position, Element element, int amount);

#endif
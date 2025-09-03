#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include <raymath.h>
#include <math.h>

#include "main.h"
#include "particles.h"

Particle _particles[MAX_PARTICLES] = {0};
int _empty_particles_index = -1;
int _empty_particles[MAX_PARTICLES] = {0};

void add_particle(Vector2 position, Vector2 velocity, float gravity, Particle_type type, float lifetime, float damping)
{
    int spot = -1;
    if(_empty_particles_index < 0)
    {
        for(int i = 0; i < MAX_PARTICLES; i++)
        {
            if (_particles[i].used)
                continue;
            
            spot = i;
            break;
        }
    }
    else
    {
        spot = _empty_particles[_empty_particles_index];
        _empty_particles_index--;
    }

    if (spot < 0)
        return;

    if (_particles[spot].used && _printDebug)
        printf("debug: add_particle: particle spot taken %i\n", spot);
    
    printf("adding particle!!\n");
    
    _particles[spot].used = true;
    _particles[spot].gravity = gravity;
    _particles[spot].type = type;
    _particles[spot].velocity = velocity;
    _particles[spot].position = position;
    _particles[spot].lifetime = lifetime;
    _particles[spot].damping = damping;
}

void remove_particle(int particle)
{
    if (particle < 0 || particle >= MAX_PARTICLES)
    {
        printf("error: remove_particle: particle index outta bounds %i\n", particle);
        return;
    }

    _empty_particles_index++;

    if(_empty_particles_index < 0 || _empty_particles_index >= MAX_PARTICLES)
    {
        printf("error: remove_particle: _empty_particles_index outta bounds %i\n", _empty_particles_index);
        return;
    }

    printf("removing particles\n");

    _empty_particles[_empty_particles_index] = particle;

    _particles[particle].used = false;
}

void emit_particles_explosion(Particle_type type, float lifetime, Vector2 position, Vector2 velocity, float gravity, float damping, int amount)
{
    printf("emitting particles!\n");
    for(int i = 0; i < amount; i++)
    {
        velocity.x = ((rand() % (int)fmax(velocity.x * 200, 1)) - velocity.x*100) / 100.0;
        velocity.y = ((rand() % (int)fmax(velocity.y * 200, 1)) - velocity.y*100) / 100.0;

        printf("velocity %.2f, %.2f\n", velocity.x, velocity.y);

        add_particle(position, velocity, gravity, type, lifetime, damping);
    }
}

void draw_particles()
{
    float delta = GetFrameTime();
    for(int i = 0; i < MAX_PARTICLES; i++)
    {
        if (!_particles[i].used)
            continue;
        
        _particles[i].velocity.y += _particles[i].gravity * delta;

        _particles[i].velocity = Vector2MoveTowards(_particles[i].velocity, (Vector2){0,0}, delta*_particles[i].damping);

        _particles[i].position = Vector2Add(_particles[i].position, Vector2Scale(_particles[i].velocity, delta));

        _particles[i].lifetime -= delta;
        
        if (_particles[i].lifetime < 0)
            remove_particle(i);
        
        switch(_particles[i].type)
        {
            case PARTICLE_DUST:
            {
                DrawCircle(_particles[i].position.x, _particles[i].position.y, 5, ColorAlpha(GRAY, 0.4));
                break;
            }

            case PARTICLE_BLOOD:
            {
                DrawCircle(_particles[i].position.x, _particles[i].position.y, 5, ColorAlpha(RED, 0.5));
                break;
            }
        }
    }
}
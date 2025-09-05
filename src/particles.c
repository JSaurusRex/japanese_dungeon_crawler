#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include "rendering.h"
#include <raymath.h>
#include <math.h>

#include "main.h"
#include "particles.h"
#include "battle.h"

Particle _particles[MAX_PARTICLES] = {0};
int _empty_particles_index = -1;
int _empty_particles[MAX_PARTICLES] = {0};

void draw_damage_number_particles();

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
                drawCircle(_particles[i].position.x, _particles[i].position.y, 5, ColorAlpha(GRAY, 0.4));
                break;
            }

            case PARTICLE_BLOOD:
            {
                drawCircle(_particles[i].position.x, _particles[i].position.y, 5, ColorAlpha(RED, 0.5));
                break;
            }
        }
    }

    draw_damage_number_particles();
}

sDamageNumberParticle _damage_number_particles[MAX_DAMAGE_NUMBER_PARTICLES] = {0};
int _empty_damage_number_particles_index = -1;
int _empty_damage_number_particles[MAX_PARTICLES] = {0};

void add_damage_number_particle(Vector2 position, Element element, int amount)
{
    int spot = -1;
    if(_empty_damage_number_particles_index < 0)
    {
        for(int i = 0; i < MAX_PARTICLES; i++)
        {
            if (_damage_number_particles[i].used)
                continue;
            
            spot = i;
            break;
        }
    }
    else
    {
        spot = _empty_damage_number_particles[_empty_damage_number_particles_index];
        _empty_damage_number_particles_index--;
    }

    if (spot < 0)
        return;

    if (_damage_number_particles[spot].used && _printDebug)
        printf("debug: add_particle: particle spot taken %i\n", spot);
    
    printf("adding particle!!\n");

    Vector2 randomOffset = {rand() % 100 - 50, rand() % 100 - 50};
    
    _damage_number_particles[spot].used = true;
    _damage_number_particles[spot].amount = amount;
    _damage_number_particles[spot].position = Vector2Add(position, randomOffset);
    _damage_number_particles[spot].element = element;
    _damage_number_particles[spot].lifetime = 2;
}

void remove_damage_number_particle(int particle)
{
    if (particle < 0 || particle >= MAX_PARTICLES)
    {
        printf("error: remove_particle: particle index outta bounds %i\n", particle);
        return;
    }

    _empty_damage_number_particles_index++;

    if(_empty_damage_number_particles_index < 0 || _empty_damage_number_particles_index >= MAX_PARTICLES)
    {
        printf("error: remove_particle: _empty_damage_number_particles_index outta bounds %i\n", _empty_damage_number_particles_index);
        return;
    }

    printf("removing particles\n");

    _empty_damage_number_particles[_empty_damage_number_particles_index] = particle;

    _damage_number_particles[particle].used = false;
}

void draw_damage_number_particles()
{
    float delta = GetFrameTime();
    for(int i = 0; i < MAX_PARTICLES; i++)
    {
        if (!_damage_number_particles[i].used)
            continue;
        
        _damage_number_particles[i].lifetime -= delta;

        if (_damage_number_particles[i].lifetime < 0)
            remove_damage_number_particle(i);
        
        Color color;
        switch(_damage_number_particles[i].element)
        {
            case ELEMENT_NONE:
            {
                color = RED;

                if (_damage_number_particles[i].amount > 0)
                    color = GREEN;
                break;
            }

            case ELEMENT_FIRE:
            {
                color = ORANGE;
                break;
            }

            case ELEMENT_WIND:
            {
                color = LIGHTGRAY;
                break;
            }

            case ELEMENT_EARTH:
            {
                color = BROWN;
                break;
            }

            case ELEMENT_WATER:
            {
                color = BLUE;
                break;
            }
        }

        char str[STRING_LENGTH];
        snprintf(str, STRING_LENGTH, "%i", _damage_number_particles[i].amount);
        drawText(str, _damage_number_particles[i].position.x, _damage_number_particles[i].position.y, 14, color);
    }
}
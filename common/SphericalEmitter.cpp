#include "SphericalEmitter.h"
#include "iostream"
#include <algorithm>


SphericalEmitter::SphericalEmitter(Drawable* _model, int number, glm::vec3 Pos, glm::vec4 BegCol, glm::vec4 EndCol,float RadiusMax) :
    ParticleEmitter(_model, number, Pos) {
    ColorBegin = BegCol;
    ColorEnd = BegCol;
    this->RadiusMax = RadiusMax;
    
    for (int i = 0; i < number_of_particles; i++) {
        createNewParticle(i);
    }
}


void SphericalEmitter::createNewParticle(int index, glm::vec3 dir) {
    particleAttributes& particle = p_attributes[index];

    particle.radius = RAND * (RadiusMax - 0.1) + 0.1;
    particle.Rotation = 360 * RAND;
    float theta = 360 * RAND / 2;
    glm::vec3 partPos = particle.radius * glm::vec3(sin(particle.Rotation) * cos(theta), cos(particle.Rotation), sin(particle.Rotation) * sin(theta));

    particle.position = EmitterPos + partPos;

    particle.velocity = partPos;
    particle.velocity /= CalcMag(particle.velocity);


    particle.LifeRemaining = RAND;


    particle.life = 1.0f;
    particle.DeathRate = this->DeathRate;

    particle.ColorBegin = this->ColorBegin;
    particle.ColorEnd = this->ColorEnd;
    particle.SizeBegin = 0.04;
}


void SphericalEmitter::updateParticles(int new_particles, float dt, glm::vec3  , glm::vec3 vel) {

    for (unsigned int i = 0; i < new_particles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        createNewParticle(unusedParticle);
    }

    for (int i = 0; i < number_of_particles; i++) {
        particleAttributes& particle = p_attributes[i];
        if (particle.LifeRemaining > 0.0f)
        {

            particle.Rotation += 0.1f * dt;
            particle.position += particle.velocity * dt / 5.0f;

        }


    }


}


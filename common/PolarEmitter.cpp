#include "PolarEmitter.h"
#include "iostream"
#include <algorithm>


PolarEmitter::PolarEmitter(Drawable* _model, int number, glm::vec3 Pos,glm::vec3 Dir) : 
    ParticleEmitter(_model, number,Pos){
    //TailDir = Dir;
    DeathRate = 0.7f;
    for (int i = 0; i < number_of_particles; i++) {
        createNewParticle(i,Dir);
    }


}PolarEmitter::PolarEmitter(Drawable* _model, int number, glm::vec3 Pos,glm::vec3 Dir, glm::vec4 ClrBeg, glm::vec4 ClrEnd) :
    ParticleEmitter(_model, number,Pos){
    ColorBegin = ClrBeg;
    ColorEnd = ClrEnd;
    DeathRate = 0.7f;
    for (int i = 0; i < number_of_particles; i++) {
        createNewParticle(i,Dir);
    }
}


void PolarEmitter::createNewParticle(int index, glm::vec3 dir) {
    particleAttributes& particle = p_attributes[index];

    particle.radius = RAND * (0.5 - 0.1) + 0.1;
    particle.Rotation = 360 * RAND;

    glm::vec3 partPos = glm::vec3(particle.radius * sin(particle.Rotation), 0.0f, particle.radius * cos(particle.Rotation));

    glm::vec3 temp(0, 1, 0);
    //temp = normalize(temp);
    glm::vec3 prod = cross(temp, dir);
    float angle = acos(dot(temp, dir));
    glm::mat4 rotation = glm::rotate(glm::mat4(), angle, prod);
    partPos = glm::vec3(rotation * glm::vec4(partPos, 1));

    particle.position = EmitterPos + partPos;

    particle.velocity = partPos;
    particle.velocity /= CalcMag(particle.velocity);


    particle.LifeRemaining = RAND;


    particle.life = 1.0f;

    particle.ColorBegin = ColorBegin;
    particle.ColorEnd = ColorEnd;
    particle.SizeBegin = 0.02f;
}


void PolarEmitter::updateParticles(int new_particles, float dt, glm::vec3 dir, glm::vec3 vel) {
    for (unsigned int i = 0; i < new_particles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        createNewParticle(unusedParticle, dir);
    }

    for (int i = 0; i < number_of_particles; i++) {
        particleAttributes& particle = p_attributes[i];
        if (particle.LifeRemaining > 0.0f)
        {
            particle.LifeRemaining -= dt;

            particle.Rotation += 0.1f * dt;
            particle.position += particle.velocity * dt / 5.0f;

        }


    }


}

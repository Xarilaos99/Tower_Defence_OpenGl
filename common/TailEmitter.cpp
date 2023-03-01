#include "TailEmitter.h"
#include "iostream"
#include <algorithm>


TailEmitter::TailEmitter(Drawable* _model, int number, glm::vec3 Pos, glm::vec3 TowerPos, float VelFactor) :
    ParticleEmitter(_model, number, Pos)  {
    this->TowerPos = TowerPos;
    this->VelFactor = VelFactor;
    DeathRate = 0.7f;
    for (int i = 0; i < number_of_particles; i++) {
        createNewParticle(i);
    }
}

TailEmitter::TailEmitter(Drawable* _model, int number, glm::vec3 Pos, glm::vec4 ClrBeg,glm::vec4 ClrEnd, float VelFactor) :
    ParticleEmitter(_model, number, Pos) {
    this->VelFactor = VelFactor;
    this->TowerPos = Pos;
    DeathRate = 0.7f;
    ColorBegin = ClrBeg;
    ColorEnd = ClrEnd;
    for (int i = 0; i < number_of_particles; i++) {
        createNewParticle(i);
    }


}


void TailEmitter::createNewParticle(int index, glm::vec3 dir) {
    float exp, y;
    float _z, z;
    float  x;
    glm::vec3 posx;
    glm::vec3 posz;
    glm::vec3 pos;

    glm::mat4 RotMatZ = glm::mat4(1.0f);
    glm::mat4 RotMatX = glm::mat4(1.0f);

    float ComponentX, ComponentZ, Hypo;
    float CosA, SinA;



    exp = (pow(4, RAND) - 1) / 3;
    y = (-0.05 + .1 * RAND);
    x = -0.05 + 0.1 * RAND;
    particleAttributes& particle = p_attributes[index];
    _z = -0.1 - 0.4 * exp;
    z = ((_z + 0.3) + 0.2) / 0.4 + 0.5;

    pos = glm::vec3(x, z * y, _z);
    if (TowerPos.z >= EmitterPos.z) {
        RotMatZ = glm::rotate(glm::mat4(), (float)3.14, glm::vec3(0.0, 1.0, 0.0));
    }


    if (TowerPos.x >= EmitterPos.x) {
        RotMatX = glm::rotate(glm::mat4(), -(float)3.14 / 2, glm::vec3(0.0, 1.0, 0.0));
    }
    else {
        RotMatX = glm::rotate(glm::mat4(), (float)3.14 / 2, glm::vec3(0.0, 1.0, 0.0));
    }


    posx = glm::vec3(RotMatX * glm::vec4(pos, 1));
    posz = glm::vec3(RotMatZ * glm::vec4(pos, 1));



    ComponentX = abs(TowerPos.x - EmitterPos.x);
    ComponentZ = abs(TowerPos.z - EmitterPos.z);
    Hypo = sqrt(pow(ComponentX, 2) + pow(ComponentZ, 2));
    CosA = ComponentX / Hypo;
    SinA = ComponentZ / Hypo;
    particle.position = EmitterPos + (posx * CosA + posz * SinA);

    particle.LifeRemaining = z - 0.5;
    particle.velocity = glm::vec3(0, 0, 1);

    particle.life = 1.0f;
    particle.VelFactor = VelFactor;
    particle.Rotation = RAND * 2.0f * glm::pi<float>();
    particle.ColorBegin = ColorBegin;
    particle.ColorEnd = ColorEnd;
    particle.SizeBegin = 0.02;
}


void TailEmitter::updateParticles(int new_particles, float dt, glm::vec3 dir, glm::vec3 vel) {

    for (unsigned int i = 0; i < new_particles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        createNewParticle(unusedParticle);
    }

    for (int i = 0; i < number_of_particles; i++) {
        particleAttributes& particle = p_attributes[i];
        if (particle.life > 0.0f)
        {	

            particle.LifeRemaining -= particle.DeathRate * dt;

            particle.Rotation += 3.14 / 8.0f * dt;
            particle.position += vel * particle.VelFactor * dt;

        }


    }
    EmitterPos = dir;


}


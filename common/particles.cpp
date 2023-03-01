
#include "particles.h"
#include "iostream"
#include <algorithm>





ParticleEmitter::ParticleEmitter(Drawable* _model, int number, glm::vec3 Pos) {
    model = _model;
    number_of_particles = number;
    EmitterPos = Pos;
    p_attributes.resize(number_of_particles, particleAttributes());
    Colors.resize(number_of_particles, glm::vec4());
    Positions.resize(number_of_particles, glm::mat4());
}


/*
IntParticleEmitter::IntParticleEmitter(Drawable* _model, int number, glm::vec3 Pos,int mode,glm::vec3 dir)
{
    model = _model;
    number_of_particles = number;
    EmitterPos = Pos;
    TowerPos = Pos;
    p_attributes.resize(number_of_particles, particleAttributes());
    Colors.resize(number_of_particles, glm::vec4());
    Positions.resize(number_of_particles, glm::mat4());

    if(mode==0){
        for (int i = 0; i < number_of_particles; i++) {
            createNewParticle(i);

        }
    }
    else if (mode == 1) {
        for (int i = 0; i < number_of_particles; i++) {
            createNewParticleOrbit(i,dir);

        }
    }
    else {
        for (int i = 0; i < number_of_particles; i++) {
            createNewParticleShpere(i);

        }
    }


}
*/



void ParticleEmitter::bindAndUpdateBuffers(glm::mat4 PV, GLuint VPloc)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int i = 0; i < number_of_particles; i++) {
        particleAttributes& particle = p_attributes[i];
        glm::vec4& clr = Colors[i];
        glm::mat4& pos = Positions[i];


        float life = particle.LifeRemaining / particle.life;
        clr = LinearInter(particle.ColorBegin, particle.ColorEnd, life);
        clr.a = 0.5f+ clr.a *life;

        float size = LinearInter(particle.SizeBegin, particle.SizeEnd, life);

        pos = glm::translate(glm::mat4(), particle.position)
            * glm::rotate(glm::mat4(), particle.Rotation, { 0.0, 0.0, 1.0})
            * glm::scale(glm::mat4(), { size,  size,  size });
    }


    this->model->bind();
    unsigned int instanceVBO;
    unsigned int modelVBO;

    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * number_of_particles, &Colors[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribDivisor(3, 1);


    glGenBuffers(1, &modelVBO);
    glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * number_of_particles, &Positions[0], GL_STREAM_DRAW);
    std::size_t vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    //This tells opengl how each particle should get data its slice of data from the mat4
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glVertexAttribDivisor(7, 1);






    glUniformMatrix4fv(VPloc, 1, GL_FALSE, &PV[0][0]);
    //glUniformMatrix4fv(Ploc, 1, GL_FALSE, &proj[0][0]);
    //glUniformMatrix4fv(Mloc, 1, GL_FALSE, &model[0][0]);


    this->model->drawInstanced(100);

}



unsigned int ParticleEmitter::firstUnusedParticle()
{
    // first search from last used particle, this will usually return almost instantly
    for (unsigned int i = lastUsedParticle; i < number_of_particles; ++i) {
        if (this->p_attributes[i].LifeRemaining <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // otherwise, do a linear search
    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if (this->p_attributes[i].LifeRemaining <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}
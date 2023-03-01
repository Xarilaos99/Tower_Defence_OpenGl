
#ifndef PARTICLES_H
#define PARTICLES_H
    #pragma once
    #include <GL/glew.h>
    #include <vector>
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include "model.h"

    #include <glm/gtx/string_cast.hpp>


#define RAND ((float) rand()) / (float) RAND_MAX





//std::cout << random_value << std::endl;

struct particleAttributes {
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 velocity;
    float life = 0.0f;
    float Rotation;
    //blue-green for robot
    glm::vec4 ColorBegin={ 0 / 255.0f, 224 / 255.0f, 254 / 255.0f, 1.0f }, ColorEnd= { 15 / 255.0f, 254 / 255.0f, 0 / 255.0f, 1.0f };
    float SizeBegin , SizeEnd = 0.0f;
    float LifeRemaining = 1.0f;
    float radius=0.5f;
    float VelFactor = 1.35;
    float DeathRate = 0.7;
    


};


class ParticleEmitter
{
public:
    
    int number_of_particles;

    //vectors for shaders
    std::vector<particleAttributes> p_attributes;
    std::vector < glm::vec4 > Colors;
    std::vector < glm::mat4 > Positions;


   
    float DeathRate ;
 

    glm::vec3 EmitterPos; //the origin of the emitter
     





    //IntParticleEmitter(Drawable* _model, int number, glm::vec3 Pos,int mode=0,glm::vec3 dir=glm::vec3(0,0,0));
    ParticleEmitter(Drawable* _model, int number, glm::vec3 Pos);
    ~ParticleEmitter() {};
    //void changeParticleNumber(int new_number);

    virtual void updateParticles(int new_particles, float dt, glm::vec3 dir, glm::vec3 vel) = 0;
    virtual void createNewParticle(int index, glm::vec3 dir = glm::vec3(1, 1, 1)) = 0;

    void bindAndUpdateBuffers(glm::mat4 PV, GLuint VPloc);
    
    /*
    void createNewParticle(int index);
    void updateParticles(int new_particles, float dt, glm::vec3 dir,glm::vec3 vel);


    void updateParticlesOrbit(int new_particles, float dt, glm::vec3 dir);
    void updateParticlesShpere(int new_particles, float dt);
    
    void createNewParticleOrbit(int index,glm::vec3 dir= glm::vec3(1,1,1));
    void createNewParticleShpere(int index);
    */




protected:
    float CalcMag(glm::vec3 myvec) {
        return sqrt(pow(myvec.x, 2) + pow(myvec.y, 2) + pow(myvec.z, 2));
    }

    glm::vec4 LinearInter(glm::vec4 a, glm::vec4 b, float c) {
        return a + c * (b - a);
    }

    float LinearInter(float a, float b, float c) {
        return a + c * (b - a);
    }
    unsigned int firstUnusedParticle();


private:

    int lastUsedParticle = 0;
    Drawable* model;
    


    

};

#endif


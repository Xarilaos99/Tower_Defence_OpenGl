#ifndef SphericalEMITTER_H
#define SphericalEMITTER_H
	#pragma once
	#include "particles.h"

	class SphericalEmitter :public ParticleEmitter {
	public:
		SphericalEmitter(Drawable* _model, int number, glm::vec3 Pos,glm::vec4 BegCol, glm::vec4 EndCol, float RadiusMax);
		void updateParticles(int new_particles, float dt, glm::vec3 dir = glm::vec3(1, 1, 1), glm::vec3 vel = glm::vec3(1, 1, 1)) override;
		void createNewParticle(int index, glm::vec3 dir = glm::vec3(1, 1, 1)) override;


	private:
		
		float RadiusMax;
		glm::vec4 ColorBegin , ColorEnd ;
		
	};






#endif 
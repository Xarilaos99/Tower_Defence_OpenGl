#ifndef POLAREMITTER_H
#define POLAREMITTER_H
	#pragma once
	#include "particles.h"

	class PolarEmitter :public ParticleEmitter {
	public:
		PolarEmitter(Drawable* _model, int number, glm::vec3 Pos, glm::vec3 Dir);
		PolarEmitter(Drawable* _model, int number, glm::vec3 Pos, glm::vec3 Dir, glm::vec4 ClrBeg, glm::vec4 ClrEnd);
		void updateParticles(int new_particles, float dt, glm::vec3 dir = glm::vec3(1, 1, 1), glm::vec3 vel = glm::vec3(1, 1, 1)) override;
		void createNewParticle(int index, glm::vec3 dir = glm::vec3(1, 1, 1)) override;


		
		glm::vec4 ColorBegin = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		glm::vec4 ColorEnd = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	private:
		//glm::vec3 TailDir;




	};







#endif 
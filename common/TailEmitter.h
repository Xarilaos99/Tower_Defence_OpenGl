#ifndef TailEMITTER_H
#define TailEMITTER_H
	#pragma once
	#include "particles.h"



	class TailEmitter :public ParticleEmitter {
		public:
			TailEmitter(Drawable* _model, int number, glm::vec3 Pos, glm::vec3 TowerPos, float VelFactor=1.35f);
			TailEmitter(Drawable* _model, int number, glm::vec3 Pos, glm::vec4 ClrBeg, glm::vec4 ClrEnd, float VelFactor = 1.35f);
			//TailEmitter(Drawable* _model, int number, glm::vec3 Pos, float VelFactor = 1.35f);
			void updateParticles(int new_particles, float dt, glm::vec3 dir, glm::vec3 vel) override;
			void createNewParticle(int index, glm::vec3 dir = glm::vec3(1, 1, 1)) override;
			
			

		
			glm::vec4 ColorBegin = { 242 / 255.0f,0 / 255.0f,243 / 255.0f, 1.0f };
			glm::vec4 ColorEnd = { 124 / 255.0f, 3 / 255.0f, 145 / 255.0f,1.0f };
			
		private:
			glm::vec3 TowerPos;
			float VelFactor = 1.35f;
			
	};



#endif 
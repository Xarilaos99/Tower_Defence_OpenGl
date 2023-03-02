#pragma once
#include "tower.h"
#include "model.h"
#include <iostream>
#include <random>
#include <glm/gtc/matrix_transform.hpp>


using namespace glm;
using namespace std;


float CalcMag(vec3 myvec);


Tower::Tower(){
    CurrentPos=vec3(0,0,0);

}

Tower::Tower(vec3 pos,float size){
    CurrentPos=pos;
    TowerScaleMatrix = glm::scale(glm::mat4(), glm::vec3(size, size, size));
    TowerModel=new Drawable("Assets/tower.obj");
    
	TowerModelMatrix = translate(mat4(), CurrentPos) * TowerScaleMatrix;
	glm::vec3 SfairaPos = vec3(TowerModelMatrix * vec4(vec3(TowerModel->RightVal-2.5, TowerModel->HighVal - 3.0, 0.0), 1));
	

    NormalSfaira = new Sfaira(SfairaPos, 0.05,1.05);
    BigSfaira = new Sfaira(SfairaPos, 0.1, 2.5);


    parts = new Drawable("Assets/star.obj");

    NormalParticles = new TailEmitter(parts, 500, CurrentPos, CurrentPos,.8);
    BigParticles = new TailEmitter(parts, 1000, CurrentPos, DBegClr, DEndClr,1.8);
    
    ColParts = new Drawable("Assets/earth.obj");


    NormalCollusionEmitter = new PolarEmitter(ColParts, 500, SfairaPos, SfairaPos);
    BigCollusionEmitter = new PolarEmitter(ColParts, 500, SfairaPos, SfairaPos, DEndClr, DBegClr);
    CollusionEmitter = NormalCollusionEmitter;



    CurrentSfaira = NormalSfaira;
    TailParticles = NormalParticles;

    BigSfaira->ChangeAttack(50);
    NormalSfaira->ChangeAttack(30);

}



void Tower::Draw(glm::mat4 proj, glm::mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc,float dt,bool DrawParts){
    TowerModel->bind();
	//mat4 ModelMatrix = translate(mat4(), CurrentPos) * ;
    //TowerModelMatrix= translate(mat4(), CurrentPos)* TowerScaleMatrix;
	glUniformMatrix4fv(Vloc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(Ploc, 1, GL_FALSE, &proj[0][0]);
	glUniformMatrix4fv(Mloc, 1, GL_FALSE, &TowerModelMatrix[0][0]);
	TowerModel->draw();

	
    if (CurrentSfaira == NormalSfaira) {
        glUniform1i(IsSfaira, 1);
    }
    else {
        glUniform1i(IsSfaira, 2);
    }
    CurrentSfaira->Draw(proj, view, Vloc, Mloc, Ploc);
    glUniform1i(IsSfaira, 0);


    if ((colussion || AnimationOrbit >= 0) && DrawParts) {
        CollusionEmitter->updateParticles(5, dt, CurrentSfaira->GetVel());
        glUseProgram(particleShaderProgram);
        CollusionEmitter->bindAndUpdateBuffers(proj*view, PVParticleLocation);
        glUseProgram(shaderProgram);
        if (colussion) {
            AnimationOrbit = 30;
        }
        else {
            AnimationOrbit--;
        }
    }

}








bool Tower::RobotCollusion(Robot &TempRobot, float dt) {

    

    //CurrentSfaira = CollusionCntr == 0 ? NormalSfaira : BigSfaira;
    if (CollusionCntr == 4) {
        CurrentSfaira = BigSfaira;
        TailParticles = BigParticles;
        //CollusionEmitter = BigCollusionEmitter;
    }
    else {
        CurrentSfaira = NormalSfaira;
        TailParticles = NormalParticles;
        //CollusionEmitter = NormalCollusionEmitter;


    }


    CurrentSfaira->ChangeVelPos(TempRobot.GetCenter(),dt);
    
    
    TailParticles->updateParticles(10, dt, CurrentSfaira->GetPos(), CurrentSfaira->GetVel());

   

    if (CurrentSfaira->CheckColussion(TempRobot)) {

        
        
        
        
        TempRobot.Attack(CurrentSfaira->GetAttack());
        
        
        

        if (CollusionCntr == 4) {
            CollusionCntr = 0;
            BigCollusionEmitter->~PolarEmitter();
            BigCollusionEmitter = new PolarEmitter(ColParts, 500, CurrentSfaira->GetPos(), CurrentSfaira->GetVel(), DEndClr, DBegClr);
            BigParticles->~TailEmitter();
            BigParticles = new TailEmitter(parts, 1000, CurrentPos,  DEndClr, DBegClr,1.8);
            TailParticles = BigParticles;
            CollusionEmitter = BigCollusionEmitter;

        }
        else {
            CollusionCntr++;
            NormalCollusionEmitter->~PolarEmitter();
            NormalCollusionEmitter = new PolarEmitter(ColParts, 500, CurrentSfaira->GetPos(), CurrentSfaira->GetVel());
            NormalParticles->~TailEmitter();
            NormalParticles = new TailEmitter(parts, 500, CurrentPos, CurrentPos,.8);
            TailParticles = NormalParticles;
            CollusionEmitter = NormalCollusionEmitter;
        }
        
        CurrentSfaira->Starting();
        

        
        colussion = TempRobot.IsDead()? false:true;
        return true;
    }
    colussion = false;
    return false;



}




void Tower::DrawSfairaAABB(glm::mat4 proj, glm::mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc) {
    CurrentSfaira->DrawSfairaAABB(proj, view, Vloc, Mloc, Ploc);
}


void Tower::updateOrbit(int new_particles, float dt) {

    CollusionEmitter->updateParticles(new_particles, dt, CurrentSfaira->GetVel());
}


void Tower::DrawEmitter(glm::mat4 PV, GLuint &VPloc, GLuint particleShaderProgram) {
    glUseProgram(particleShaderProgram);
    TailParticles->bindAndUpdateBuffers(PV, VPloc);

    

}



void Tower::DrawEmitterOrbit(glm::mat4 PV, GLuint& VPloc, GLuint particleShaderProgram) {
    glUseProgram(particleShaderProgram);
    CollusionEmitter->bindAndUpdateBuffers(PV, VPloc);

}






void Tower::PassPrograms(GLuint particleShaderProgram, GLuint PVParticleLocation, GLuint shaderProgram) {
    this->particleShaderProgram= particleShaderProgram;;
    this->PVParticleLocation = PVParticleLocation;;
    this->shaderProgram = shaderProgram;;
}

float CalcMag(vec3 myvec) {
    return sqrt(pow(myvec.x, 2) + pow(myvec.y, 2) + pow(myvec.z, 2));
}





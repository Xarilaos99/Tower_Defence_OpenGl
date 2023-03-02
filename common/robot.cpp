#pragma once
#include "robot.h"
#include "model.h"
#include <iostream>
#include <random>
#include <glm/gtc/matrix_transform.hpp>


using namespace glm;
using namespace std;

Robot::Robot() {
	CurrentPos = vec3(0, 0, 0);
	LastPos=0;
	LenPath = 0;
	
}


Robot::Robot(int lastpos,int PathPieces,float size ) {
	CurrentPos = vec3(lastpos+0.5,0,0);
	//CurrentPos = vec3(4.5,0,3.5);
	LastPos = lastpos;
	LenPath = PathPieces;
    RobotScaleMatrix = glm::scale(glm::mat4(), glm::vec3(size, size, size));
	RobotModel = new Drawable("Assets/robot.obj");

    //cout << RobotModel->HighVal <<"  "<< RobotModel->RightVal << endl;
    HightPoint = RobotModel->HighVal;
    RightPoint = RobotModel->RightVal;
    CreateAABB();

    LifeBar = new Drawable("Assets/health_bar_green.obj");
    DeathParts = new Drawable("Assets/earth.obj");
    health = 400;
   // DeathEmitter = new SphericalEmitter(DeathParts, 200, CenterPoint, DeathBeginCol, DeathEndCol, RadiusMax);
    
   
}

void Robot::AccessBarLoc(GLuint& IsRed, GLuint& IsGreen) {
    IsRedloc = IsRed;
    IsGreenloc=IsGreen;
    
}

bool Robot::IsDead()
{
    return (health <= 0);
}

void Robot::CreateAABB() {

    AABBVerts = {
        //base
        vec3(-RightPoint,0,-RightPoint),//0
        vec3(-RightPoint,0,RightPoint),//1
        vec3(RightPoint,0,-RightPoint),//2

        vec3(-RightPoint,0,RightPoint),
        vec3(RightPoint,0,-RightPoint),
        vec3(RightPoint,0,RightPoint),//3


        //left
        vec3(-RightPoint,0,-RightPoint),//0
        vec3(-RightPoint,0,RightPoint),//1
        vec3(-RightPoint,HightPoint,-RightPoint),//0-panw

        vec3(-RightPoint,0,RightPoint),//1
        vec3(-RightPoint,HightPoint,-RightPoint),//0-panw
        vec3(-RightPoint,HightPoint,RightPoint),//1-panw

        //front
        vec3(RightPoint,0,RightPoint),//3
        vec3(-RightPoint,0,RightPoint),//1
        vec3(RightPoint,HightPoint,RightPoint),//3-panw

        vec3(-RightPoint,0,RightPoint),//1
        vec3(RightPoint,HightPoint,RightPoint),//3-panw
        vec3(-RightPoint,HightPoint,-RightPoint),//0-panw 


        //RightPoint
        vec3(RightPoint,0,RightPoint),//3
        vec3(RightPoint,0,-RightPoint),//2
        vec3(RightPoint,HightPoint,RightPoint),//3-panw

        vec3(RightPoint,0,-RightPoint),//2
        vec3(RightPoint,HightPoint,RightPoint),//3-panw
        vec3(RightPoint,HightPoint,-RightPoint),//2-panw 


        //back
        vec3(-RightPoint,0,-RightPoint),//0
        vec3(RightPoint,0,-RightPoint),//2
        vec3(RightPoint,HightPoint,-RightPoint),//2-panw

        vec3(-RightPoint,0,-RightPoint),//0
        vec3(RightPoint,HightPoint,-RightPoint),//2-panw
        vec3(-RightPoint,HightPoint,-RightPoint),//0-panw


        //panw
         vec3(-RightPoint,HightPoint,-RightPoint),//0-panw
         vec3(-RightPoint,HightPoint,RightPoint),//1-panw
         vec3(RightPoint,HightPoint,RightPoint),//3-panw

         vec3(-RightPoint,HightPoint,-RightPoint),//0-panw
          vec3(RightPoint,HightPoint,RightPoint),//3-panw
          vec3(RightPoint,HightPoint,-RightPoint),//2-panw


    };

    cout << sizeof(AABBVerts)<<"  "<<AABBVerts.size()  << endl;

    MaxAABB = AABBVerts[32];
    MinAABB = AABBVerts[0];
    AABBRobot = new Drawable(AABBVerts);
    
}





void Robot::Draw(mat4 proj, mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc,float dt,bool drawHealth,bool DrawParts) {
   

    if (IsDead() && DrawParts) {

        if (AnimationTime == 90) {
            
            DeathEmitter = new SphericalEmitter(DeathParts, 200, CenterPoint, DeathBeginCol, DeathEndCol, RadiusMax);
            AnimationTime--;
        }
        else if (AnimationTime >= 0) {
            DeathEmitter->updateParticles(0, dt);
            glUseProgram(particleShaderProgram);
            DeathEmitter->bindAndUpdateBuffers(proj * view, PVParticleLocation);
            glUseProgram(shaderProgram);
            AnimationTime--;

        }
        else {
            EraseRobot = true;
        }


    }
    else{
	    RobotModel->bind();
	    //mat4 ModelMatrix = translate(mat4(), CurrentPos) * ;
        RobotModelMatrix = translate(mat4(), CurrentPos) * RobotRotateMatrix * RobotScaleMatrix;
	    glUniformMatrix4fv(Vloc, 1, GL_FALSE, &view[0][0]);
	    glUniformMatrix4fv(Ploc, 1, GL_FALSE, &proj[0][0]);
	    glUniformMatrix4fv(Mloc, 1, GL_FALSE, &RobotModelMatrix[0][0]);
	    RobotModel->draw();



    
        if(drawHealth){
            glUniform1i(IsRedloc, 1);
    
            LifeBar->bind();
            vec3 top = vec3(RobotModelMatrix * vec4(-RightPoint, HightPoint, 0, 1));
            mat4 LifeBarMat = translate(mat4(),top) * RobotRotateMatrix * glm::scale(glm::mat4(), glm::vec3(sizeBar, sizeBar, sizeBar));
            glUniformMatrix4fv(Vloc, 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(Ploc, 1, GL_FALSE, &proj[0][0]);
            glUniformMatrix4fv(Mloc, 1, GL_FALSE, &LifeBarMat[0][0]);

            LifeBar->draw();
    
            glUniform1i(IsRedloc, 0);


    
            glUniform1i(IsGreenloc, 1);
            LifeBar->bind();
            top = vec3(RobotModelMatrix * vec4(-RightPoint-0.05, HightPoint, 0, 1));
            float life = health / 400.0f;
            LifeBarMat = translate(mat4(),top) * RobotRotateMatrix * glm::scale(glm::mat4(), glm::vec3((sizeBar+0.002)* life, sizeBar + 0.002, sizeBar + 0.002));
            glUniformMatrix4fv(Vloc, 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(Ploc, 1, GL_FALSE, &proj[0][0]);
            glUniformMatrix4fv(Mloc, 1, GL_FALSE, &LifeBarMat[0][0]);

            LifeBar->draw();
            glUniform1i(IsGreenloc, 0);
        }


        //CenterPoint = vec3(RobotModelMatrix * vec4(0, HightPoint/2, 0, 1));
    
        
    }

    

}



void Robot::DrawAABB(mat4 proj, mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc) {
    AABBRobot->bind();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    mat4 t  = translate(mat4(), CurrentPos) * RobotRotateMatrix*RobotScaleMatrix;
    glUniformMatrix4fv(Vloc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(Ploc, 1, GL_FALSE, &proj[0][0]);
    glUniformMatrix4fv(Mloc, 1, GL_FALSE, &t[0][0]);
    
    AABBRobot->draw();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}



void Robot::UpdateRobot(float dt,int *IndexPath ) {
    
    //float dt = curTim - Tim;
    dtRobot += dt;
    if (dtRobot>0.17 && indexOfPath < 14) {
        //Tim = curTim;
        rounds++;
        if (rounds == 5) {

            indexOfPath++;
            if (indexOfPath < 14) {
                vec3 temp = Dir;
                if (LastPos > IndexPath[indexOfPath]) {

                    Dir = -PosX;
                }
                else if (LastPos < IndexPath[indexOfPath]) {
                    Dir = PosX;
                }
                else {
                    Dir = PosZ;
                    

                }
                rounds = -3;
                LastPos = IndexPath[indexOfPath];
            }

        }

        CurrentPos += Dir * RobotSpeed;
        
        if (Dir == PosX) {
            RobotRotateMatrix = rotate(mat4(), (float)3.14 / 2, vec3(0.0, 1.0, 0.0));
            MaxAABB = AABBVerts[31];
            MinAABB = AABBVerts[2];
        }
        else if (Dir == -PosX) {
            RobotRotateMatrix = rotate(mat4(), (float)3.14 / 2, vec3(0.0, -1.0, 0.0));
            MaxAABB = AABBVerts[35];
            MinAABB = AABBVerts[1];
        }
        else {
            RobotRotateMatrix = mat4(1);
            MaxAABB = AABBVerts[32];
            MinAABB = AABBVerts[0];
        }
        dtRobot = 0;

    }
    RobotModelMatrix = translate(mat4(), CurrentPos) * RobotRotateMatrix * RobotScaleMatrix;
    CenterPoint = vec3(RobotModelMatrix * vec4(0, HightPoint / 2, 0, 1));


    MinPoint = vec3(RobotModelMatrix * vec4(MinAABB , 1.0));
    MaxPoint = vec3(RobotModelMatrix * vec4(MaxAABB , 1.0));

    if (indexOfPath == 2) {
        Index = 1;
    }
    
}


void Robot::PassPrograms(GLuint ParticleProgram, GLuint RenderrerProgram, GLuint PVloc)
{
    particleShaderProgram = ParticleProgram;
    shaderProgram = RenderrerProgram;
    PVParticleLocation = PVloc;
}


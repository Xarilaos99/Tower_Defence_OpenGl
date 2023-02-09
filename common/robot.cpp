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
	//CurrentPos = vec3(3.0+0.5,0,4.5);
	LastPos = lastpos;
	LenPath = PathPieces;
    RobotScaleMatrix = glm::scale(glm::mat4(), glm::vec3(size, size, size));
	RobotModel = new Drawable("robot.obj");

    //cout << RobotModel->HighVal <<"  "<< RobotModel->RightVal << endl;
    HightPoint = RobotModel->HighVal + 0.1;
    RightPoint = RobotModel->RightVal + 0.1;
    CreateAABB();
   
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
    MaxPoint = AABBVerts[32];
    MinPoint = AABBVerts[0];
    AABBRobot = new Drawable(AABBVerts);
    
}





void Robot::Draw(mat4 proj, mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc) {
	RobotModel->bind();
	//mat4 ModelMatrix = translate(mat4(), CurrentPos) * ;
    RobotModelMatrix= translate(mat4(), CurrentPos)* RobotRotateMatrix * RobotScaleMatrix;
	glUniformMatrix4fv(Vloc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(Ploc, 1, GL_FALSE, &proj[0][0]);
	glUniformMatrix4fv(Mloc, 1, GL_FALSE, &RobotModelMatrix[0][0]);
	RobotModel->draw();

    CenterPoint = vec3(RobotModelMatrix * vec4(0, HightPoint/2, 0, 1));
    


    
    MinPoint = vec3(RobotModelMatrix * vec4(AABBVerts[0], 1.0));
    MaxPoint = vec3(RobotModelMatrix * vec4(AABBVerts[32], 1.0));


    

}



void Robot::DrawAABB(mat4 proj, mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc) {
    AABBRobot->bind();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    RobotModelMatrix = translate(mat4(), CurrentPos) * RobotScaleMatrix;
    glUniformMatrix4fv(Vloc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(Ploc, 1, GL_FALSE, &proj[0][0]);
    glUniformMatrix4fv(Mloc, 1, GL_FALSE, &RobotModelMatrix[0][0]);
    
    AABBRobot->draw();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}



void Robot::UpdateRobot(float curTim,float &Tim,int *IndexPath ) {
    
    float dt = curTim - Tim;
    if (dt > 0.1 && indexOfPath < 14) {
        Tim = curTim;
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
        }
        else if (Dir == -PosX) {
            RobotRotateMatrix = rotate(mat4(), (float)3.14 / 2, vec3(0.0, -1.0, 0.0));
        }
        else {
            RobotRotateMatrix = mat4(1);
        }

    }
}

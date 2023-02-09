#pragma once
#include "crystal.h"
#include "model.h"
#include <iostream>
#include <random>
#include <glm/gtc/matrix_transform.hpp>


using namespace glm;
using namespace std;


Crystal::Crystal(){
    CurrentPos=vec3(0,0,0);
    health = 1000;

}

Crystal::Crystal(vec3 pos,float size){
    health = 1000;
    CurrentPos=pos;
    CrystalScaleMatrix = glm::scale(glm::mat4(), glm::vec3(size, size, size));
    CrystalModel=new Drawable("crystal.obj");

    HightPoint = CrystalModel->HighVal;
    RightPoint = CrystalModel->RightVal;

    CreateAABB();

}

void Crystal::Draw(glm::mat4 proj, glm::mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc){
    CrystalModel->bind();
	//mat4 ModelMatrix = translate(mat4(), CurrentPos) * ;
    CrystalModelMatrix= translate(mat4(), CurrentPos)* CrystalScaleMatrix;
	glUniformMatrix4fv(Vloc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(Ploc, 1, GL_FALSE, &proj[0][0]);
	glUniformMatrix4fv(Mloc, 1, GL_FALSE, &CrystalModelMatrix[0][0]);
	CrystalModel->draw();


    CrystalMinPoint = vec3(CrystalModelMatrix * vec4(AABBVerts[0], 1.0));
    CrystalMaxPoint = vec3(CrystalModelMatrix * vec4(AABBVerts[32], 1.0));
}



void Crystal::CreateAABB() {
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

    CrystalMaxPoint = AABBVerts[32];
    CrystalMinPoint = AABBVerts[0];
    AABBCrystal = new Drawable(AABBVerts);
}


void Crystal::DrawAABB(mat4 proj, mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc) {
    AABBCrystal->bind();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    CrystalModelMatrix = translate(mat4(), CurrentPos) * CrystalScaleMatrix;
    glUniformMatrix4fv(Vloc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(Ploc, 1, GL_FALSE, &proj[0][0]);
    glUniformMatrix4fv(Mloc, 1, GL_FALSE, &CrystalModelMatrix[0][0]);

    AABBCrystal->draw();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}


void Crystal::RobotCollusion(Robot& TempRobot) {


   

    if (CheckColussion(TempRobot) > 0) {
        cout << "tempCrystal" << endl;
        Attack(10);
        //SfairaPos = SfairaStartingPos;
        cout << health<<"\n" << endl;
    }
    //cout << CheckColussion(TempRobot)<<endl;
    //cout << SfairaPos.x << "  " << SfairaPos.y << "  " << SfairaPos.z << "  " << endl;

}



float Crystal::CheckColussion(Robot& TempRobot) {
    //cout<<
    float xOverlap = std::max(0.0f, std::min(TempRobot.GetMax().x, CrystalMaxPoint.x) - std::max(TempRobot.GetMin().x, CrystalMinPoint.x));
    float yOverlap = std::max(0.0f, std::min(TempRobot.GetMax().y, CrystalMaxPoint.y) - std::max(TempRobot.GetMin().y, CrystalMinPoint.y));
    float zOverlap = std::max(0.0f, std::min(TempRobot.GetMax().z, CrystalMaxPoint.z) - std::max(TempRobot.GetMin().z, CrystalMinPoint.z));
    //cout << xOverlap << "   " << yOverlap << "     " << zOverlap << "\n" << endl;

    return xOverlap * yOverlap * zOverlap;


}

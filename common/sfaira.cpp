#pragma once
#include "sfaira.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

Sfaira::Sfaira(glm::vec3 Pos,float size,float VelFactor){
    sfaira=new Drawable("earth.obj");


    HightPoint = sfaira->HighVal;
    RightPoint = sfaira->RightVal;
    SfairaPos=Pos;
    SfairaStartingPos=Pos;
    this->size=size;
    this->VelFactor=VelFactor;
    CreateSfairaAABB();
}

void Sfaira::CreateSfairaAABB(){
    AABBVerts = {
        //base
        vec3(-RightPoint,-HightPoint,-RightPoint),//0
        vec3(-RightPoint,-HightPoint,RightPoint),//1
        vec3(RightPoint,-HightPoint,-RightPoint),//2

        vec3(-RightPoint,-HightPoint,RightPoint),
        vec3(RightPoint,-HightPoint,-RightPoint),
        vec3(RightPoint,-HightPoint,RightPoint),//3


        //left
        vec3(-RightPoint,-HightPoint,-RightPoint),//0
        vec3(-RightPoint,-HightPoint,RightPoint),//1
        vec3(-RightPoint,HightPoint,-RightPoint),//0-panw

        vec3(-RightPoint,-HightPoint,RightPoint),//1
        vec3(-RightPoint,HightPoint,-RightPoint),//0-panw
        vec3(-RightPoint,HightPoint,RightPoint),//1-panw

        //front
        vec3(RightPoint,-HightPoint,RightPoint),//3
        vec3(-RightPoint,-HightPoint,RightPoint),//1
        vec3(RightPoint,HightPoint,RightPoint),//3-panw

        vec3(-RightPoint,-HightPoint,RightPoint),//1
        vec3(RightPoint,HightPoint,RightPoint),//3-panw
        vec3(-RightPoint,HightPoint,-RightPoint),//0-panw 


        //RightPoint
        vec3(RightPoint,-HightPoint,RightPoint),//3
        vec3(RightPoint,-HightPoint,-RightPoint),//2
        vec3(RightPoint,HightPoint,RightPoint),//3-panw

        vec3(RightPoint,-HightPoint,-RightPoint),//2
        vec3(RightPoint,HightPoint,RightPoint),//3-panw
        vec3(RightPoint,HightPoint,-RightPoint),//2-panw 


        //back
        vec3(-RightPoint,-HightPoint,-RightPoint),//0
        vec3(RightPoint,-HightPoint,-RightPoint),//2
        vec3(RightPoint,HightPoint,-RightPoint),//2-panw

        vec3(-RightPoint,-HightPoint,-RightPoint),//0
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

    sfairaAABB = new Drawable(AABBVerts);
}

float Sfaira::CheckColussion(Robot& TempRobot)
{
    float xOverlap = std::max(0.0f, std::min(TempRobot.GetMax().x, SfairaMaxPoint.x) - std::max(TempRobot.GetMin().x, SfairaMinPoint.x));
    float yOverlap = std::max(0.0f, std::min(TempRobot.GetMax().y, SfairaMaxPoint.y) - std::max(TempRobot.GetMin().y, SfairaMinPoint.y));
    float zOverlap = std::max(0.0f, std::min(TempRobot.GetMax().z, SfairaMaxPoint.z) - std::max(TempRobot.GetMin().z, SfairaMinPoint.z));
    /*cout << xOverlap << "   " << yOverlap << "     " << zOverlap << "\n" << endl;
    cout << TempRobot.GetMax().x << "   " << TempRobot.GetMax().y << "     " << TempRobot.GetMax().z << "\n" << endl;
    cout << SfairaMaxPoint.x<< "   " << SfairaMaxPoint.y<< "     " << SfairaMaxPoint.z<< "\n" << endl;
    cout << TempRobot.GetMin().x << "   " << TempRobot.GetMin().y << "     " << TempRobot.GetMin().z << "\n" << endl;
    cout << SfairaMinPoint.x<< "   " << SfairaMinPoint.y<< "     " << SfairaMinPoint.z<< "\n" << endl;*/

    return xOverlap * yOverlap * zOverlap;
}

void Sfaira::DrawSfairaAABB(mat4 proj, mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc) {
    sfairaAABB->bind();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    mat4 SfairaModelMatrix = translate(mat4(), SfairaPos) * scale(mat4(), glm::vec3(size, size, size));;
    glUniformMatrix4fv(Vloc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(Ploc, 1, GL_FALSE, &proj[0][0]);
    glUniformMatrix4fv(Mloc, 1, GL_FALSE, &SfairaModelMatrix[0][0]);

    sfairaAABB->draw();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void Sfaira::Draw(glm::mat4 proj, glm::mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc) {
    sfaira->bind();

    mat4 SfairaModelMatrix = translate(mat4(), SfairaPos) * scale(mat4(), glm::vec3(size, size, size));
    //mat4 SfairaModelMatrix = translate(mat4(), vec3(1.0,HightPoint+1.0,1.0)) * scale(mat4(), glm::vec3(0.35, 0.35, 0.35));
    glUniformMatrix4fv(Vloc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(Ploc, 1, GL_FALSE, &proj[0][0]);
    glUniformMatrix4fv(Mloc, 1, GL_FALSE, &SfairaModelMatrix[0][0]);
    sfaira->draw();


    SfairaMinPoint = vec3(SfairaModelMatrix * vec4(AABBVerts[0], 1.0));
    SfairaMaxPoint = vec3(SfairaModelMatrix * vec4(AABBVerts[32], 1.0));
}






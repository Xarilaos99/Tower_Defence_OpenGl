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
    TowerModel=new Drawable("tower.obj");
	sfaira=new Drawable("earth.obj");


    HightPoint = sfaira->HighVal;
    RightPoint = sfaira->RightVal;
    
	cout << HightPoint << endl;
	TowerModelMatrix = translate(mat4(), CurrentPos) * TowerScaleMatrix;
	SfairaPos = vec3(TowerModelMatrix * vec4(vec3(-TowerModel->RightVal, TowerModel->HighVal - 3.0, 0.0), 1));
	SfairaStartingPos = vec3(TowerModelMatrix * vec4(vec3(-TowerModel->RightVal, TowerModel->HighVal - 3.0, 0.0), 1));
    CreateSfairaAABB();



}



void Tower::Draw(glm::mat4 proj, glm::mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc){
    TowerModel->bind();
	//mat4 ModelMatrix = translate(mat4(), CurrentPos) * ;
    //TowerModelMatrix= translate(mat4(), CurrentPos)* TowerScaleMatrix;
	glUniformMatrix4fv(Vloc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(Ploc, 1, GL_FALSE, &proj[0][0]);
	glUniformMatrix4fv(Mloc, 1, GL_FALSE, &TowerModelMatrix[0][0]);
	TowerModel->draw();

	//SfairaPos = vec3(TowerModelMatrix* vec4(vec3(-RightPoint, HightPoint-3.0, 0.0), 1)) ;
	//cout << t.x << "  " << t.y << "  " << t.z << "  " << t.w << "  " << endl;

	//vec3 hightP = vec3(t.x / t.w, t.y / t.w, t.z / t.w);
	 
    //cout << SfairaPos.x << "  " << SfairaPos.y << "  " << SfairaPos.z << "  " << endl;
	
    sfaira->bind();

	mat4 SfairaModelMatrix = translate(mat4(), SfairaPos) * scale(mat4(), glm::vec3(0.05, 0.05, 0.05));
	//mat4 SfairaModelMatrix = translate(mat4(), vec3(1.0,HightPoint+1.0,1.0)) * scale(mat4(), glm::vec3(0.35, 0.35, 0.35));
	glUniformMatrix4fv(Vloc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(Ploc, 1, GL_FALSE, &proj[0][0]);
	glUniformMatrix4fv(Mloc, 1, GL_FALSE, &SfairaModelMatrix[0][0]);
	sfaira->draw();


    /*vec3 sfairavec = vec3(sfaira->RightVal, sfaira->HighVal, sfaira->RightVal);
    sfairavec = vec3(SfairaModelMatrix * vec4(sfairavec, 1.0));
    this->RightPoint = sfairavec.x;
    this->HightPoint = sfairavec.y;*/


    SfairaMinPoint = vec3(SfairaModelMatrix * vec4(AABBVerts[0], 1.0));
    SfairaMaxPoint = vec3(SfairaModelMatrix * vec4(AABBVerts[32], 1.0));
   
    









}






void Tower::CalDis(vec3 RobotPos) {
	vector<vec3> t = {
		RobotPos,SfairaPos,vec3(0,0,0)
	};
	
	tempPlane = new Drawable(t);

}



void Tower::CreateSfairaAABB() {
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



void Tower::DrawSfairaAABB(mat4 proj, mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc) {
    sfairaAABB->bind();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    mat4 SfairaModelMatrix = translate(mat4(), SfairaPos) * scale(mat4(), glm::vec3(0.05, 0.05, 0.05));;
    glUniformMatrix4fv(Vloc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(Ploc, 1, GL_FALSE, &proj[0][0]);
    glUniformMatrix4fv(Mloc, 1, GL_FALSE, &SfairaModelMatrix[0][0]);

    sfairaAABB->draw();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Tower::RobotCollusion(Robot &TempRobot) {

    vec3 Dir = TempRobot.GetCenter() - SfairaPos;
    Dir /= CalcMag(Dir);

    SfairaPos += float(0.01) * Dir;

    if (CheckColussion(TempRobot) > 0) {
        cout << "temp" << endl;
        TempRobot.Attack(10);
        SfairaPos = SfairaStartingPos;
    }
    //cout << CheckColussion(TempRobot)<<endl;
    //cout << SfairaPos.x << "  " << SfairaPos.y << "  " << SfairaPos.z << "  " << endl;

}



float Tower::CheckColussion(Robot& TempRobot) {
    //cout<<
    float xOverlap = std::max(0.0f, std::min(TempRobot.GetMax().x,SfairaMaxPoint.x) - std::max(TempRobot.GetMin().x, SfairaMinPoint.x));
    float yOverlap = std::max(0.0f, std::min(TempRobot.GetMax().y,SfairaMaxPoint.y) - std::max(TempRobot.GetMin().y, SfairaMinPoint.y));
    float zOverlap = std::max(0.0f, std::min(TempRobot.GetMax().z,SfairaMaxPoint.z) - std::max(TempRobot.GetMin().z, SfairaMinPoint.z));
    //cout << xOverlap << "   " << yOverlap << "     " << zOverlap << "\n" << endl;
    
    return xOverlap * yOverlap * zOverlap;
    

}


float CalcMag(vec3 myvec) {
	return sqrt(pow(myvec.x, 2) + pow(myvec.y, 2) + pow(myvec.z, 2));
}





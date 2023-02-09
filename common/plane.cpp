#pragma once
#include "plane.h"
#include "model.h"
#include <iostream>
#include <random>


using namespace glm;
using namespace std;

Plane::Plane() {
    position=vec3(0.0,0.0,0.0);
	

}

Plane::Plane(vec3 pos){
    position=pos;
	
}



void Plane::CreatePlane(){
    vector<vec3> planeVertices = {
        position,
        position+positionZ,
        position+positionDiag,
        position+positionDiag,
        position+positionX,
        position,

	};

	// plane normals
	vector<vec3> planeNormals = {
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f)
	};

	// plane uvs
	vector<vec2> planeUVs = {
		vec2(0.0f, 0.0f),
		vec2(0.0f, 1.0f),
		vec2(1.0f, 1.0f),
		vec2(1.0f, 1.0f),
		vec2(1.0f, 0.0f),
		vec2(0.0f, 0.0f),
	};
    plane=new Drawable(planeVertices,planeUVs,planeNormals);
	
 
	//create path map

	
	
	CreateTerrainWithIndex();



}

void Plane::CreateTerrainWithIndex() {
	int len = sizeof(indexRowMatrix) / sizeof(int);
	
	for (int i = 0; i < len; i++) {
		texSel[10 * indexRowMatrix[i] + indexColumnMatrix[i]] = 1;
	}

	

}

void Plane::CreateTerrain() {

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, 9);
	uniform_int_distribution<> dis1(0, 2);

	int randomNumber = dis(gen);
	int randomPos = 0;


	for (float i = 0.0; i < 10; i++) {
		for (float j = 0.0; j < 10; j++) {
			//cout << i << "  " << j << endl;

			if (randomPos == 0) {
				if ((int)j == randomNumber) {
					texSel[(int)i * 10 + (int)j] = 1;
				}
				else {
					texSel[(int)i * 10 + (int)j] = 0;
				}

			}
			else {
				if (randomPos == 1) {
					if (randomNumber == 0) {
						if (j == 0) {
							texSel[(int)i * 10] = 1;
						}
						else texSel[(int)i * 10 + (int)j] = 0;

					}
					else {
						if ((int)j == randomNumber) {
							texSel[(int)i * 10 + (int)j - 1] = 1;
							texSel[(int)i * 10 + (int)j] = 1;
							randomNumber--;
						}
						else {
							texSel[(int)i * 10 + (int)j] = 0;
						}

					}
				}
				else {
					if (randomNumber == 9) {
						if (j == 9) {
							texSel[(int)i * 10 + (int)j] = 1;
						}
						else texSel[(int)i * 10 + (int)j] = 0;

					}
					else {
						if ((int)j == randomNumber) {
							texSel[(int)i * 10 + (int)j + 1] = 1;
							texSel[(int)i * 10 + (int)j] = 1;
							randomNumber++;
							randomPos = 0;
						}
						else {
							texSel[(int)i * 10 + (int)j] = 0;
						}

					}

				}
			}


		}
		randomPos = dis1(gen);

	}
}

void Plane::Draw(mat4 model,mat4 proj,mat4 view,GLuint VPloc,GLuint Mloc){
	plane->bind();
	mat4 terrainModelMatrix = model;
	mat4 terrainVP = proj * view;
	glUniformMatrix4fv(VPloc, 1, GL_FALSE, &terrainVP[0][0]);
	glUniformMatrix4fv(Mloc, 1, GL_FALSE, &terrainModelMatrix[0][0]);
	plane->draw();
}

void Plane::DrawInstanced(mat4 model, mat4 proj, mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc) {

	plane->bind();

	//create random number
	


	//create arrays for instancing
	glm::vec3 offset[100];
	
	for (float i = 0.0; i < 10; i++) {
		for (float j = 0.0; j < 10; j++) {
			//cout << i << "  " << j << endl;
			offset[(int)i * 10 + (int)j] = positionX * j + positionZ * i;

		}
	}

	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 100, &offset[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(3, 1);
	
	unsigned int texVBO;
	glGenBuffers(1, &texVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * 100, &this->texSel[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(4, 1);

	//cout << planeTex.GetId() << endl;
	//cout << planeTex[0]->GetSam() << "  " << planeTex[1]->GetSam() << endl;

	//const GLint samplerIDs[2] = { planeTex[0]->GetId() ,planeTex[1]->GetId() };
	
	
	glUniformMatrix4fv(Vloc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(Ploc, 1, GL_FALSE, &proj[0][0]);
	glUniformMatrix4fv(Mloc, 1, GL_FALSE, &model[0][0]);


	plane->drawInstanced(100);
}
#ifndef TOWER_H
#define TOWER_H
    #pragma once
    #include <glm/glm.hpp>
    #include <vector>
    #include <GL/glew.h>
    #include "model.h"
    #include "robot.h"


    class Tower{
        public:
            Tower();
            Tower(glm::vec3 pos,float size);
            ~Tower() = default;
            //void CreateTower();
            void Draw(glm::mat4 proj, glm::mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc);
            void CalDis(glm::vec3 RobotPos);
            void DrawSfairaAABB(glm::mat4 proj, glm::mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc);
            void RobotCollusion(Robot &TempRobot);
            
            //void UpdateTower(float curTim, float& Tim, int* IndexPath);

        private:
            glm::mat4  TowerModelMatrix;
            glm::mat4  TowerScaleMatrix;
            glm::vec3 CurrentPos;
            Drawable* TowerModel;
            //Edw prepei na ftiaxw kai mia class gia ti sfaira

            Drawable* sfaira;
            float HightPoint = 0;
            float RightPoint = 0;

            glm::vec3 SfairaMaxPoint = glm::vec3(0, 0, 0);
            glm::vec3 SfairaMinPoint = glm::vec3(0, 0, 0);
            
            
            std::vector<glm::vec3> AABBVerts ;



            glm::vec3 SfairaPos=glm::vec3(0,0,0);
            glm::vec3 	SfairaStartingPos =glm::vec3(0,0,0);
            void CreateSfairaAABB();
            float CheckColussion(Robot& TempRobot);
            Drawable* sfairaAABB;
            Drawable* tempPlane;




    };



#endif
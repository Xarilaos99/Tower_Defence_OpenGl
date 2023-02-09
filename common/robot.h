#ifndef ROBOT_H
#define ROBOT_H
    #pragma once
    #include <glm/glm.hpp>
    #include <vector>

    #include <GL/glew.h>
    #include "model.h"


    class Robot{
        public:
            Robot();
            Robot(int lastpos, int PathPieces,float size);
            ~Robot() = default;
            //void CreateRobot();
            void Draw(glm::mat4 proj, glm::mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc);
            void DrawAABB(glm::mat4 proj, glm::mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc);
            void UpdateRobot(float curTim, float& Tim, int* IndexPath);
            glm::vec3 GetCenter() { return CenterPoint; }
            float GetHight() { return HightPoint; }
            float GetRight() { return RightPoint; }
            glm::vec3 GetMax() { return MaxPoint; }
            glm::vec3 GetMin() { return MinPoint; }
            int GetHealth() { return health; }
            void Attack(int Dam) {
                health -= Dam;
            }
            
        private:
            glm::mat4  RobotModelMatrix;
            glm::mat4  RobotScaleMatrix;
            glm::mat4  RobotRotateMatrix;
            int health=100;
            float RobotSpeed=0.125;
            glm::vec3 PosX = glm::vec3(1, 0, 0);
            glm::vec3 PosZ = glm::vec3(0, 0, 1);
            float LastPos;
            int LenPath;
            glm::vec3 CurrentPos;
            Drawable* RobotModel;
            Drawable* AABBRobot;


            //==============AABB VALS=============================
            std::vector<glm::vec3> AABBVerts;
            float HightPoint=0;
            float RightPoint=0;
            glm::vec3 MaxPoint = glm::vec3(0, 0, 0);
            glm::vec3 MinPoint = glm::vec3(0, 0, 0);
            void CreateAABB();
            glm::vec3 CenterPoint;

            //VARS FOR UPDATE======================
            int rounds=0;
            int indexOfPath=0;
            glm::vec3 Dir = PosZ;



    };



#endif
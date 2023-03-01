#ifndef ROBOT_H
#define ROBOT_H
    #pragma once
    #include <glm/glm.hpp>
    #include <vector>

    #include <GL/glew.h>
    #include "model.h"
    #include "SphericalEmitter.h"


    class Robot{
        public:
            Robot();
            Robot(int lastpos, int PathPieces,float size);
            ~Robot() = default;
            //void CreateRobot();
            void Draw(glm::mat4 proj, glm::mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc,float dt, bool drawHealth,bool DrawParts=true);
            void DrawAABB(glm::mat4 proj, glm::mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc);
            void UpdateRobot(float dt, int* IndexPath);
            glm::vec3 GetCenter() { return CenterPoint; }
            glm::vec3 GetPos() { return CurrentPos; }
            float GetHight() { return HightPoint; }
            float GetRight() { return RightPoint; }
            glm::vec3 GetMax() { return MaxPoint; }
            glm::vec3 GetMin() { return MinPoint; }
            int GetHealth() { return health; }
            void Attack(int Dam) {
                health -= Dam;
            }
            void AccessBarLoc(GLuint& IsRed, GLuint& IsGreen);
            bool IsDead();
            void PassPrograms(GLuint ParticleProgram, GLuint RenderrerProgram, GLuint PVloc);
            int GetIndex() {
                return Index;
            }
            bool ChechErase() {
                return EraseRobot;
            }
            
        private:
            glm::mat4  RobotModelMatrix;
            glm::mat4  RobotScaleMatrix;
            glm::mat4  RobotRotateMatrix;
            int health=70;
            float RobotSpeed=0.125;
            glm::vec3 PosX = glm::vec3(1, 0, 0);
            glm::vec3 PosZ = glm::vec3(0, 0, 1);
            float LastPos;
            int LenPath;
            glm::vec3 CurrentPos;
            Drawable* RobotModel;
            Drawable* AABBRobot;
            float dtRobot=0.0f;


            //==============AABB VALS=============================
            std::vector<glm::vec3> AABBVerts;
            float HightPoint=0;
            float RightPoint=0;
            glm::vec3 MaxPoint = glm::vec3(0, 0, 0);
            glm::vec3 MinPoint = glm::vec3(0, 0, 0);
            glm::vec3 MaxAABB, MinAABB;
            void CreateAABB();
            glm::vec3 CenterPoint;

            //VARS FOR UPDATE======================
            int rounds=0;
            int indexOfPath=0;
            int Index=0;
            glm::vec3 Dir = PosZ;


            //life Bar======================
            Drawable* LifeBar;
            float sizeBar = 0.052;
            float XFactor = 0.1f;
            GLuint IsRedloc, IsGreenloc;


            //===============Particles====================================
            SphericalEmitter* DeathEmitter;
            glm::vec4 DeathBeginCol = { 0 / 255.0f, 224 / 255.0f, 254 / 255.0f, 1.0f };
            glm::vec4 DeathEndCol = { 15 / 255.0f, 254 / 255.0f, 0 / 255.0f, 1.0f };
            float RadiusMax = 0.5;
            Drawable* DeathParts;
            GLuint particleShaderProgram, shaderProgram, PVParticleLocation;
            int AnimationTime = 90;
            

            //Erase ============================================
            bool EraseRobot = false;



    };



#endif
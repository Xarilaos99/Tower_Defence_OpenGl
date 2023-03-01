#ifndef CRYSTAL_H
#define CRYSTAL_H
    #pragma once
    #include <glm/glm.hpp>
    #include <vector>
    #include <GL/glew.h>
    #include "model.h"
    #include "robot.h"
    #include "SphericalEmitter.h"


    class Crystal{
        public:
            Crystal();
            Crystal(glm::vec3 pos,float size);
            ~Crystal() = default;
            //void CreateCrystal();
            void Draw(glm::mat4 proj, glm::mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc,float dt,bool drawHealth,bool DrawParts=true);
            void DrawAABB(glm::mat4 proj, glm::mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc);
            void RobotCollusion(Robot& TempRobot);
            float GetHealth() { return health; }
            void Attack(int Val) {
                health -= Val;
            }
            glm::vec3 GetCenter() { return CenterPoint; }
            bool IsDead();
            void PassPrograms(GLuint ParticleProgram, GLuint RenderrerProgram, GLuint PVloc);
            void AccessBarLoc(GLuint& IsRed, GLuint& IsGreen);

            bool GetEndGame() {
                return stopGame;
            }
            //void UpdateCrystal(float curTim, float& Tim, int* IndexPath);

        private:
            glm::mat4  CrystalModelMatrix;
            glm::mat4  CrystalScaleMatrix;
            glm::vec3 CurrentPos;
            Drawable* CrystalModel;
            std::vector<glm::vec3> AABBVerts;
            float HightPoint;
            float RightPoint;
            glm::vec3 CenterPoint;
            void CreateAABB();
            glm::vec3 CrystalMaxPoint = glm::vec3(0, 0, 0);
            glm::vec3 CrystalMinPoint = glm::vec3(0, 0, 0);
            Drawable* AABBCrystal;
            float CheckColussion(Robot& TempRobot);

            int health =0;
            //Edw prepei na ftiaxw  otan to health tha einai 0 na exoume explode kai particles

            //===============Particles====================================
            SphericalEmitter* DeathEmitter;
            glm::vec4 DeathBeginCol= { 255 / 255.0f, 29 / 255.0f, 215 / 255.0f, 1.0f } ;
            glm::vec4 DeathEndCol = { 241 / 255.0f, 11 / 255.0f, 11 / 255.0f, 1.0f };
            float RadiusMax = 1.5;
            Drawable* DeathParts;
            GLuint particleShaderProgram, shaderProgram, PVParticleLocation;
            int AnimationTime=100;
            bool stopGame = false;



            //life Bar======================
            Drawable* LifeBar;
            float sizeBar = 0.087;
            float XFactor = 0.1f;
            GLuint IsRedloc, IsGreenloc;





    };



#endif
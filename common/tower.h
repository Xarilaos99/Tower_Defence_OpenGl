#ifndef TOWER_H
#define TOWER_H
    #pragma once
    #include <glm/glm.hpp>
    #include <vector>
    #include <GL/glew.h>
    #include "model.h"
    #include "robot.h"
    #include "sfaira.h"
    #include <common/TailEmitter.h>
    #include <common/PolarEmitter.h>


    class Tower{
        public:
            Tower();
            Tower(glm::vec3 pos,float size);
            ~Tower() = default;
            //void CreateTower();
            void Draw(glm::mat4 proj, glm::mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc,float dt, bool DrawParts=true);
            void CalDis(glm::vec3 RobotPos);
            void DrawSfairaAABB(glm::mat4 proj, glm::mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc);
            bool RobotCollusion(Robot &TempRobot,float dt);
            void PassUniform(GLuint Isloc) {
                IsSfaira = Isloc;
            }
            void PassPrograms(GLuint particleShaderProgram, GLuint PVParticleLocation, GLuint shaderProgram);
            glm::vec3 GetPos() { return CurrentPos; }
            void Starting() {
                CurrentSfaira->Starting();
            }

            bool IsStarting() {
                return  CurrentSfaira->GetPos() == CurrentSfaira->GetStartingPos();
            }

            bool GetFirstCheck() {
                return firstCheck;
            }
             
            void ChangeFirstCheck(bool fcheck) {
                firstCheck = fcheck;
            }


            

            

            TailEmitter* NormalParticles;
            TailEmitter* BigParticles;
            TailEmitter* TailParticles;


            PolarEmitter* CollusionEmitter;
            PolarEmitter* NormalCollusionEmitter;
            PolarEmitter* BigCollusionEmitter;

            bool colussion=false;

            void updateOrbit(int new_particles, float dt);
            void DrawEmitterOrbit(glm::mat4 PV, GLuint& VPloc, GLuint particleShaderProgram);
            void DrawEmitter(glm::mat4 PV, GLuint &VPloc, GLuint particleShaderProgram);
        private:
            glm::mat4  TowerModelMatrix;
            glm::mat4  TowerScaleMatrix;
            glm::vec3 CurrentPos;
            Drawable* TowerModel;
            

            //Sfaira =======================================

            Sfaira *NormalSfaira;
            
            Sfaira *BigSfaira;
            Sfaira* CurrentSfaira;
            int CollusionCntr=0;
            GLuint IsSfaira;
            
            int AnimationOrbit=-1;

           
            //Particles =======================================
            
            GLuint particleShaderProgram;
            GLuint PVParticleLocation;
            GLuint shaderProgram;
            glm::mat4 PV;

            Drawable* parts;


            //Collusion Particles====================
            Drawable* ColParts;
            glm::vec3 DirParts;

            //Big Sfaira colors========================
            glm::vec4 DBegClr = { 243 / 255.0f,1/ 255.0f,1/ 255.0f, 1.0f };
            glm::vec4 DEndClr = { 1 / 255.0f,6/ 255.0f,2/ 255.0f, 1.0f };

            //Big Sfaira Cullusion colors========================
            
            //the first check of tower dis with robot
            bool firstCheck=true;




    };



#endif
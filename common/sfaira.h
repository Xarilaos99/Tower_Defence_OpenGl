#ifndef SFAIRA_H
#define SFAIRA_H
#pragma once
    #include <glm/glm.hpp>
    #include <vector>
    #include <GL/glew.h>
    #include "model.h"
    #include "robot.h"

    class Sfaira{

        public:
            Sfaira(glm::vec3 Pos, float size, float VelFactor);
            ~Sfaira();
            float CheckColussion(Robot& TempRobot);
            void Draw(glm::mat4 proj, glm::mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc);
            void DrawSfairaAABB(glm::mat4 proj, glm::mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc);

            
            glm::vec3 GetPos() {
                return SfairaPos;
            }


            glm::vec3 GetStartingPos() {
                return SfairaStartingPos;
            }

            glm::vec3 GetVel() {
                return velocity;
            }

            int GetAttack() {
                return Attack;
            }
            void ChangeAttack(int att) {
                Attack = att;
            }
            
            void ChangeVelPos(glm::vec3 dir,float dt) {
                velocity = dir - SfairaPos;
                velocity /= CalcMag(velocity);
                SfairaPos += VelFactor * velocity*dt;
            }
            void Starting() {
                SfairaPos = SfairaStartingPos;
            }
            

        private:
            Drawable* sfaira;
            float HightPoint = 0;
            float RightPoint = 0;



            glm::vec3 SfairaMaxPoint = glm::vec3(0, 0, 0);
            glm::vec3 SfairaMinPoint = glm::vec3(0, 0, 0);
            
            
            std::vector<glm::vec3> AABBVerts ;

            glm::vec3 SfairaPos=glm::vec3(0,0,0);
            glm::vec3 SfairaStartingPos =glm::vec3(0,0,0);
            void CreateSfairaAABB();
            float CalcMag(glm::vec3 myvec) {
                return sqrt(pow(myvec.x, 2) + pow(myvec.y, 2) + pow(myvec.z, 2));
            }

            glm::vec3 velocity;
            float size;
            float VelFactor;
            Drawable* sfairaAABB;
            int Attack=10;




    };



#endif
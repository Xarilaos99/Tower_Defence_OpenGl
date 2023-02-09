#ifndef CRYSTAL_H
#define CRYSTAL_H
    #pragma once
    #include <glm/glm.hpp>
    #include <vector>
    #include <GL/glew.h>
    #include "model.h"
    #include "robot.h"


    class Crystal{
        public:
            Crystal();
            Crystal(glm::vec3 pos,float size);
            ~Crystal() = default;
            //void CreateCrystal();
            void Draw(glm::mat4 proj, glm::mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc);
            void DrawAABB(glm::mat4 proj, glm::mat4 view, GLuint Vloc, GLuint Mloc, GLuint Ploc);
            void RobotCollusion(Robot& TempRobot);
            float GetHealth() { return health; }
            void Attack(int Val) {
                health -= Val;
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
            void CreateAABB();
            glm::vec3 CrystalMaxPoint = glm::vec3(0, 0, 0);
            glm::vec3 CrystalMinPoint = glm::vec3(0, 0, 0);
            Drawable* AABBCrystal;
            float CheckColussion(Robot& TempRobot);

            int health =0;
            //Edw prepei na ftiaxw  otan to health tha einai 0 na exoume explode kai particles




    };



#endif
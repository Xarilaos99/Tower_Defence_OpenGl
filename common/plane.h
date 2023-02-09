#ifndef PLANE_H
#define PLANE_H
    #pragma once
    #include <glm/glm.hpp>
    #include <vector>
    #include "texture.h"
    #include <GL/glew.h>
    #include "model.h"

    class Plane{
        public:
            Plane();
            Plane(glm::vec3 pos);
            ~Plane() = default;
            void CreatePlane();
            void Draw(glm::mat4 model, glm::mat4 proj, glm::mat4 view,GLuint VPloc,GLuint Mloc);
            void DrawInstanced(glm::mat4 model, glm::mat4 proj, glm::mat4 view,GLuint Vloc,GLuint Mloc, GLuint Ploc);
            const glm::vec3 GetPos(){return position;};
            
            std::vector<glm::vec3>  GetVert(){return plane->vertices;}
            std::vector<glm::vec3>  GetNorm(){return plane->normals;}
            std::vector<glm::vec2>  GetUvs(){return plane->uvs;}

            int  GetLenPath(){return LenPath;}
            int indexRowMatrix[14] = { 0,1,2,3,3,3,4,5,6,6,7,7,8,9 };
            int indexColumnMatrix[14] = { 5,5,5,5,4,3,3,3,3,4,4,5,5,5 };
            
        private:
            void CreateTerrain();
            void CreateTerrainWithIndex();

            glm::vec3 position;
            Drawable* plane;
            glm::vec3 positionX=glm::vec3(1.0,0.0,0.0);
            glm::vec3 positionZ=glm::vec3(0.0,0.0,1.0);
            glm::vec3 positionDiag=glm::vec3(1.0,0.0,1.0);
            int texSel[100] = { };
            int LenPath = sizeof(indexRowMatrix) / sizeof(indexRowMatrix[0]);
            

            
    };


#endif
#ifndef RENDERER_H
#define RENDERER_H
#pragma once

#include <functional>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glfw3.h>




class Renderer{


    public:
        Renderer();
        ~Renderer();
        //void Initialize();
        void createContext();
        void scene();
        void Objects(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, GLuint VLocation, GLuint PLocation, GLuint MLocation,float dt);
        static void pollKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
        void HelpingWindow();

        bool GetEndGame() {
            return EndGame;
        }

        void AccessEndGame(bool endg) {
            EndGame = endg;
        }
    
    private:
        bool EndGame = false;

        

};








#endif
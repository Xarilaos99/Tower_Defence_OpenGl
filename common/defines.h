#ifndef DEFINE_H
#define DEFINE_H

    #pragma once
    #include <iostream>
    #include <string>
    #include <vector>
    #include <stdio.h>

    //ImGui
    #include "imgui.h"
    #include "imgui_impl_glfw.h"
    #include "imgui_impl_opengl3.h"

    // Include GLEW
    #include <GL/glew.h>

    // Include GLFW
    #include <glfw3.h>

    // Include GLM
    #include <glm/glm.hpp>
    #include <glm/ext.hpp>
    #include <glm/gtc/matrix_transform.hpp>



    // Shader loading utilities and other
    #include <common/shader.h>
    #include <common/util.h>
    #include <common/camera.h>
    #include <common/model.h>
    #include <common/texture.h>
    #include <common/plane.h>
    #include <common/light.h>
    #include <common/robot.h>
    #include <common/crystal.h>
    #include <common/tower.h>
    

    using namespace std;
    using namespace glm;
    using namespace ImGui;
    using namespace ogl;

    #define W_WIDTH 1024
    #define W_HEIGHT 1024
    #define SHADOW_WIDTH 1024
    #define SHADOW_HEIGHT 1024
    #define TITLE "My Tower Defence"



    
    GLFWwindow* window;
    Camera* camera;


    GLuint colors;


    //===============Time================================================
    
    float currentTime;
    float Time;


    //===========Shader Stuff=================================
    //===========Programs=================================
    GLuint shaderProgram;






    //===========Uniforms=================================
    GLuint VLocation, MLocation, PLocation;
    GLuint IsPlaneloc, IsTowerloc;
    GLuint IsCrystaloc, IsRobotloc;

    //============================================



    //===========Drawables=================================
    Drawable* terrain;
    Drawable* tower;
    Drawable* crystal;
    Drawable* robot;




    //============================================



    //===========Textures=================================
    Texture roadTex;
    Texture grassTex;
    Texture towerTex;
    Texture crystalTex;
    Texture robotTex;



    //===============MY Objects=============================


    //===========Plane=================================
    Plane* onePlane;
    //============================================
    // 
    //===========Robot=================================
    Robot* oneRobot;
    //============================================
    
    //===========Crystal=================================
    Crystal* oneCrystal;
    //============================================
    
    //===========Tower=================================
    Tower* oneTower;
    //============================================
    





    float color[4] = { 0.8f,0.5f,0.2f,1.0f };
    //===========Light====================================================================
    Light* light;

    float width = 400;
    float height = 300;
    float pos_x = 20;
    float pos_y = 20;




    GLuint quadTextureSamplerLocation;

    //DepthMap====================================================================
    //DepthMap=============================
    GLuint depthProgram;
    GLuint miniProgram;

    GLuint frameBuffer, depthTexture;
    GLuint depthMapSampler;

    GLuint shadowViewLocation;
    GLuint shadowProjectionLocation;
    GLuint shadowModelLocation;
    Drawable* quad;


    glm::mat4 viewMatrix, projectionMatrix;


    void DepthMapShaderCall();
    void DepthPass(mat4 viewMatrix, mat4 projectionMatrix);
    void RenderDepthMap();


    GLuint IsPlanelocShadow;
    Drawable *sphere;
    Drawable *suz;



   





    

#endif
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
   
    #include <common/particles.h>
    

    using namespace std;
    using namespace glm;
    using namespace ImGui;
    using namespace ogl;


    //MY TV==============================================
    //*
    #define W_WIDTH 1366
    #define W_HEIGHT 766
    //*/

    //MY Laptop==============================================
    /*
    #define W_WIDTH 1920
    #define W_HEIGHT 1080
    //*/

    #define SHADOW_WIDTH 1024
    #define SHADOW_HEIGHT 1024
    #define TITLE "My Tower Defence"



    
    GLFWwindow* window;
    Camera* camera;


    GLuint colors;


    //===============Time================================================
    
    float currentTime;
    float Time;
    float TowerTime=6.0f;
    float dt;


    //===========Shader Stuff=================================
    //===========Programs=================================
    GLuint shaderProgram;






    //===========Uniforms=================================
    GLuint VLocation, MLocation, PLocation;
    GLuint IsPlaneloc, IsTowerloc;
    GLuint IsCrystaloc, IsRobotloc;
    GLuint IsGreenloc, IsRedloc;
    GLuint IsSfairaloc,IsSelectedloc,IsObjectloc;

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
    





    
    //===========Light====================================================================
    Light* light;

    float width = 400;
    float height = 300;
    float pos_x = 600;
    float pos_y = 20;




    GLuint quadTextureSamplerLocation;

    //DepthMap====================================================================

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
    void DepthPass(mat4 viewMatrix, mat4 projectionMatrix,float dt);
    void RenderDepthMap();


    GLuint IsPlanelocShadow;
    Drawable *sphere;
    Drawable *suz;


    //Particle System====================================================================
    //Shaders 
    
    GLuint particleShaderProgram;
    GLuint PVParticleLocation;

    //Orbit Vars=================
    bool Col=false;
    int update=-1;

    //Inderaction Robots vs Towers=================
    //vectors of robots and Tower

    std::vector<Robot *> Robots;
    std::vector<Tower *> Towers;

    int MaxRobots = 5;
    int RobCnt=0;
  
    float GetDistance(Robot* rob, Tower* tow);
   
    float CalcLength(glm::vec3 myvec) {
        return sqrt(pow(myvec.x, 2) + pow(myvec.y, 2) + pow(myvec.z, 2));
    }
  

    float MinDis=10000.0f;
    int* ActiveRobot= new int[2]();
    
    bool CheckRobots = true;
    bool tempCheck = false;


    //Select Part =============================================
    bool SelectSection=false;
    Plane* SelectPlane;
    



    //Game=========================================
    bool game_paused = false;
    


   





    

#endif
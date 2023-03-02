#pragma once
#include "defines.h"
#include "../common/renderer.h"
 


vec3 currentPos;
float LastPos ;






Renderer::Renderer(){

    // Initialize GLFW
    if (!glfwInit()) {
        throw runtime_error("Failed to initialize GLFW\n");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, TITLE, NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        throw runtime_error(string(string("Failed to open GLFW window.") +
            " If you have an Intel GPU, they are not 3.3 compatible." +
            "Try the 2.1 version.\n"));
    }
    glfwMakeContextCurrent(window);

    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Start GLEW extension handler
    glewExperimental = GL_TRUE;

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        throw runtime_error("Failed to initialize GLEW\n");
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Hide the mouse and enable unlimited movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, W_WIDTH / 2, W_HEIGHT / 2);

    // Gray background color
    //glClearColor(0.5f, 0.3f, 0.1f, 0.45f);
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
    glfwSetKeyCallback(window, pollKeyboard);

    //glfwSetKeyCallback(window, pollKeyboard);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    

    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glEnable(GL_CULL_FACE);



    logGLParameters();
    glEnable(GL_TEXTURE_2D);

    
    
   
   
    

    camera = new Camera(window);


    light = new Light(window,
        vec4{ 1, 1, 1, 1 },
        vec4{ 1, 1, 1, 1 },
        vec4{ 1, 1, 1, 1 },
        vec3{ -1.1, 6.5, 10.9},
        58.0f
    );

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
        camera->onMouseMove(xpos, ypos);
        }
    );


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

}



Renderer::~Renderer(){
    glDeleteProgram(shaderProgram);
    glDeleteProgram(depthProgram);
    glDeleteProgram(miniProgram);
    glDeleteProgram(particleShaderProgram);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    DestroyContext();
    
    
    glfwTerminate();
    
}


//this function is initialaze the shaders
void Renderer::createContext(){

    

    //Shaders============================================================================

    shaderProgram = loadShaders("SimpleTexture.vertexshader", "SimpleTexture.fragmentshader");
    depthProgram = loadShaders("Depth.vertexshader", "Depth.fragmentshader");
    miniProgram = loadShaders("Quad.vertexshader", "Quad.fragmentshader");

    // Draw wire frame triangles or fill: GL_LINE, or GL_FILL
    
    colors= glGetUniformLocation(shaderProgram, "mycolor");


    //Textures============================================================================
    roadTex.myloaderBMP("Assets/road.bmp");
    grassTex.myloaderBMP("Assets/CloisterGrounds.bmp");
    towerTex.myloaderBMP("Assets/tower.bmp");
    crystalTex.myloaderBMP("Assets/crystal.bmp");
    robotTex.myloaderBMP("Assets/robot.bmp");



    //Shader Call Textures============================================================================
    roadTex.shaderCall(shaderProgram, "roadSampler");
    grassTex.shaderCall(shaderProgram, "grassSampler");
    towerTex.shaderCall(shaderProgram, "towerSampler");
    crystalTex.shaderCall(shaderProgram, "crystalSampler");
    robotTex.shaderCall(shaderProgram, "robotSampler");


   

    
    

  

    // Get a pointer location to model matrix in the vertex shader
    VLocation = glGetUniformLocation(shaderProgram, "V");
    MLocation = glGetUniformLocation(shaderProgram, "M");
    PLocation = glGetUniformLocation(shaderProgram, "P");




    IsPlaneloc = glGetUniformLocation(shaderProgram, "IsPlane");
    IsTowerloc = glGetUniformLocation(shaderProgram, "IsTower");
    IsCrystaloc = glGetUniformLocation(shaderProgram, "IsCrystal");
    IsRobotloc = glGetUniformLocation(shaderProgram, "IsRobot");
    IsPlanelocShadow = glGetUniformLocation(depthProgram, "IsPlane");

    IsRedloc = glGetUniformLocation(shaderProgram, "IsRed");
    IsGreenloc= glGetUniformLocation(shaderProgram, "IsGreen");
    IsSfairaloc= glGetUniformLocation(shaderProgram, "IsSfaira");


    IsSelectedloc= glGetUniformLocation(shaderProgram, "IsSelectPlane");
    IsObjectloc= glGetUniformLocation(shaderProgram, "IsObject");


    //DepthMap===========================================================
    
    DepthMapShaderCall();
    

    //Quad===========================================================
    vector<vec3> quadVertices = {
      vec3(0.5, 0.5, 0.0),
      vec3(1.0, 0.5, 0.0),
      vec3(1.0, 1.0, 0.0),

      vec3(1.0, 1.0, 0.0),
      vec3(0.5, 1.0, 0.0),
      vec3(0.5, 0.5, 0.0)
    };

    vector<vec2> quadUVs = {
      vec2(0.0, 0.0),
      vec2(1.0, 0.0),
      vec2(1.0, 1.0),

      vec2(1.0, 1.0),
      vec2(0.0, 1.0),
      vec2(0.0, 0.0)
    };

    quad= new Drawable(quadVertices, quadUVs);


    /*
    sphere = new Drawable("earth.obj");
   
    
    for (int i = 0; i < sphere->normals.size(); i++) {

        sphere->normals[i] *= -1.0;

    }

    //sphere = new Drawable(sphere->vertices, sphere->uvs, sphere->normals);
    //suz = new Drawable("suzanne.obj");
    //*/
    


    //Light==============================================================


    light->shaderCall(shaderProgram);


    //Particle System=======================================================
    particleShaderProgram = loadShaders(
        "ParticleShader.vertexshader",
        "ParticleShader.fragmentshader");
    PVParticleLocation = glGetUniformLocation(particleShaderProgram, "PV");

    onePlane = new Plane(vec3(0.0, 0.0, 0.0));
    onePlane->CreatePlane();

    
   
    oneCrystal = new Crystal(vec3(4.5, 0, 10.5), 0.025);
    oneCrystal->PassPrograms(particleShaderProgram, shaderProgram, PVParticleLocation);
    
    
    oneRobot = new Robot(onePlane->indexColumnMatrix[4], onePlane->GetLenPath(), 0.2);
    oneRobot->AccessBarLoc(IsRedloc, IsGreenloc);
    oneRobot->PassPrograms(particleShaderProgram, shaderProgram, PVParticleLocation);

    


    /*

    Towers.push_back(new Tower(vec3(4.5, 0, 5.5), 0.2));
    //Towers.push_back(new Tower(vec3(2.5, 0, 8.5), 0.2));

    for (int i = 0; i < Towers.size(); i++) {
        Towers[i]->PassPrograms(particleShaderProgram, PVParticleLocation, shaderProgram);
        Towers[i]->PassUniform(IsSfairaloc);;
    }
    */

    glUniform1i(IsRedloc, 0);
    glUniform1i(IsGreenloc, 0);


    

    for (int i = 0; i < MaxRobots; i++) {
        
        Robots.push_back(new Robot(onePlane->indexColumnMatrix[0], onePlane->GetLenPath(), 0.2));
        Robots[i]->AccessBarLoc(IsRedloc, IsGreenloc);
        Robots[i]->PassPrograms(particleShaderProgram, shaderProgram, PVParticleLocation);
        
    }

    

    //Select Plane=============================================
    SelectPlane = new Plane(vec3(0.0,0.1,0.0));
    SelectPlane->CreatePlane();
    SelectPlane->PassObjectloc(IsObjectloc);
    

    
}





void Renderer::scene(){
    Time = glfwGetTime();
    

    
    
    
    oneCrystal->AccessBarLoc(IsRedloc, IsGreenloc);
    //onerobot->CreateRobot();

    glUniform1i(IsPlaneloc, 0);
    glUniform1i(IsTowerloc, 0);
    glUniform1i(IsCrystaloc, 0);
    glUniform1i(IsRobotloc, 0);
    glUniform1i(IsSfairaloc, 0);
    glUniform1i(IsPlanelocShadow, 0);
    glUniform1i(IsSelectedloc, 0);

   
    light->update();
    light->upLoad();


    


    do{
        currentTime = glfwGetTime();
        
        
        dt = currentTime - Time;
        //float dt = 0.2f;
        


        //camera
       
        camera->update(SelectSection);
        
        
        mat4 projectionMatrix = camera->projectionMatrix;
        mat4 viewMatrix = camera->viewMatrix;
       

        

        DepthPass(light->viewMatrix, light->projectionMatrix,dt);

        glUseProgram(shaderProgram);



        roadTex.useTexture(0);
        grassTex.useTexture(1);
        towerTex.useTexture(2);
        crystalTex.useTexture(3);
        robotTex.useTexture(4);

        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glUniform1i(depthMapSampler, 5);


        

        
        glViewport(0, 0, W_WIDTH, W_HEIGHT);

        // Clearing color and depth info
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //if u want to change light pos=======================================
        //light->update();
        light->upLoad();

        

        Objects(viewMatrix, projectionMatrix,VLocation,PLocation,MLocation,dt);



        //if u want to see depth Map up right pos==============
        //RenderDepthMap();



        //ImGui Window====================================
        //HelpingWindow();
        AccessEndGame(oneCrystal->GetEndGame());
        glfwPollEvents();
        glfwSwapBuffers(window);
        Time= currentTime;
        TowerTime += dt;
    }while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
    glfwWindowShouldClose(window) == 0 && !EndGame);
}



void Renderer::Objects(mat4 viewMatrix,mat4 projectionMatrix,GLuint VLocation, GLuint PLocation, GLuint MLocation,float dt) {

    

    glUniform1i(IsPlaneloc, 1);

    onePlane->DrawInstanced(mat4(1), projectionMatrix, viewMatrix, VLocation, MLocation, PLocation);
    glUniform1i(IsPlaneloc, 0);
    
    glUniform1i(IsCrystaloc, 1);
    if(!game_paused)oneCrystal->Draw(projectionMatrix, viewMatrix, VLocation, MLocation, PLocation,dt,true);
    glUniform1i(IsCrystaloc, 0);
    

    //oneCrystal->DrawAABB(projectionMatrix, viewMatrix, VLocation, MLocation, PLocation);
    
   
  
    for (int i = 0; i < Towers.size(); i++) {
        glUniform1i(IsTowerloc, 1);
        Towers[i]->Draw(projectionMatrix, viewMatrix, VLocation, MLocation, PLocation, dt);
        glUniform1i(IsTowerloc, 0);
        //Towers[i]->DrawSfairaAABB(projectionMatrix, viewMatrix, VLocation, MLocation, PLocation);
        //*
        if (!Towers[i]->IsStarting() ) {
            Towers[i]->DrawEmitter(projectionMatrix * viewMatrix, PVParticleLocation, particleShaderProgram);
            glUseProgram(shaderProgram);

        }
        //*/
        
    }
       
   
    


    //Check How much Robots to Draw Correct;
    //*
    if (Robots.size() != 0) {
        for (int i = 0; i < Robots.size() - 1; i++) {
            RobCnt += Robots[i]->GetIndex();

        }
    }


   
   
    
   
    
    //*
    if(Robots.size()!=0 ){
        for (int j = 0; j < RobCnt+1  ; j++) {
        
                glUniform1i(IsRobotloc, 1);
                Robots[j]->Draw(projectionMatrix, viewMatrix, VLocation, MLocation, PLocation, dt, true);


                glUniform1i(IsRobotloc, 0);
                //Robots[j]->DrawAABB(projectionMatrix, viewMatrix, VLocation, MLocation, PLocation);
                
                if(!game_paused) Robots[j]->UpdateRobot(dt, onePlane->indexColumnMatrix);

                if (Robots.size() == 1 && Robots[0]->IsDead()) CheckRobots = false;
                if (!Robots[j]->IsDead()) {
                    oneCrystal->RobotCollusion(*Robots[j]);
                }

                if (Robots[j]->ChechErase()) {
                    Robots.erase(Robots.begin() + j);
                    RobCnt--;

                    for (int k = 0; k < Towers.size(); k++) {
                        ActiveRobot[k] = (ActiveRobot[k] >= Robots.size()) ? --ActiveRobot[k] : ActiveRobot[k];
                    }
                    
                    
                   

                }

                
            

            
        


        }
    }
    //*/

    if (CheckRobots && !game_paused && !oneCrystal->IsDead()) {
        for (int j = 0; j < Towers.size(); j++) {

            if (Towers[j]->GetFirstCheck()) {
                for (int i = 0; i < RobCnt + 1; i++) {
                    if (!Robots[i]->IsDead()) {
                        float dis = GetDistance(Robots[i], Towers[j]);
                        //cout << dis << endl;

                        if (MinDis > dis && dis < 3.0f) {
                            MinDis = dis;
                            ActiveRobot[j] = i;
                            
                            Towers[j]->ChangeFirstCheck(false);
                        }
                        tempCheck = true;


                    }

                }
                CheckRobots = tempCheck;
            }
            else {

                if (Towers[j]->RobotCollusion(*Robots[ActiveRobot[j]], dt)) {
                    if (Robots[ActiveRobot[j]]->IsDead()) {
                        for (int k = 0; k < Towers.size(); k++) {
                            if (ActiveRobot[k] == ActiveRobot[j]) {
                                Towers[k]->Starting();
                                Towers[k]->ChangeFirstCheck(true);
                            }
                        }
                    }

                }
            }
        }




    }
    else if(!game_paused){
        for (int j = 0; j < Towers.size(); j++) {
            Towers[j]->colussion = false;
            Towers[j]->Starting();

        }

    }

   
 
    




    
    MinDis = 100.0f;
    tempCheck = false;
    

    

   
    if (SelectSection) {
        glUniform1i(IsSelectedloc, 1);

        SelectPlane->Draw(mat4(1), projectionMatrix, viewMatrix, VLocation, MLocation, PLocation);
        glUniform1i(IsSelectedloc, 0);
    }

   

    /*
    sphere->bind();
    

    ModelMatrix = translate(mat4(), light->lightPosition_worldspace)* scale(mat4(), vec3(0.5, 0.5, 0.5));
    glUniformMatrix4fv(MLocation, 1, GL_FALSE, &ModelMatrix[0][0]);
    sphere->draw();
    
    
    
    suz->bind();
    
    ModelMatrix = translate(mat4(), vec3(7.5,0.5, 7.5)) * glm::rotate(mat4(), (float)3.14 / 2, vec3(0.0, -1.0, 0.0)) * scale(mat4(), vec3(0.5, 0.5, 0.5));
    glUniformMatrix4fv(VLocation, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(PLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
    glUniformMatrix4fv(MLocation, 1, GL_FALSE, &ModelMatrix[0][0]);
    suz->draw();
    //*/
    
    

    

}



void Renderer::pollKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods){

    if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS) {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            camera->active = true;
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            camera->active = false;
        }

    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        game_paused = !game_paused;
    }


    
   
    if (TowerTime >8) {
        if (Towers.size() < 3) {

            if (key == GLFW_KEY_T && action == GLFW_PRESS) {
                SelectSection = !SelectSection;
            }




            if (SelectSection) {
                if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
                    if (SelectPlane->ZOffset != 0.0) {
                        SelectPlane->ZOffset -= 1.0;
                    }

                }

                if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
                    if (SelectPlane->ZOffset != 9.0) {
                        SelectPlane->ZOffset += 1.0;
                    }

                }

                if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
                    if (SelectPlane->XOffset != 9.0) {
                        SelectPlane->XOffset += 1.0;

                    }

                }

                if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
                    if (SelectPlane->XOffset != 0.0) {
                        SelectPlane->XOffset -= 1.0;
                    }

                }
                SelectPlane->Update();

                if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
                    SelectSection = !SelectSection;
                    if (SelectPlane->MakeTower()) {
                        Tower* t = new Tower(vec3(SelectPlane->XOffset + .5, 0, SelectPlane->ZOffset + .5), 0.2);
                        t->PassPrograms(particleShaderProgram, PVParticleLocation, shaderProgram);
                        t->PassUniform(IsSfairaloc);
                        Towers.push_back(t);
                        TowerTime = 0;
                        

                    }
                    else cout << "Apagoreumenh Thesi" << endl;
                    SelectPlane->XOffset = 0.0;
                    SelectPlane->ZOffset = 0.0;
                    delete[] ActiveRobot;
                    ActiveRobot = new int[Towers.size()]();

                }

            }
        }
        else cout << "Den mporeis na ftiaxeis perissoterous Purgous" << endl;
    }
    else cout << "Den mporeis akoma na ftiajeis Purgo" << endl;



}


///*
void DepthMapShaderCall() {
    glGenFramebuffers(1, &frameBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);


    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0,
        GL_DEPTH_COMPONENT, GL_FLOAT, NULL);


    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);


    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);



    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        glfwTerminate();
        throw runtime_error("Frame buffer not initialized correctly");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    depthMapSampler = glGetUniformLocation(shaderProgram, "shadowMapSampler");
    shadowViewLocation = glGetUniformLocation(depthProgram, "V");
    shadowProjectionLocation = glGetUniformLocation(depthProgram, "P");
    shadowModelLocation = glGetUniformLocation(depthProgram, "M");

    // --- miniMapProgram ---
    quadTextureSamplerLocation = glGetUniformLocation(miniProgram, "textureSampler");
}


void RenderDepthMap() {
    glUseProgram(miniProgram);

    //enabling the texture - follow the aforementioned pipeline
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glUniform1i(quadTextureSamplerLocation, 0);

    quad->bind();
    quad->draw();

}

void DepthPass(mat4 viewMatrix,mat4 projectionMatrix,float dt) {



    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

    // Step 2: Clearing color and depth info
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    glClear(GL_DEPTH_BUFFER_BIT);

    // Step 3: Selecting shader program
    glUseProgram(depthProgram);

    mat4 ModelMatrix;

    

    //======================================================================
    glUniform1i(IsPlaneloc, 1);

    onePlane->DrawInstanced(mat4(1), projectionMatrix, viewMatrix, shadowViewLocation, shadowModelLocation, shadowProjectionLocation);
    glUniform1i(IsPlaneloc, 0);


    glUniform1i(IsCrystaloc, 1);
    if (!oneCrystal->IsDead()) oneCrystal->Draw(projectionMatrix, viewMatrix, shadowViewLocation, shadowModelLocation, shadowProjectionLocation,dt,false,false);
    glUniform1i(IsCrystaloc, 0);

   

    for (int i = 0; i < Towers.size(); i++) {

        Towers[i]->Draw(projectionMatrix, viewMatrix, shadowViewLocation, shadowModelLocation, shadowProjectionLocation, dt,false);
    }
  
    if (Robots.size() != 0){
        for (int i = 0; i < RobCnt + 1; i++) {

            if(!Robots[i]->IsDead()) Robots[i]->Draw(projectionMatrix, viewMatrix, shadowViewLocation, shadowModelLocation, shadowProjectionLocation, dt, false,false);
        }

        RobCnt = 0;
    }
   
    

    

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    


}




void Renderer::HelpingWindow() {
    float* t[3] = { &light->targetPosition.x,&light->targetPosition.y,&light->targetPosition.z };
    float* pos[3] = { &light->lightPosition_worldspace.x,&light->lightPosition_worldspace.y,&light->lightPosition_worldspace.z };
    float* campos[3] = { &camera->position.x,&camera->position.y,&camera->position.z };


    //ImGui=============================================================
    //*
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    NewFrame();
    Begin("ImGui window");
    //SetWindowPos(ImVec2(pos_x, pos_y));
    //SetWindowSize(ImVec2(width, height));
    Text("That's very bad");
    //ColorEdit4("Xrwma", color);
    SliderFloat("near", &light->nearPlane, 0.1, 50);
    SliderFloat("far", &light->farPlane, 20, 100);
    SliderFloat("win", &light->win, 0, 100);
    SliderFloat("power", &light->power, 0, 100);
    SliderFloat3("target", *t, 0, 10);
    SliderFloat3("pos", *pos, 0, 10);
    SliderFloat3("campos", *campos, 0, 20);

    //ColorEdit4("Color Begin", glm::value_ptr(oneTower->TailParticles->ColorBegin));
    //ColorEdit4("Color  End", glm::value_ptr(oneTower->TailParticles->ColorEnd));
    SliderFloat("XOffset", &SelectPlane->XOffset,0.0, 10.0);
    //SliderFloat("Size", &oneCrystal->sizeBar, 0.01, 0.3);


    ImGui::Text("Performance %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    End();
    Render();
    ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
}




float GetDistance(Robot* rob, Tower* tow) {
    glm::vec3 dis = tow->GetPos()-rob->GetCenter();
    return CalcLength(dis);
}













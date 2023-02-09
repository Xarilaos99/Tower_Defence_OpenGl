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
    

    
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glEnable(GL_CULL_FACE);



    logGLParameters();
    glEnable(GL_TEXTURE_2D);

    
    
   
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    

    camera = new Camera(window);


    light = new Light(window,
        vec4{ 1, 1, 1, 1 },
        vec4{ 1, 1, 1, 1 },
        vec4{ 1, 1, 1, 1 },
        vec3{ 5.8, 5.3, 5.9},
        43.0f
    );

}



Renderer::~Renderer(){
    glDeleteProgram(shaderProgram);
    glDeleteProgram(depthProgram);
    glDeleteProgram(miniProgram);

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

    //DRAWABLES============================================================================
    tower = new Drawable("tower.obj");
    crystal = new Drawable("crystal.obj");
    robot = new Drawable("robot.obj");


    //Textures============================================================================
    roadTex.myloaderBMP("Assets/road.bmp");
    grassTex.myloaderBMP("Assets/CloisterGrounds.bmp");
    towerTex.myloaderBMP("tower.bmp");
    crystalTex.myloaderBMP("crystal.bmp");
    robotTex.myloaderBMP("robot.bmp");



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

    sphere = new Drawable("earth.obj");
   
    //*
    for (int i = 0; i < sphere->normals.size(); i++) {

        sphere->normals[i] *= -1.0;

    }

    sphere = new Drawable(sphere->vertices, sphere->uvs, sphere->normals);
    suz = new Drawable("suzanne.obj");

    


    //Light==============================================================


    light->shaderCall(shaderProgram);
}





void Renderer::scene(){
    Time = glfwGetTime();
    onePlane = new Plane(vec3(0.0, 0.0, 0.0));
    onePlane->CreatePlane();

    oneRobot = new Robot(onePlane->indexColumnMatrix[0],onePlane->GetLenPath(),0.2);
    oneTower = new Tower(vec3(4.5,0,5.5), 0.2);
    oneCrystal = new Crystal(vec3(5.5, 0, 10), 0.01);
    //onerobot->CreateRobot();

    glUniform1i(IsPlaneloc, 0);
    glUniform1i(IsTowerloc, 0);
    glUniform1i(IsCrystaloc, 0);
    glUniform1i(IsRobotloc, 0);
    glUniform1i(IsPlanelocShadow, 0);
    light->update();
    light->upLoad();


    do{
        currentTime = glfwGetTime();
        



        //camera
        camera->update();
        mat4 projectionMatrix = camera->projectionMatrix;
        mat4 viewMatrix = camera->viewMatrix;
        glUniform4f(colors, color[0], color[1], color[2], color[3]);

        DepthPass(light->viewMatrix, light->projectionMatrix);

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

        // Step 2: Clearing color and depth info
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        light->update();
        light->upLoad();

        // Step 3: Selecting shader program

        Objects(viewMatrix, projectionMatrix,VLocation,PLocation,MLocation);
        


        //Update for===============
           
        /*
        if(dt>0.1 && tempindex < 14){
            time = currentTime;
            t++;
            if (t == 5) {

                tempindex++;
                if(tempindex<14){
                    vec3 temp = option;
                    if (LastPos > oneplane->indexColumnMatrix[tempindex]) {

                        option = -posX;
                    }
                    else if (LastPos < oneplane->indexColumnMatrix[tempindex]) {
                        option = posX;
                    }
                    else {
                        option = posZ;
                        

                    }
                    t = -3;
                    LastPos = oneplane->indexColumnMatrix[tempindex];
                }

            }
        
            //cout << (option * robotSpeed).x << "  " << (option * robotSpeed).y << "  " << (option * robotSpeed).z << "  " << endl;
            currentPos += option * robotSpeed;
            //cout << currentPos.x << "  " << currentPos.y << "  " << currentPos.z << "  " << endl;

        }
        */
        
        //MyDepthMap.RenderDepthMap(miniProgram, quad, quadTextureSamplerLocation);
        
       
        RenderDepthMap();

        float* t[3] = { &light->targetPosition.x,&light->targetPosition.y,&light->targetPosition.z };
        float* pos[3] = { &light->lightPosition_worldspace.x,&light->lightPosition_worldspace.y,&light->lightPosition_worldspace.z };
        float* campos[3] = { &camera->position.x,&camera->position.y,&camera->position.z };


        //ImGui=============================================================
        //*
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        NewFrame();
        Begin("ImGui window");
        SetWindowPos(ImVec2(pos_x, pos_y));
        SetWindowSize(ImVec2(width, height));
        Text("That's very bad");
        ColorEdit4("Xrwma", color);
        SliderFloat("near", &light->nearPlane, 0.1, 50);
        SliderFloat("far", &light->farPlane, 20, 100);
        SliderFloat("win", &light->win, 0, 100);
        SliderFloat("power", &light->power, 0, 100);
        SliderFloat3("target", *t, 0, 10);
        SliderFloat3("pos", *pos, 0, 10);
        SliderFloat3("campos", *campos, 0, 20);
        End();
        Render();
        ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());



        glfwSwapBuffers(window);
        glfwPollEvents();
    }while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
    glfwWindowShouldClose(window) == 0);
}



void Renderer::Objects(mat4 viewMatrix,mat4 projectionMatrix,GLuint VLocation, GLuint PLocation, GLuint MLocation) {

    mat4 ModelMatrix, VPMatrix;

    
    glUniform1i(IsPlaneloc, 1);

    onePlane->DrawInstanced(mat4(1), projectionMatrix, viewMatrix, VLocation, MLocation, PLocation);
    glUniform1i(IsPlaneloc, 0);
    
   

    ///*
    glUniform1i(IsCrystaloc, 1);
    oneCrystal->Draw(projectionMatrix, viewMatrix, VLocation, MLocation, PLocation);
    glUniform1i(IsCrystaloc, 0);
    oneCrystal->DrawAABB(projectionMatrix, viewMatrix, VLocation, MLocation, PLocation);
    oneCrystal->RobotCollusion(*oneRobot);
    

   

    glUniform1i(IsRobotloc, 1);
    oneRobot->Draw(projectionMatrix, viewMatrix, VLocation, MLocation, PLocation);
    oneRobot->UpdateRobot(currentTime, Time, onePlane->indexColumnMatrix);

    glUniform1i(IsRobotloc, 0);
    oneRobot->DrawAABB(projectionMatrix, viewMatrix, VLocation, MLocation, PLocation);

    glUniform1i(IsTowerloc, 1);
    oneTower->Draw(projectionMatrix, viewMatrix, VLocation, MLocation, PLocation);
    glUniform1i(IsTowerloc, 0);
    oneTower->DrawSfairaAABB(projectionMatrix, viewMatrix, VLocation, MLocation, PLocation);
    oneTower->RobotCollusion(*oneRobot);



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
    
    cout << oneRobot->GetHealth() << endl;

    

}
void Renderer::pollKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods){

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

void DepthPass(mat4 viewMatrix,mat4 projectionMatrix) {



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
    oneCrystal->Draw(projectionMatrix, viewMatrix, shadowViewLocation, shadowModelLocation, shadowProjectionLocation);
    glUniform1i(IsCrystaloc, 0);

    glUniform1i(IsRobotloc, 1);
    oneRobot->Draw(projectionMatrix, viewMatrix, shadowViewLocation, shadowModelLocation, shadowProjectionLocation);
    glUniform1i(IsRobotloc, 0);


    glUniform1i(IsTowerloc, 1);
    oneTower->Draw(projectionMatrix, viewMatrix, shadowViewLocation, shadowModelLocation, shadowProjectionLocation);
    glUniform1i(IsTowerloc, 0);
    
    

    suz->bind();

    ModelMatrix = translate(mat4(), vec3(7.5, 0.5, 7.5)) * glm::rotate(mat4(), (float)3.14 / 2, vec3(0.0, -1.0, 0.0)) * scale(mat4(), vec3(0.5, 0.5, 0.5));
    glUniformMatrix4fv(shadowViewLocation, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(shadowProjectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
    glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &ModelMatrix[0][0]);
    suz->draw();



    

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    


}


















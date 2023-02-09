

#include "../common/renderer.h"
#include <iostream>
using namespace std;

/*
void initialize();
void createContext();
void mainLoop();
void free();
void pollKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
*/



Renderer towerDefence;
// Global variables






// Function 
/*
void createContext() {
    // Create and compile our GLSL program from the shaders
    shaderProgram = loadShaders("SimpleTexture.vertexshader", "SimpleTexture.fragmentshader");

    // Draw wire frame triangles or fill: GL_LINE, or GL_FILL
    
    
    colors= glGetUniformLocation(shaderProgram, "mycolor");

    terrain = new Drawable("Assets/terrain.obj" );
    temp = new Drawable("suzanne.obj");

    


   //terrain = new Drawable(floorVertices, floorUVs, floorNormals);


    roadTex.myloaderBMP("Assets/road.bmp");
    roadTex.shaderCall(shaderProgram, "textureSampler");
    





    // Get a pointer location to model matrix in the vertex shader
    VPLocation = glGetUniformLocation(shaderProgram, "VP");
    MLocation = glGetUniformLocation(shaderProgram, "M");
}

*/


/*
void mainLoop() {


    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        
        

        
        //ImGui=============================================================
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        NewFrame();
        Begin("ImGui window");
        Text("That's very bad");
        ColorEdit4("Xrwma", color);
        End();
        Render();
        ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
        
        

        //camera
        camera->update();
        mat4 projectionMatrix = camera->projectionMatrix;
        mat4 viewMatrix = camera->viewMatrix;
        glUniform4f(colors, color[0], color[1], color[2], color[3]);

        
        terrain->bind();
        roadTex.useTexture();
        mat4 terrainModelMatrix = mat4(1);
        mat4 terrainVP = projectionMatrix * viewMatrix;
        glUniformMatrix4fv(VPLocation, 1, GL_FALSE, &terrainVP[0][0]);
        glUniformMatrix4fv(MLocation, 1, GL_FALSE, &terrainModelMatrix[0][0]);
        terrain->draw();
       

        
        glUniformMatrix4fv(VPLocation, 1, GL_FALSE, &terrainVP[0][0]);
        glUniformMatrix4fv(MLocation, 1, GL_FALSE, &terrainModelMatrix[0][0]);
        temp->bind();
        temp->draw();




        glfwSwapBuffers(window);
        glfwPollEvents();


    }while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
    glfwWindowShouldClose(window) == 0);
}
*/


//=================free======================
/*
void free() {

    


    glDeleteProgram(shaderProgram);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    DestroyContext();
    

    glfwTerminate();
}
*/

/*
void initialize() {
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

    //glfwSetKeyCallback(window, pollKeyboard);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glDepthFunc(GL_LESS);

    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);


 
    logGLParameters();

    
    
   
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    

    camera = new Camera(window);

}
*/





int main()
{
    
    
    try
    {
        /*
        initialize();
        createContext();
        mainLoop();
        free();
        */
        towerDefence.createContext();
        towerDefence.scene();


        //cout << "Ola kala" << endl;
    }
    catch (exception& ex)
    {
        cout << ex.what() << endl;
        getchar();
        //free();
        return -1;
    }

    return 0;
}
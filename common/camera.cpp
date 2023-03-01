#include <glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include <iostream>

using namespace glm;



Camera::Camera(GLFWwindow* window) : window(window) {
    //position = vec3(2,1,2.3);

    //center scene
    ///*
    Normal.Pos = vec3(-1.513,1.47,3.76);
    Normal.horizontalAngle = 1.548f;
    Normal.verticalAngle = -0.012f;
    
    
    //end to begin
    Normal.Pos = vec3(5.03,3.55,14.7);
    Normal.horizontalAngle = 3.132;
    Normal.verticalAngle = -0.15; 
    
    //center
    Normal.Pos = vec3(-6.3,4.33,5.3);
    Normal.horizontalAngle = 1.57;
    Normal.verticalAngle = 0.09;


    Select.Pos = vec3(5.68, 10.4, 5.7);
    Select.horizontalAngle = -3.177f;
    Select.verticalAngle = -1.304f;
     //*/

    /*
    position = vec3(5.65,0.912,7.123);
    horizontalAngle = 3.11f;
    verticalAngle = 0.155f;
    */
    //end  scene
    /*
    position = vec3(3.3,1.15,1.4);
    horizontalAngle = -0.189;
    verticalAngle = 0.06f;
    //*/
    
    //crystal
    /*
    position = vec3(6.1,2,6);
    horizontalAngle = -0.004;
    verticalAngle = 0.02f;
    //*/

    //front robot===
    /*
    position = vec3(5.4,1.5,4.0);
    horizontalAngle = 3.12;
    verticalAngle = 0.1f;
    //*/
    FoV = 88.0f;
    speed = 3.0f;
    mouseSpeed = 0.001f;
    fovSpeed = 2.0f;
}

void Camera::update(bool SelectMode) {
    // glfwGetTime is called only once, the first time this function is called
    
    Current = SelectMode ? &Select : & Normal;
    
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Task 5.4: right and up vectors of the camera coordinate system
    // use spherical coordinates
    vec3 direction(
        cos(Current->verticalAngle) * sin(Current->horizontalAngle),
        sin(Current->verticalAngle),
        cos(Current->verticalAngle) * cos(Current->horizontalAngle)
    );

    // Right vector
    vec3 right(
        sin(Current->horizontalAngle - 3.14f / 2.0f),
        0,
        cos(Current->horizontalAngle - 3.14f / 2.0f)
    );

    // Up vector
    vec3 up = cross(right, direction);

    // Task 5.5: update camera position using the direction/right vectors
    // Move forward

    if(!SelectMode){
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && Current->Pos.y >= 1.0) {
            Current->Pos += direction * deltaTime * speed;
        }
        // Move backward
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && Current->Pos.x >= -11.0 ) {
            Current->Pos -= direction * deltaTime * speed;
        }
        // Strafe right
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && Current->Pos.z <= 15.0) {
            Current->Pos += right * deltaTime * speed;
        }
        // Strafe left
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && Current->Pos.z >= 1.0) {
            Current->Pos -= right * deltaTime * speed;
        }

        
    }




    
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        printCamera();
    }


    // Task 5.7: construct projection and view matrices
    projectionMatrix = perspective(radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
    viewMatrix = lookAt(
        Current->Pos,
        Current->Pos + direction,
        up
    );
    
    lastTime = currentTime;
    
}

void Camera::onMouseMove(double xPos, double yPos)
{
    static double lastxPos = xPos;
    static double lastyPos = yPos;

    if (active) {
        Current->horizontalAngle += mouseSpeed * (lastxPos - xPos);
        Current->verticalAngle += mouseSpeed * (lastyPos - yPos);
    }
    lastxPos = xPos;
    lastyPos = yPos;
}


void Camera::printCamera() {
    std::cout << "x= " << Current->Pos.x << " y= " << Current->Pos.y << " z= " << Current->Pos.z << std::endl;
    std::cout << "hor= " << Current->horizontalAngle << " ver= " << Current->verticalAngle << " fov= " << FoV << std::endl;
}


void Camera::SelectionMode() {
    position = vec3(5.68, 10.4, 5.7);
    horizontalAngle =-3.177f;
    verticalAngle = -1.304f;
    

}

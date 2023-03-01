#include <Gl/glew.h>
#include <glfw3.h>
#include <iostream>
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include "light.h"

using namespace glm;

Light::Light(GLFWwindow* window, 
             glm::vec4 init_La,
             glm::vec4 init_Ld,
             glm::vec4 init_Ls,
             glm::vec3 init_position,
             float init_power) : window(window) {
    La = init_La;
    Ld = init_Ld;
    Ls = init_Ls;
    power = init_power;
    lightPosition_worldspace = init_position;

    // setting near and far plane affects the detail of the shadow
    nearPlane = 0,1;
    farPlane = 100.0;
    win = 5;

    targetPosition = glm::vec3(7.9, 0.0, 3.4);
    
    direction = normalize(targetPosition - lightPosition_worldspace);

    lightSpeed = 0.1f;


    projectionMatrix = ortho(-win, win, -win, win, nearPlane, farPlane);
    orthoProj = true;
}



void Light::update() {


   // Move across z-axis
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        lightPosition_worldspace -= lightSpeed * vec3(0.0, 0.0, 1.0);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        lightPosition_worldspace += lightSpeed * vec3(0.0, 0.0, 1.0);
    }
    // Move across x-axis
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        lightPosition_worldspace += lightSpeed * vec3(1.0, 0.0, 0.0);
    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        lightPosition_worldspace -= lightSpeed * vec3(1.0, 0.0, 0.0);
    }
    // Move across y-axis
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        lightPosition_worldspace += lightSpeed * vec3(0.0, 1.0, 0.0);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        lightPosition_worldspace -= lightSpeed * vec3(0.0, 1.0, 0.0);
    }
    
    

   
    


    // We have the direction of the light and the point where the light is looking at
    // We will use this information to calculate the "up" vector, 
    // just like we did with the camera

    direction = normalize(targetPosition - lightPosition_worldspace);
    //projectionMatrix = ortho(-win, win, -win, win, nearPlane, farPlane);
    

    // converting direction to cylidrical coordinates
    float x = direction.x;
    float y = direction.y;
    float z = direction.z;

    // We don't need to calculate the vertical angle
    
    float horizontalAngle;
    if (z > 0.0) horizontalAngle = atan(x/z);
    else if (z < 0.0) horizontalAngle = atan(x/z) + 3.1415f;
    else horizontalAngle = 3.1415f / 2.0f;

    // Right vector
    vec3 right(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f)
    );

    // Up vector
    vec3 up = cross(right, direction);
   
    viewMatrix = lookAt(
        lightPosition_worldspace,
        targetPosition,
        up
       
    );
    //*/

}


mat4 Light::lightVP() {
    return projectionMatrix * viewMatrix;
}



void Light::shaderCall(GLuint shaderProgram) {
    lightProps.LaLocation = glGetUniformLocation(shaderProgram, "light.La");
    lightProps.LdLocation = glGetUniformLocation(shaderProgram, "light.Ld");
    lightProps.LsLocation = glGetUniformLocation(shaderProgram, "light.Ls");
    lightProps.lightPositionLocation = glGetUniformLocation(shaderProgram, "light.lightPosition_worldspace");
    lightProps.lightPowerLocation = glGetUniformLocation(shaderProgram, "light.power");
    lightProps.lightVPLocation = glGetUniformLocation(shaderProgram, "lightVP");


}


void Light::upLoad() {
    glUniform4f(lightProps.LaLocation, La.r, La.g, La.b, La.a);
    glUniform4f(lightProps.LdLocation, Ld.r, Ld.g, Ld.b, Ld.a);
    glUniform4f(lightProps.LsLocation, Ls.r, Ls.g, Ls.b, Ls.a);
    glUniform3f(lightProps.lightPositionLocation, lightPosition_worldspace.x, lightPosition_worldspace.y,
        lightPosition_worldspace.z);
    glUniform1f(lightProps.lightPowerLocation, power);

    mat4 light_VP = lightVP();

    glUniformMatrix4fv(lightProps.lightVPLocation, 1, GL_FALSE, &light_VP[0][0]);

}



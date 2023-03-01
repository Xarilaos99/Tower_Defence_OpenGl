#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>


struct CurrentCamera {
    glm::vec3 Pos;
    float horizontalAngle;
    float verticalAngle;
};

class Camera {
public:
    GLFWwindow* window;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    // Initial position : on +Z
    glm::vec3 position;
    // Initial horizontal angle : toward -Z
    float horizontalAngle;
    // Initial vertical angle : none
    float verticalAngle;
    // Field of View
    float FoV;
    float speed; // units / second
    float mouseSpeed;
    float fovSpeed;

    bool active = true;
    //bool SelectMode = false;

    Camera(GLFWwindow* window);
    void update(bool SelectMode);
    void onMouseMove(double xPos, double yPos);
    void printCamera();
    void SelectionMode();
    CurrentCamera Normal, Select;
    CurrentCamera* Current;
};

#endif

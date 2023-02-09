#include <glm/glm.hpp>


struct LightProp {
    GLuint lightVPLocation;
    GLuint LaLocation;
    GLuint LdLocation;
    GLuint LsLocation;
    GLuint lightPositionLocation;
    GLuint lightPowerLocation;
};



class Light {

    public:
        Light(GLFWwindow* window,
            glm::vec4 init_La,
            glm::vec4 init_Ld,
            glm::vec4 init_Ls,
            glm::vec3 init_position,
            float init_power);

        void update();

        void shaderCall(GLuint shaderProgram);
        void upLoad();

        glm::mat4 lightVP();

        float nearPlane;
        float farPlane;
        float win;
        float power;
        glm::vec3 targetPosition;
        glm::vec3 lightPosition_worldspace;
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;


    private:
        GLFWwindow* window;

        

        glm::vec4 La;
        glm::vec4 Ld;
        glm::vec4 Ls;

       
       

        

        bool orthoProj;

        float lightSpeed;
        glm::vec3 direction;

        // Where the light will look at
        /*glm::vec3 targetPosition;*/

        LightProp lightProps;

        // Constructor
        
};

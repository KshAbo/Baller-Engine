#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "shader.hpp"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, glm::mat4& transform);

int HEIGHT = 1080;
int WIDTH = 1920;

const char* vertFilePath{"../src/shader/VertexShader.vert"};
const char* fragFilePath{"../src/shader/FragmentShader.frag"};

int main(){

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window;

    window = glfwCreateWindow(WIDTH, HEIGHT, "Window", NULL, NULL);
    glfwMakeContextCurrent(window);

    if(!window){
        glfwTerminate();
        return -1;
    }
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
            cout << "Failed to iniatialize GLAD" << endl;
            glfwTerminate();
            return 1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader myshader(vertFilePath, fragFilePath);

    GLfloat vertices[] = {
        // positions
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        0.0f,  0.5f, 0.0f // top 
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*(sizeof(float)), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(0);

    // create transformations
    glm::mat4 modelMat = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

    glm::mat4 viewMat = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

    glm::mat4 perspectiveMat = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

    myshader.use();
    GLuint modelLoc = glGetUniformLocation(myshader.ID, "model");
    GLuint viewLoc = glGetUniformLocation(myshader.ID, "view");
    GLuint perspectiveLoc = glGetUniformLocation(myshader.ID, "perspective");
    perspectiveMat = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    while(!glfwWindowShouldClose(window)){

        glClearColor(0.8, 0.7, 0.9,1);
        glClear(GL_COLOR_BUFFER_BIT);




        processInput(window, modelMat);
        const GLfloat radius = 3.0f;
        GLfloat camX = sin(glfwGetTime())*radius;
        GLfloat camZ = cos(glfwGetTime())*radius;
        viewMat = glm::lookAt(glm::vec3(camX,0.0,camZ), glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,1.0,0.0));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
        glUniformMatrix4fv(perspectiveLoc, 1, GL_FALSE, glm::value_ptr(perspectiveMat));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, glm::mat4& transform)
{
    const GLfloat cameraSpeed = 0.005f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        transform = glm::translate(transform, glm::vec3(0, 1*cameraSpeed, 0));
    {
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        transform = glm::translate(transform, glm::vec3(0, -1*cameraSpeed, 0));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        transform = glm::translate(transform, glm::vec3(-1*cameraSpeed, 0, 0));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        transform = glm::translate(transform, glm::vec3(1*cameraSpeed, 0, 0));
    }
}

#include <fstream>
#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include <sstream>
#include <string>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int HEIGHT = 1080;
int WIDTH = 1920;

const char* vertFilePath{"../src/VertexShader.vert"};
const char* fragFilePath{"../src/FragmentShader.glsl"};

int main(){

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window;

    window = glfwCreateWindow(HEIGHT, WIDTH, "Window", NULL, NULL);
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

    ifstream vertSourceFile, fragSourceFile;
    stringstream vertStream, fragStream;
    string vertSource, fragSource;

    vertSourceFile.open(vertFilePath);
    fragSourceFile.open(fragFilePath);
    
    vertStream << vertSourceFile.rdbuf();
    fragStream << fragSourceFile.rdbuf();

    vertSource = vertStream.str();
    fragSource = fragStream.str();

    const char* vertCode = vertSource.c_str();
    const char* fragCode = fragSource.c_str();

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertCode, NULL);
    glShaderSource(fragmentShader, 1, &fragCode, NULL);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    unsigned int ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    float vertices[] = {
        // positions
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        0.0f,  0.5f, 0.0f // top 
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*(sizeof(float)), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window)){

        glClearColor(0.1, 0.1, 0.5,1);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(ID);
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

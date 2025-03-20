#include "raylib.h"
#include "camera.hpp"
#include <cmath>
using namespace std;

#define WIDTH 1920
#define HEIGHT 1080

int main (int argc, char *argv[]) {

    InitWindow(WIDTH, HEIGHT, "Baller Engine");

    Model cube = LoadModel("../obj/monkey.obj");
    Shader shader = LoadShader("./shader/vertex.vert", "./shader/fragment.frag");

    cube.materials[0].shader = shader;
    
    HideCursor();

    BallerEngine::Camera camera({0, 10, 10}, {0, 0, 0}, {0, 1, 0}, 45);

    SetTargetFPS(240);
    
    DisableCursor();

    Vector3 lightPosition = (Vector3){3*sin(GetFrameTime()), 0.0f, 3*cos(GetFrameTime())};

    while(!WindowShouldClose()){

        lightPosition = (Vector3){static_cast<float>(3*sin(GetTime())), 0.0f, static_cast<float>(3*cos(GetTime()))};
        SetShaderValue(shader, GetShaderLocation(shader, "lightPosition"), &lightPosition, SHADER_UNIFORM_VEC3);

        BeginDrawing();

            ClearBackground(GRAY);

            BeginMode3D(camera.camera);

            DrawCube(lightPosition, 0.5f, 0.5f, 0.5f, WHITE);
            DrawModel(cube, Vector3{0,0,0}, 1.0, WHITE);

            camera.updateCamera();
            DrawGrid(100, 1);
            EndMode3D();

            DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}

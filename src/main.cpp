#include "raylib.h"
#include "camera.hpp"
#include "rcamera.h"
#include <iostream>
using namespace std;

#define WIDTH 1920
#define HEIGHT 1080

int main (int argc, char *argv[]) {

    InitWindow(WIDTH, HEIGHT, "Baller Engine");
    Model cube = LoadModel("../obj/monkey.obj");
    
    HideCursor();

    BallerEngine::Camera camera({0, 10, 10}, {0, 0, 0}, {0, 1, 0}, 45);

    SetTargetFPS(240);
    
    DisableCursor();

    while(!WindowShouldClose()){

        Vector3 cubePosition = {0.0, 0.0, 0.0};

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera.camera);

            // DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
            // DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
            DrawModel(cube, Vector3{0,0,0}, 1.0, BLUE);
            DrawGrid(10, 1);

            camera.updateCamera();
            EndMode3D();

            DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}

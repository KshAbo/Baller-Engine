#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include "iostream"
#include <cmath>
#include <vector>

#include "camera.hpp"
#include "physicsEngine.hpp"


using namespace std;

#define WIDTH 1920
#define HEIGHT 1080

PhysicsEngine* PhysicsEngine::engine = nullptr;

int main (int argc, char *argv[]) {

    InitWindow(WIDTH, HEIGHT, "Baller Engine");

    Model cube = LoadModel("../obj/monkey.obj");
    Shader shader = LoadShader("./shader/vertex.vert", "./shader/fragment.frag");

    cube.materials[0].shader = shader;
    
    HideCursor();

    BallerEngine::Camera camera({0, 10, 10}, {0, 0, 0}, {0, 1, 0}, 45);


    SetTargetFPS(240);
    
    DisableCursor();

    Vector3 lightPosition = (Vector3){10*sin(GetFrameTime()), 0.0f, 10*cos(GetFrameTime())};
    BoundingBox box;

    // Create the physics bodies
	std::vector<PhysicsObject> physics_objects;
	physics_objects = {
		PhysicsObject( {0,0,0}, {0,0,0}, {8,0.5,8}, CUBE, STATIC, 0.0, GRAY ) // Floor
	};

	// Create a group of random dynamic objects
	for (size_t i = 0; i < 30; i++) {
		const std::vector<Color> colors {
			MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE,
			RED, GOLD, LIME, BLUE, VIOLET, PINK, YELLOW,
			GREEN, SKYBLUE, PURPLE
		};

		physics_objects.push_back(
			PhysicsObject(
				{(float)GetRandomValue(-5, 5), (float)GetRandomValue(10, 15), (float)GetRandomValue(-5, 5)},
				{(float)GetRandomValue(-3, 3), (float)GetRandomValue(-3, 3), (float)GetRandomValue(-3, 3)},
				{(float)GetRandomValue(1, 3), (float)GetRandomValue(1, 3), (float)GetRandomValue(1, 3)},
				(Shape)GetRandomValue(0, 1),
				DYNAMIC,
				1.0,
				colors[ GetRandomValue(0, colors.size() - 1) ]
			)
		);
	}


    while(!WindowShouldClose()){

        lightPosition = (Vector3){static_cast<float>(5*sin(GetTime())), 0.0f, static_cast<float>(5*cos(GetTime()))};
        SetShaderValue(shader, GetShaderLocation(shader, "lightPosition"), &lightPosition, SHADER_UNIFORM_VEC3);

        box = GetModelBoundingBox(cube);

        //Rendering
        BeginDrawing();

            ClearBackground(GRAY);

            BeginMode3D(camera.camera);

            PhysicsEngine::getEngine()->update();

            DrawCube(lightPosition, 0.5f, 0.5f, 0.5f, WHITE);
            DrawModel(cube, Vector3{0,0,0}, 1.0, WHITE);
            DrawBoundingBox(box, BLUE);

            for (auto& object : physics_objects) {
                object.render();
            }

            camera.updateCamera();
            DrawGrid(100, 1);
            EndMode3D();

            DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadModel(cube);
    UnloadShader(shader);

    for (auto& object : physics_objects) {
        object.unload();
    }

    CloseWindow();
    
    return 0;
}

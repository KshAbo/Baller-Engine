#include <arpa/inet.h>
#include <cmath>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "camera.hpp"
#include "gameObjects.hpp"
#include "physicsEngine.hpp"
#include "raylib/raylib.h"
#include "raylib/raymath.h"

using namespace std;

#define WIDTH 1920
#define HEIGHT 1080

PhysicsEngine *PhysicsEngine::engine = nullptr;

int main(int argc, char *argv[]) {

  InitWindow(WIDTH, HEIGHT, "Baller Engine");
  SetTargetFPS(240);
  HideCursor();
  DisableCursor();

  Model cube = LoadModel("../obj/monkey.obj");
  Shader shader1 =
      LoadShader("./shader/vertex.vert", "./shader/fragment1.frag");
  Shader shader2 =
      LoadShader("./shader/vertex.vert", "./shader/fragment2.frag");
  Shader shader3 = LoadShader("./shader/vertex.vert", "./shader/ball.frag");

  cube.materials[0].shader = shader1;

  BallerEngine::Camera camera({0, 10, 10}, {0, 0, 0}, 45);

  Vector3 lightPosition =
      (Vector3){10 * sin(GetFrameTime()), 0.0f, 10 * cos(GetFrameTime())};
  Vector3 cameraPosition;
  BoundingBox box;

  // Drawing the world
  PhysicsObject floor({0, 0, 0}, {0, 0, 0}, {200, 0.5, 150}, CUBE, STATIC, 0.0,
                      GRAY); // Floor
  PhysicsObject wall1({-100, 50, 0}, {0, 0, 0}, {0.5, 100, 150}, CUBE, STATIC,
                      0.0, GRAY); // Wall
  PhysicsObject wall2({100, 50, 0}, {0, 0, 0}, {0.5, 100, 150}, CUBE, STATIC,
                      0.0, GRAY); // Wall
  PhysicsObject wall3({0, 50, 75}, {0, 0, 0}, {200, 100, 0.5}, CUBE, STATIC,
                      0.0, GRAY); // Wall
  PhysicsObject wall4({0, 50, -75}, {0, 0, 0}, {200, 100, 0.5}, CUBE, STATIC,
                      0.0, GRAY); // Wall
  floor.attachShader(shader1);
  wall1.attachShader(shader2);
  wall2.attachShader(shader2);
  wall3.attachShader(shader2);
  wall4.attachShader(shader2);

  Car car(Vector3{10, 0, 0});
  Ball ball(Vector3{0, 0, 0});
  ball.attachShader(shader3);

  // Socket setup
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(65432);
  inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
  connect(sock, (sockaddr *)&server_address, sizeof(server_address));

  btVector3 carLoc;

  while (!WindowShouldClose()) {

    int radius = 5;
    lightPosition = (Vector3){static_cast<float>(radius * sin(GetTime())), 0.0f,
                              static_cast<float>(radius * cos(GetTime()))};
    cameraPosition = car.getCameraPosition();
    SetShaderValue(shader3, GetShaderLocation(shader3, "lightPosition"),
                   &lightPosition, SHADER_UNIFORM_VEC3);
    SetShaderValue(shader1, GetShaderLocation(shader1, "cameraPosition"),
                   &cameraPosition, SHADER_UNIFORM_VEC3);
    SetShaderValue(shader2, GetShaderLocation(shader2, "cameraPosition"),
                   &cameraPosition, SHADER_UNIFORM_VEC3);

    box = GetModelBoundingBox(cube);

    // Rendering
    BeginDrawing();

    ClearBackground(DARKBLUE);

    BeginMode3D(car.camera.camera);

    DrawCube(lightPosition, 0.5f, 0.5f, 0.5f, WHITE);
    DrawModel(cube, Vector3{0, 0, 0}, 1.0, WHITE);
    DrawBoundingBox(box, BLUE);

    PhysicsEngine::getEngine()->update(GetFPS());
    floor.render();
    wall1.render();
    wall2.render();
    wall3.render();
    wall4.render();
    car.update(sock);
    car.render();
    ball.render();

    camera.updateCamera();
    DrawGrid(100, 1);

    EndMode3D();

    DrawFPS(10, 10);
    btVector3 carLoc = car.getOrigin();
    DrawText(TextFormat("Car Coordinates: %i %i %i", (int)carLoc.x(),
                        (int)carLoc.y(), (int)carLoc.z()),
             10, 30, 20, RED);

    EndDrawing();
  }

  UnloadModel(cube);
  UnloadShader(shader1);
  UnloadShader(shader2);
  UnloadShader(shader3);
  close(sock);

  CloseWindow();

  return 0;
}

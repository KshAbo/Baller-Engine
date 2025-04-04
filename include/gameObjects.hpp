#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

#include <iostream>

#include "bullet/btBulletDynamicsCommon.h"
#include "camera.hpp"
#include "controller.hpp"
#include "physicsEngine.hpp"
#include "raylib/raylib.h"

// Collider shape
enum Shape {
  CUBE,
  SPHERE,
};

enum PhysicsType {
  STATIC,
  DYNAMIC,
};

class PhysicsObject {
protected:
  btRigidBody *body;
  btCollisionShape *collider_shape;
  Model model;

public:
  Color color;

  PhysicsObject(Vector3 position = {0, 0, 0}, Vector3 rotation = {0, 0, 0},
                Vector3 size = {1, 1, 1}, Shape shape = CUBE,
                PhysicsType type = STATIC, float mass = 0, Color color = WHITE);
  void attachShader(Shader shader);
  void render();
  ~PhysicsObject();
};

// Car class
class Car : public PhysicsObject {

  Controller controller;

  void stabilize();
  void resetOrientation();

public:
  GameCamera camera;

  Car(const Vector3 pos = Vector3{0, 0, 0},
      const Vector3 rot = Vector3{0, 0, 0}, Input input = GESTURE);

  bool isOnGround();
  void applyForce(const btVector3 &force);
  void applyImpulse(const btVector3 &impulse);
  void applyTorque(const btVector3 &torque);
  btVector3 getForwardVector();
  btVector3 getOrigin();
  btTransform getTransform();
  btVector3 getUpVector();
  Vector3 getCameraPosition();
  void update(int socket);
};

// Ball class
class Ball : public PhysicsObject {
public:
  Ball(const Vector3 pos)
      : PhysicsObject({0, 10, 0}, {0, 0, 0}, {5, 5, 5}, SPHERE, DYNAMIC, 200,
                      WHITE) {}
};

#endif // !GAME_OBJECTS_H

#define CATCH_CONFIG_MAIN

#include "gameObjects.hpp"
#include "catch.hpp"
#include "raylib/raylib.h"

PhysicsEngine *PhysicsEngine::engine = nullptr;

TEST_CASE("Game Objects", "[Game Objects]") {
  InitWindow(100, 100, "GameObjectsTest");

  SECTION("Get Up Vector of Car") {
    Car car(Vector3{0, 0, 0});
    REQUIRE(car.getUpVector() == btVector3(0, 1, 0));
  }

  SECTION("Ground Test") {
    Car car1(Vector3{0, 0, 0});
    REQUIRE(car1.isOnGround() == 1);
    Car car2(Vector3({0, 200, 0}));
    REQUIRE(car2.isOnGround() == 0);
  }

  SECTION("Force Test") {
    Car car(Vector3{0, 0, 0});
    car.applyForce(btVector3(100, 100, 100));
    btVector3 initialPos = car.getOrigin();
    PhysicsEngine::getEngine()->stepSimulation(2);
    btVector3 finalPos = car.getOrigin();
    REQUIRE(initialPos != finalPos);
  }

  SECTION("Impulse Test") {
    Car car(Vector3{0, 0, 0});
    car.applyImpulse(btVector3(100, 100, 100));
    btVector3 initialPos = car.getOrigin();
    PhysicsEngine::getEngine()->stepSimulation(2);
    btVector3 finalPos = car.getOrigin();
    REQUIRE(initialPos != finalPos);
  }

  SECTION("Torque Test") {
    Car car(Vector3{0, 0, 0});
    car.applyTorque(btVector3(100, 100, 100));
    btQuaternion initialRot = car.getTransform().getRotation();
    PhysicsEngine::getEngine()->stepSimulation(2);
    btQuaternion finalRot = car.getTransform().getRotation();
    REQUIRE(initialRot != finalRot);
  }
  CloseWindow();
}

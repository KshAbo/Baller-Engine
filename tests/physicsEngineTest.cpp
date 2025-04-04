#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "gameObjects.hpp"
#include "raylib/raylib.h"

PhysicsEngine *PhysicsEngine::engine = nullptr;
TEST_CASE("PhysicsEngine Class", "[PhysicsEngine]") {

  REQUIRE(PhysicsEngine::getEngine() != nullptr);
  REQUIRE(PhysicsEngine::getEngine()->world != nullptr);

  SECTION("Adding rigid body to world") {
    btTransform transform;
    transform.setIdentity();
    btDefaultMotionState *motionState = new btDefaultMotionState(transform);
    btCollisionShape *collisionShape = new btSphereShape(1);
    btRigidBody body(0, motionState, collisionShape);
    PhysicsEngine::getEngine()->addRigidBody(&body);
    REQUIRE(
        PhysicsEngine::getEngine()->world->getCollisionObjectArray().size() ==
        1);
    delete motionState;
    delete collisionShape;
  }
}

#ifndef PHYSICS_H
#define PHYSICS_H

#include <btBulletDynamicsCommon.h>

class PhysicsEngine {

  PhysicsEngine() {
    collisionConfig = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfig);
    broadphase = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver();
    world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver,
                                        collisionConfig);
    world->setGravity(btVector3(0, -9.8f, 0));
  }

public:
  static PhysicsEngine *engine;

  btDefaultCollisionConfiguration *collisionConfig;
  btCollisionDispatcher *dispatcher; // Collision dispatcher, handles collision
  btBroadphaseInterface
      *broadphase; // Broadphase interface, detects overlapping objects
  btSequentialImpulseConstraintSolver *solver; // Constraint solver
  btDiscreteDynamicsWorld *world; // The world where physics takes place

  static PhysicsEngine *getEngine() {
    if (!engine) {
      engine = new PhysicsEngine();
    }
    return engine;
  }

  void stepSimulation(float deltaTime) { world->stepSimulation(deltaTime, 10); }

  void addRigidBody(btRigidBody *body) { world->addRigidBody(body); }

  void update(int FPS) {
    if (FPS > 0) {
      world->stepSimulation(1.0 / FPS, 10); // Update physics
      return;
    }
    world->stepSimulation(1.0 / 100, 10); // Update physics
  }

  ~PhysicsEngine() {

    // Remove rigid bodies
    for (int i = world->getNumCollisionObjects() - 1; i >= 0; i--) {
      btCollisionObject *obj = world->getCollisionObjectArray()[i];
      btRigidBody *body = btRigidBody::upcast(obj);
      if (body && body->getMotionState()) {
        delete body->getMotionState();
      }
      world->removeCollisionObject(obj);
      delete obj;
    }

    delete world;
    delete solver;
    delete broadphase;
    delete dispatcher;
    delete collisionConfig;
  }
};

#endif // !PHYSICS_H

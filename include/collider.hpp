#ifndef COLLIDER_H
#define COLLIDER_H

#include "raylib/raylib.h"
#include "bullet/btBulletDynamicsCommon.h"
#include <iostream>

// Base collider class
class Collider {
protected:
    btRigidBody* body;
    btCollisionShape* shape;
    btDefaultMotionState* motionState;

public:
    Collider(btCollisionShape* shape, float mass, const btVector3& pos) : shape(shape) {
        motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos));
        btVector3 inertia(0, 0, 0);
        shape->calculateLocalInertia(mass, inertia);

        btRigidBody::btRigidBodyConstructionInfo bodyCI(mass, motionState, shape, inertia);
        std::cout << "body mass " << bodyCI.m_mass << std::endl;
        body = new btRigidBody(bodyCI);
        std::cout << "created body " << body->getMotionState() << " " << body->getMass() << " " << body->getCollisionShape() << std::endl;
    }

    btRigidBody* getBody() { return body; }

    virtual ~Collider() {
        delete body;
        delete motionState;
        delete shape;
    }
};

// Car class
class Car {
public:
    Collider collider;
    Car(const btVector3& pos) : collider(new btBoxShape(btVector3(1.0f, 0.5f, 2.0f)), 1200.0f, pos) {}
};

// Ball class
class Ball {
public:
    Collider collider;
    Ball(const btVector3& pos) : collider(new btSphereShape(0.3f), 0.4f, pos) {}
};

#endif // !COLLIDER_H



#ifndef PHYSICS_H
#define PHYSICS_H

#include <btBulletDynamicsCommon.h>
#include "collider.hpp"

#define FPS 240

class PhysicsEngine {

    PhysicsEngine() {
        collisionConfig = new btDefaultCollisionConfiguration();
        dispatcher = new btCollisionDispatcher(collisionConfig);
        broadphase = new btDbvtBroadphase();
        solver = new btSequentialImpulseConstraintSolver();

        world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
        world->setGravity(btVector3(0, -9.8f, 0));
    }

public:

    static PhysicsEngine* engine;
    
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* dispatcher; // Collision dispatcher, handles collision
    btBroadphaseInterface* broadphase; // Broadphase interface, detects overlapping objects
    btSequentialImpulseConstraintSolver* solver; // Constraint solver
    btDiscreteDynamicsWorld* world; // The world where physics takes place
    btAlignedObjectArray<btCollisionShape*> collision_shapes; // Keeps track of collision shapes
    
    static PhysicsEngine* getEngine(){
        if(!engine){
            engine = new PhysicsEngine();
        }
        return engine;
    }

    void addObject(btRigidBody* body) { world->addRigidBody(body); }

    void stepSimulation(float deltaTime) { world->stepSimulation(deltaTime, 10); }

    void addColliderShape(btCollisionShape* shape){
        collision_shapes.push_back(shape);
    }

    void addRigidBody(btRigidBody* body){
        world->addRigidBody(body);
    }

    void checkCollisions() {
        // Simplify collision detection for now
        world->updateAabbs();
    }

    void update() {
        world->stepSimulation(1.0 / float(FPS), 10); // Update physics
    }

    ~PhysicsEngine() {

        // Remove rigid bodies
        for (int i = world->getNumCollisionObjects() - 1; i >= 0; i--) {
            btCollisionObject* obj = world->getCollisionObjectArray()[i];
            btRigidBody* body = btRigidBody::upcast(obj);
            if (body && body->getMotionState()) {
                delete body->getMotionState();
            }
            world->removeCollisionObject(obj);
            delete obj;
        }

        // Delete collision shapes
        for (int i = 0; i < collision_shapes.size(); i++) {
            btCollisionShape* shape = collision_shapes[i];
            collision_shapes[i] = 0;
            delete shape;
        }

        delete world;
        delete solver;
        delete broadphase;
        delete dispatcher;
        delete collisionConfig;
    }
};

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
    private:
        btRigidBody* body;
        btCollisionShape* collider_shape;
        Model model;

    public:
        Color color;

        PhysicsObject(
                Vector3 position = {0,0,0},
                Vector3 rotation = {0,0,0},
                Vector3 size = {1,1,1},
                Shape shape = CUBE,
                PhysicsType type = STATIC,
                float mass = 0,
                Color color = WHITE
                ) {
            this->color = color;

            if (shape == CUBE) {
                collider_shape = new btBoxShape(
                        btVector3( btScalar(size.x/2.0), btScalar(size.y/2.0), btScalar(size.z/2.0) )
                        );

                model = LoadModelFromMesh( GenMeshCube(size.x, size.y, size.z) );
            } else if (shape == SPHERE) {
                collider_shape = new btSphereShape( btScalar(size.x) );

                model = LoadModelFromMesh( GenMeshSphere(size.x, 8, 16) );
            }

            // collision_shapes.push_back(collider_shape);
            PhysicsEngine::getEngine()->addColliderShape(collider_shape);

            // Set location and rotation
            btTransform transform;
            transform.setIdentity();
            transform.setOrigin( btVector3(position.x, position.y, position.z) );
            transform.setRotation(
                    btQuaternion( btScalar(rotation.z), btScalar(rotation.y), btScalar(rotation.x) )
                    );

            btScalar object_mass(mass); // Set the object's mass

            // Calculate local inertia for dynamic objects
            btVector3 local_inertia(0, 0, 0);
            if (type == DYNAMIC || mass != 0.0) // Objects with 0.0 mass are static
                collider_shape->calculateLocalInertia(mass, local_inertia);

            btDefaultMotionState* motion_state = new btDefaultMotionState(transform);

            btRigidBody::btRigidBodyConstructionInfo rb_info(object_mass, motion_state, collider_shape, local_inertia);
            body = new btRigidBody(rb_info);

            PhysicsEngine::getEngine()->addRigidBody(body);
        }

        void render() {
            const float radian_scale = 57.296;
            btTransform trans;

            // Get the transform of the body
            if (body && body->getMotionState())
                body->getMotionState()->getWorldTransform(trans);
            else
                return;


            // Get position from transform
            Vector3 position = {
                float(trans.getOrigin().getX()),
                float(trans.getOrigin().getY()),
                float(trans.getOrigin().getZ())
            };

            // Get rotation from transform
            btQuaternion quat = trans.getRotation();

            Vector3 axis = {
                float(quat.getAxis().getX()),
                float(quat.getAxis().getY()),
                float(quat.getAxis().getZ())
            };
            float angle = float( quat.getAngle() ) * radian_scale; // Convert radians to degrees

            // Render model
            DrawModelEx(model, position, axis, angle, {1,1,1}, color);
            DrawModelWiresEx(
                    model, position, axis, angle, {1,1,1},
                    {(unsigned char)(color.r/2), (unsigned char)(color.g/2), (unsigned char)(color.b/2), color.a}
                    );
        }

        void unload() {
            UnloadModel(model);
        }
};

#endif // !PHYSICS_H

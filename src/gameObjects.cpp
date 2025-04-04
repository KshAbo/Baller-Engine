#include "gameObjects.hpp"

PhysicsObject::PhysicsObject(Vector3 position, Vector3 rotation, Vector3 size,
                             Shape shape, PhysicsType type, float mass,
                             Color color) {
  this->color = color;

  if (shape == CUBE) {
    collider_shape =
        new btBoxShape(btVector3(btScalar(size.x / 2.0), btScalar(size.y / 2.0),
                                 btScalar(size.z / 2.0)));

    model = LoadModelFromMesh(GenMeshCube(size.x, size.y, size.z));
  } else if (shape == SPHERE) {
    collider_shape = new btSphereShape(btScalar(size.x));

    model = LoadModelFromMesh(GenMeshSphere(size.x, 8, 16));
  }

  // Set location and rotation
  btTransform transform;
  transform.setIdentity();
  transform.setOrigin(btVector3(position.x, position.y, position.z));
  transform.setRotation(btQuaternion(btScalar(rotation.z), btScalar(rotation.y),
                                     btScalar(rotation.x)));

  btScalar object_mass(mass);

  // Calculate local inertia for dynamic objects
  btVector3 local_inertia(0, 0, 0);
  if (type == DYNAMIC || mass != 0.0) // Objects with 0.0 mass are static
    collider_shape->calculateLocalInertia(mass, local_inertia);

  btDefaultMotionState *motion_state = new btDefaultMotionState(transform);

  btRigidBody::btRigidBodyConstructionInfo rb_info(
      object_mass, motion_state, collider_shape, local_inertia);
  body = new btRigidBody(rb_info);

  PhysicsEngine::getEngine()->addRigidBody(body);
}

void PhysicsObject::attachShader(Shader shader) {
  model.materials[0].shader = shader;
}

void PhysicsObject::render() {
  const float radian_scale = 57.296;
  btTransform trans;

  // Get the transform of the body
  if (body && body->getMotionState())
    body->getMotionState()->getWorldTransform(trans);
  else
    return;

  // Get position from transform
  Vector3 position = {float(trans.getOrigin().getX()),
                      float(trans.getOrigin().getY()),
                      float(trans.getOrigin().getZ())};

  // Get rotation from transform
  btQuaternion quat = trans.getRotation();

  Vector3 axis = {float(quat.getAxis().getX()), float(quat.getAxis().getY()),
                  float(quat.getAxis().getZ())};
  float angle =
      float(quat.getAngle()) * radian_scale; // Convert radians to degrees

  // Render model
  DrawModelEx(model, position, axis, angle, {1, 1, 1}, color);
  DrawModelWiresEx(model, position, axis, angle, {1, 1, 1},
                   {(unsigned char)(color.r / 2), (unsigned char)(color.g / 2),
                    (unsigned char)(color.b / 2), color.a});
}

PhysicsObject::~PhysicsObject() { UnloadModel(model); }

Car::Car(const Vector3 pos)
    : PhysicsObject(pos, {0, 0, 0}, {4.5, 2, 3}, CUBE, DYNAMIC, 500, RED),
      controller(this), camera(Vector3{pos.x - 1, pos.y + 1, pos.z},
                               Vector3{pos.x, pos.y + 1, pos.z}, this) {
  body->setFriction(0.0f);
  body->setRollingFriction(0.0f);
  body->setActivationState(DISABLE_DEACTIVATION);
  body->setDamping(0.7, 0.8);
  body->setGravity(btVector3(0, -20, 0));
}

bool Car::isOnGround() {
  btTransform transform;
  body->getMotionState()->getWorldTransform(transform);
  return (transform.getOrigin().getY() < 1.1) ? true : false;
}

void Car::applyForce(const btVector3 &force) { body->applyCentralForce(force); }

void Car::applyImpulse(const btVector3 &impulse) {
  body->applyCentralImpulse(impulse);
}

void Car::applyTorque(const btVector3 &torque) { body->applyTorque(torque); }

btVector3 Car::getForwardVector() {
  btTransform transform;
  body->getMotionState()->getWorldTransform(transform);
  btVector3 forward = transform.getBasis() * btVector3{1, 0, 0};
  forward.normalize();
  return forward;
}

btVector3 Car::getOrigin() { return body->getCenterOfMassPosition(); }

btTransform Car::getTransform() {
  btTransform trans;
  body->getMotionState()->getWorldTransform(trans);
  return trans;
}

btVector3 Car::getUpVector() {
  btTransform transform;
  body->getMotionState()->getWorldTransform(transform);
  btVector3 up = transform.getBasis() * btVector3{0, 1, 0};
  up.normalize();
  return up;
}

Vector3 Car::getCameraPosition() { return camera.camera.position; }

void Car::update(int socket) {
  stabilize();
  camera.updateCamera();
  btVector3 up = getUpVector();
  btScalar dotProduct = up.dot(btVector3(0, 1, 0));
  if (dotProduct < 0) {
    resetOrientation();
  }
  controller.update(socket);
}

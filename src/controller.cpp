#include "controller.hpp"
#include "gameObjects.hpp"

void Controller::accelerate() {

  btVector3 forward = car->getForwardVector();
  car->applyForce(forward * 15000);
}

void Controller::brake() {

  btVector3 backward = -(car->getForwardVector());
  car->applyForce(backward * 15000);
}

void Controller::steerLeft() {
  btVector3 torque(0, -5000, 0);
  car->applyTorque(torque);
}

void Controller::steerRight() {
  btVector3 torque(0, 5000, 0);
  car->applyTorque(torque);
}

void Controller::jump() {
  if (car->isOnGround()) {
    btVector3 up = car->getUpVector();
    car->applyImpulse(up * 500);
  }
}

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "bullet/btBulletDynamicsCommon.h"
#include "iostream"
#include "raylib/raylib.h"

#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

enum Input {
  KEYBOARD,
  GESTURE,
};

class Car;
class Controller {
  Input input;

public:
  Controller(class Car *car, Input in = GESTURE) : car(car) { input = in; }

  void update(int socket) {
    if (input == GESTURE) {
      char buffer[1024] = {0}; // Initialize buffer to avoid garbage values
      std::string currentGesture = "stop";

      ssize_t bytes_received = read(socket, buffer, sizeof(buffer) - 1);
      if (bytes_received > 0) {
        buffer[bytes_received] = '\0'; // Null-terminate the received string
        currentGesture = std::string(buffer);

        // Safely remove trailing newlines or spaces
        size_t end = currentGesture.find_last_not_of("\r\n ");
        if (end != std::string::npos) {
          currentGesture = currentGesture.substr(0, end + 1);
        }
      } else if (bytes_received == 0) {
        std::cerr << "Client disconnected." << std::endl;
        return;
      } else {
        perror("Read error");
        return;
      }

      if (currentGesture == "forward") {
        accelerate();
      } else if (currentGesture == "left") {
        steerRight();
      } else if (currentGesture == "right") {
        steerLeft();
      } else if (currentGesture == "brake") {
        brake();
      } else if (currentGesture == "jump") {
        jump();
      }
    } else if (input == KEYBOARD) {
      if (IsKeyDown(KEY_UP)) {
        accelerate();
      }
      if (IsKeyDown(KEY_DOWN)) {
        brake();
      }
      if (IsKeyDown(KEY_LEFT)) {
        steerRight();
      }
      if (IsKeyDown(KEY_RIGHT)) {
        steerLeft();
      }
      if (IsKeyDown(KEY_SPACE)) {
        jump();
      }
    }
  }

private:
  Car *car;

  void accelerate();
  void steerRight();
  void steerLeft();
  void brake();
  void jump();
};

#endif // !CONTROLLER_H

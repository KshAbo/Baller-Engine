#ifndef CAMERA_H
#define CAMERA_H

#include "raylib/raylib.h"

class Car;
namespace BallerEngine {

class Camera {

  Vector3 m_position;
  Vector3 m_target;
  Vector3 m_camera_left;

  Vector3 _getCameraForward();
  Vector3 _getCameraRight();
  void _moveForward();
  void _moveSideward();
  void _moveUpward();
  void _panVertical();
  void _panHorizontal();

  friend class CameraTest;

public:
  ::Camera camera;
  Camera(Vector3 position, Vector3 target, float fov = 45.0f);
  void updateCamera();
};

}; // namespace BallerEngine

class GameCamera {

  Vector3 m_position;
  Vector3 m_target;
  Vector3 m_camera_left;
  Car *car;

public:
  ::Camera camera;
  GameCamera(Vector3 position, Vector3 target, Car *car);
  void updateCamera();
};

#endif // !CAMERA_H

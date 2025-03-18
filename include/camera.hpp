#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"
#include "raymath.h"

namespace BallerEngine{

    class Camera{

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

        public:

            ::Camera camera;
            Camera(Vector3 position, Vector3 target, Vector3 up, float fov);
            void updateCamera();

    };
};

#endif // !CAMERA_H

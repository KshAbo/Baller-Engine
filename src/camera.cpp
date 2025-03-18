#include "camera.hpp"
#include <iostream>

BallerEngine::Camera::Camera(Vector3 position, Vector3 target, Vector3 up, float fov){

    camera.position = position;
    camera.target = target;
    camera.projection = CAMERA_PERSPECTIVE;
    camera.fovy = fov;
    camera.up = Vector3{0.0f, 1.0f, 0.0f};

}

Vector3 BallerEngine::Camera::_getCameraForward(){
    return Vector3Normalize(Vector3Subtract(camera.target, camera.position));
}

Vector3 BallerEngine::Camera::_getCameraRight(){
    return Vector3Normalize(Vector3CrossProduct(_getCameraForward(), camera.up));
}

void BallerEngine::Camera::_moveForward(){
    float sensitivity = 0.05f;
    Vector3 forward = Vector3Scale(_getCameraForward(), sensitivity);
    if(IsKeyDown(KEY_W)){
        camera.target = Vector3Add(camera.target, forward);
        camera.position = Vector3Add(camera.position, forward);
    }
    if(IsKeyDown(KEY_S)){
        camera.target = Vector3Subtract(camera.target, forward);
        camera.position = Vector3Subtract(camera.position, forward);
    }
}

void BallerEngine::Camera::_moveSideward(){
    float sensitivity = 0.05f;
    Vector3 right = Vector3Scale(_getCameraRight(), sensitivity);
    if(IsKeyDown(KEY_D)){
        camera.target = Vector3Add(camera.target, right);
        camera.position = Vector3Add(camera.position, right);
    }
    if(IsKeyDown(KEY_A)){
        camera.target = Vector3Subtract(camera.target, right);
        camera.position = Vector3Subtract(camera.position, right);
    }
}

void BallerEngine::Camera::_moveUpward(){
    float sensitivity = 0.05f;
    if(IsKeyDown(KEY_E)){
        camera.target = Vector3Add(camera.target, Vector3Scale(camera.up, sensitivity));
        camera.position = Vector3Add(camera.position, Vector3Scale(camera.up, sensitivity));
    }
    if(IsKeyDown(KEY_Q)){
        camera.target = Vector3Subtract(camera.target, Vector3Scale(camera.up, sensitivity));
        camera.position = Vector3Subtract(camera.position, Vector3Scale(camera.up, sensitivity));
    }
}

void BallerEngine::Camera::_panVertical(){
    float sensitivity = 0.05f;
    float deltaY = GetMouseDelta().y;
    if(deltaY != 0.0f){
        camera.target = Vector3Add(camera.target, Vector3Scale(camera.up, -deltaY*sensitivity));
        // camera.up = Vector3Normalize(Vector3CrossProduct(_getCameraRight(), _getCameraForward()));
    }
}

void BallerEngine::Camera::_panHorizontal(){
    float sensitivity = 0.05f;
    float deltaX = GetMouseDelta().x;
    if(deltaX != 0.0f){
        Vector3 right = _getCameraRight();
        camera.target = Vector3Add(camera.target, Vector3Scale(right, deltaX*sensitivity));
    }
}

void BallerEngine::Camera::updateCamera(){
    float sensitivity = 0.05f;
    Vector2 mouseDelta = GetMouseDelta();
    if(mouseDelta != Vector2{0.0f, 0.0f}){
        _panVertical();
        _panHorizontal();
    }
    
    if(IsKeyDown(KEY_W)||IsKeyDown(KEY_S)){
        _moveForward();
    }

    if(IsKeyDown(KEY_A)||IsKeyDown(KEY_D)){
        _moveSideward();
    }

    if(IsKeyDown(KEY_Q)||IsKeyDown(KEY_E)){
        _moveUpward();
    }

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        std::cout << camera.target.x << " " << camera.target.y << " " << camera.target.z << std::endl;
    }
}

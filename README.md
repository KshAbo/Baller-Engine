# Baller Engine

Baller Engine is a 3D graphics and physics engine that features a unique, hand-gesture-based control mechanism. It combines C++ (for rendering and physics simulation) with Python (for real-time computer vision and gesture recognition).

## Demo

https://github.com/user-attachments/assets/a6811e6c-c272-449c-bb07-bfa38e4e8790

## Features

- **Gesture Control**: Drive a car in a 3D space using real-time hand gestures tracked by your webcam.
- **Physics Simulation**: Built-in rigid body dynamics using the Bullet3 physics engine.
- **3D Rendering**: High-performance graphics powered by Raylib, including custom shaders, lighting, and 3D models.
- **Networked Communication**: Seamless socket communication between the Python gesture server and the C++ engine client.

## Setup & Installation

### Prerequisites

- **C++ Environment**: C++17 compiler, CMake (>= 3.10), and Make.
- **System Libraries**: `glfw`, `GL`, `X11`, `pthread`, `Xrandr`, `Xi`, `dl` (required for Raylib).
- **Python Environment**: Python 3.x, `opencv-python`, and `mediapipe`.

### Installing Python Dependencies

```bash
pip install opencv-python mediapipe
```

### Building the C++ Engine

1. Clone the repository and navigate to the project root directory.
2. Initialize and update the submodules (this downloads the `bullet3` dependencies):
   ```bash
   git submodule update --init --recursive
   ```
3. Create a build directory and run CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```
3. Compile the project:
   ```bash
   make
   ```

### Running the Application

1. **Start the Gesture Recognition Server**:
   Open a terminal, navigate to the project root directory, and run the Python script:
   ```bash
   python src/controlsNotifier.py
   ```
   *Note: This will open your webcam and start a local socket server on port 65432. Make sure it prints "Waiting for connection...".*

2. **Run the Game Engine**:
   Open a second terminal, navigate to the `build` directory, and start the executable:
   ```bash
   cd build
   ./BallerEngine
   ```
   *Note: You must run the executable from inside the `build` directory so it can correctly resolve paths to the 3D models and shaders.*

## Code Architecture

The project is split into a Python gesture recognition server and a C++ game/physics engine client.

### Python Gesture Server (`src/controlsNotifier.py`)
- **Computer Vision**: Uses OpenCV to capture webcam frames and MediaPipe Hands to detect hand landmarks.
- **Gesture Logic**: Evaluates relative positions of finger tips and joints to classify gestures (e.g., `forward`, `left`, `right`, `jump`, `brake`, `stop`).
- **Networking**: Hosts a TCP socket server on `127.0.0.1:65432` to stream gesture commands to the C++ client in real time.

### C++ Game Engine (`src/`)
- **`main.cpp`**: The entry point. Initializes the Raylib window, sets up the 3D environment, loads shaders and models, creates physics objects, and connects the client socket to the Python server. Contains the main rendering and physics loop.
- **`gameObjects.cpp` / `gameObjects.hpp`**: Defines `PhysicsObject` (base class for objects with physical properties) and `Car` (which processes network commands to apply forces and torques). Wraps the Bullet3 physics engine context.
- **`controller.cpp`**: Handles the logic of converting gesture commands (accelerate, brake, steer, jump) into physics forces and impulses applied to the car.
- **`camera.cpp`**: Implements `Camera` and `GameCamera` classes. `GameCamera` dynamically follows the car, while the free camera allows for manual 3D space navigation.
- **Shaders (`src/shader/`)**: Contains custom vertex and fragment GLSL shaders used to render the game objects and dynamic lighting.

## Controls / Gestures

- **Forward**: Point index finger up (other fingers closed).
- **Left**: Point index finger left.
- **Right**: Point index finger right.
- **Jump**: "Rock & Roll" gesture (index and pinky up, middle and ring down).
- **Brake**: Open palm facing the camera.

## Code Conventions

See `src/README.md` for specific naming conventions followed in the C++ source code.
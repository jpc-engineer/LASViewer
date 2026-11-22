# LAS Point Cloud Viewer

A fast, lightweight C++17 and OpenGL 4.1 desktop viewer for uncompressed LiDAR (`.las`) point clouds. Built from scratch without heavy geospatial or 3D engine dependencies to explore binary file I/O, 3D affine coordinate transformations, and real-time GPU rendering.

## Features
- **Zero-Dependency `.las` Binary Parser:** Reads LAS 1.2–1.4 headers directly via `<fstream>` and streams scaled `(X, Y, Z)` coordinates without external parsing libraries.
- **Floating-Point Jitter Prevention:** Automatically computes the bounding box center in 64-bit `double` precision and shifts coordinates to `(0, 0, 0)` before GPU upload, preventing vertex jitter on large map projections (e.g., UTM / MGA).
- **GIS-to-OpenGL Axis Alignment:** Converts geospatial `Z-up` coordinates into OpenGL `Y-up` space with hypsometric elevation color ramping.
- **Instant Drag-and-Drop & Subsampling:** Drop any `.las` file directly onto the window; large scans are automatically subsampled on disk via byte-offset seeking to maintain smooth 60 FPS rendering.
- **Interactive Orbit Camera:** Spherical-coordinate arcball camera supporting smooth rotation, target panning, and multiplicative zoom.

## Controls

| Input | Action |
| :--- | :--- |
| **Left-Click + Drag** | Orbit / rotate around the point cloud |
| **Right-Click + Drag** | Pan the camera target |
| **Scroll Wheel / Trackpad** | Zoom in and out |
| **Drag & Drop `.las` File** | Load and auto-fit a new point cloud |

## Architecture
```text
lidar-viewer/
├── CMakeLists.txt
├── shaders/              # GLSL vertex & fragment shaders
├── include/              # GLAD OpenGL loader headers
└── src/
    ├── main.cpp          # Application entry point & event wiring
    ├── core/             # Window management & input callbacks (GLFW)
    ├── graphics/         # Shader compilation & VAO/VBO mesh buffers
    ├── scene/            # Spherical orbit camera & MVP matrix math (GLM)
    └── io/               # Binary .las parser & elevation color mapping
```

## Build Instructions (macOS / Apple Silicon)

### Prerequisites
- CMake (3.16+)
- C++17 compiler (Apple Clang)
- [GLFW](https://glfw.org) & [GLM](https://github.com)

```bash
brew install cmake glfw glm
```

### Compile & Run
```bash
git clone https://github.com
cd lidar-viewer

cmake -B build
cmake --build build

cd build && ./LASViewer
```

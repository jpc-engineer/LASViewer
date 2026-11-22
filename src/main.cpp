#include "core/Window.hpp"
#include "graphics/Shader.hpp"
#include "graphics/PointCloudMesh.hpp"
#include "scene/OrbitCamera.hpp"
#include "io/LASLoader.hpp"
#include <iostream>

int main() {
    try {
        Window window(1280, 720, "LAS Point Cloud Viewer");
        Shader pointShader("shaders/point.vert", "shaders/point.frag");
        PointCloudMesh cloudMesh;
        OrbitCamera camera;

        auto demo = LASLoader::createDemoWave();
        cloudMesh.upload(demo.vertices);
        camera.fitToRadius(demo.radius);

        window.onMouseDrag([&](float dx, float dy, bool isRightClick) {
            if (isRightClick) camera.pan(dx, dy);
            else camera.rotate(dx, dy);
        });

        window.onScroll([&](float yOffset) {
            camera.zoom(yOffset);
        });

        window.onFileDrop([&](const std::string& filepath) {
            try {
                auto data = LASLoader::loadFromFile(filepath, 200000);
                cloudMesh.upload(data.vertices);
                camera.fitToRadius(data.radius);
            } catch (const std::exception& e) {
                std::cerr << "Load error: " << e.what() << "\n";
            }
        });

        while (!window.shouldClose()) {
            window.beginFrame(0.08f, 0.09f, 0.11f);

            pointShader.use();
            pointShader.setMat4("uMVP", camera.getMVP(window.getAspectRatio()));
            pointShader.setFloat("uPointSize", 3.0f);

            cloudMesh.draw();

            window.endFrame();
        }
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return -1;
    }

    return 0;
}

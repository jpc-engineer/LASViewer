#pragma once
#include "graphics/PointCloudMesh.hpp"
#include <string>
#include <vector>

struct PointCloudData {
    std::vector<Vertex> vertices;
    float radius = 10.0f;
};

class LASLoader {
public:
    static PointCloudData loadFromFile(const std::string& filepath, size_t maxPoints = 200000);
    static PointCloudData createDemoWave();

private:
    static void elevationToRGB(float t, float& r, float& g, float& b);
};

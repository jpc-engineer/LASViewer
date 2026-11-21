#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <stdexcept>

struct Vertex {
    float x, y, z;
    float r, g, b;
};

struct PointCloudData {
    std::vector<Vertex> vertices;
    float radius = 10.0f;
};

#pragma pack(push, 1)
struct LASHeader {
    char signature[4];
    uint16_t fileSourceID;
    uint16_t globalEncoding;
    uint32_t guid1;
    uint16_t guid2, guid3;
    uint8_t guid4[8];
    uint8_t versionMajor, versionMinor;
    char systemID[32];
    char generatingSoftware[32];
    uint16_t creationDay, creationYear;
    uint16_t headerSize;
    uint32_t offsetToPointData;
    uint32_t numVLRs;
    uint8_t pointDataFormat;
    uint16_t pointDataRecordLength;
    uint32_t legacyPointCount;
    uint32_t legacyPointsByReturn[5];
    double xScale, yScale, zScale;
    double xOffset, yOffset, zOffset;
    double maxX, minX, maxY, minY, maxZ, minZ;
};

struct LASPointBase {
    int32_t x, y, z;
    uint32_t intensity;
};
#pragma pack(pop)

// maps normalized elevation [0.0, 1.0] to a blue -> cyan -> green -> yellow -> red gradient
inline void elevationToRGB(float t, float& r, float& g, float& b) {
    t = std::clamp(t, 0.0f, 1.0f);
    if (t < 0.25f) {
        float k = t / 0.25f;
        r = 0.1f; g = 0.2f + 0.8f * k; b = 0.9f;
    } else if (t < 0.5f) {
        float k = (t - 0.25f) / 0.25f;
        r = 0.1f; g = 1.0f; b = 0.9f * (1.0f - k);
    } else if (t < 0.75f) {
        float k = (t - 0.5f) / 0.25f;
        r = 0.1f + 0.9f * k; g = 1.0f; b = 0.1f;
    } else {
        float k = (t - 0.75f) / 0.25f;
        r = 1.0f; g = 1.0f - 0.8f * k; b = 0.1f;
    }
}

inline PointCloudData loadLAS(const std::string& filepath, size_t maxPoints = 2000000) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot open file: " + filepath);

    LASHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(LASHeader));

    if (std::strncmp(header.signature, "LASF", 4) != 0) {
        throw std::runtime_error("Invalid LAS file signature.");
    }

    uint64_t totalPoints = header.legacyPointCount;
    if (totalPoints == 0 && header.versionMinor >= 4) {
        file.seekg(247);
        file.read(reinterpret_cast<char*>(&totalPoints), sizeof(uint64_t));
    } 

    double centerX = (header.minX + header.maxX) * 0.5;
    double centerY = (header.minY + header.maxY) * 0.5;
    double centerZ = (header.minZ + header.maxZ) * 0.5;
    double zSpan = std::max(0.001, header.maxZ - header.minZ);

    float spanX = static_cast<float>(header.maxX - header.minX);
    float spanY = static_cast<float>(header.maxY - header.minY);
    float radius = std::max({spanX, spanY, 1.0f}) * 0.5f;

    uint64_t step = (totalPoints > maxPoints) ? (totalPoints / maxPoints) : 1;

    PointCloudData result;
    result.radius = radius;
    result.vertices.reserve(std::min<uint64_t>(totalPoints, maxPoints));

    for (uint64_t i = 0; i < totalPoints; i += step) {
        file.seekg(header.offsetToPointData + (i * header.pointDataRecordLength));
        LASPointBase raw;
        file.read(reinterpret_cast<char*>(&raw), sizeof(LASPointBase));

        double wx = raw.x * header.xScale + header.xOffset;
        double wy = raw.y * header.yScale + header.yOffset;
        double wz = raw.z * header.zScale + header.zOffset;

        float normZ = static_cast<float>((wz - header.minZ) / zSpan);
        float r, g, b;
        elevationToRGB(normZ, r, g, b);

        // Center at (0,0,0) and swap GIS Z-up to OpenGL Y-Up
        result.vertices.push_back({
            static_cast<float>(wx - centerX),
            static_cast<float>(wz - centerZ),
            static_cast<float>(-(wy - centerY)),
            r, g, b
        });
    }

    std::cout << "Loaded " << result.vertices.size() << " points (from " << totalPoints << " total)\n";
    return result;
}

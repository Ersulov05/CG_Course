#pragma once
#include "../Canvas/RendererData.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

class OBJModelLoader {
public:
    static MeshData LoadModel(std::string modelPath) 
    {
        return LoadOBJModel(modelPath);
    }

private:
    struct RawVertexData {
        std::vector<Point3D> positions;
        std::vector<Vector3D> normals;
        std::vector<TextureCoord> texCoords;
    };
    
    struct VertexKey {
        int posIdx;      
        int texIdx;
        int normIdx;
        
        bool operator==(const VertexKey& other) const {
            return posIdx == other.posIdx && 
                   texIdx == other.texIdx && 
                   normIdx == other.normIdx;
        }
    };
    
    struct VertexKeyHash {
        std::size_t operator()(const VertexKey& k) const {
            return ((std::hash<int>()(k.posIdx) ^ 
                    (std::hash<int>()(k.texIdx) << 1)) ^ 
                    (std::hash<int>()(k.normIdx) << 2));
        }
    };

    struct Material
    {
        std::string name;
        Color ambient = Color(0xFFFFFFFF);
        Color diffuse = Color(0xFFFFFFFF);
        std::string diffuseTexture;
        
        bool hasTexture() const {
            return !diffuseTexture.empty();
        }
    };

    using VertexMap = std::unordered_map<VertexKey, unsigned int, VertexKeyHash>;
    using MaterialMap = std::unordered_map<std::string, Material>;
    
    static MeshData LoadOBJModel(const std::string& filepath) 
    {
        MeshData meshData;
        SubMeshData subMeshData;
        RawVertexData rawData;
        VertexMap vertexMap;
        MaterialMap materialMap;
        Material currentMaterial;
        std::string objDirectory = GetDirectoryPath(filepath);
        
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filepath << std::endl;
            return meshData;
        }
        
        std::string line;
        int lineNum = 0;
        
        while (std::getline(file, line)) {
            ++lineNum;
            if (line.empty() || line[0] == '#') continue;
            
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;
            
            if (prefix == "v") {
                Point3D vertexPosition = ParseVertexPosition(iss);
                rawData.positions.push_back(vertexPosition);
            }
            else if (prefix == "vt") {
                TextureCoord uv = ParseVertexTextureCoords(iss);
                rawData.texCoords.push_back(uv);
            }
            else if (prefix == "vn") {
                Vector3D normal = ParseVertexNormal(iss);
                rawData.normals.push_back(normal);
            }
            else if (prefix == "f") {
                if (!ParseFace(iss, subMeshData, vertexMap, rawData, currentMaterial)) {
                    std::cerr << "Ошибка грани в линии: " << lineNum << std::endl;
                }
            }
            else if (prefix == "usemtl") {
                std::string materialName;
                iss >> materialName;
                meshData.subMeshes.push_back(subMeshData);

                currentMaterial = GetMaterial(materialMap, materialName);
                vertexMap.clear();
                subMeshData = SubMeshData();
                if (currentMaterial.hasTexture()) {
                    subMeshData.textures = {currentMaterial.diffuseTexture};
                }
            }
            else if (prefix == "mtllib") {
                std::string mtlFile;
                iss >> mtlFile;
                materialMap = LoadMTLFile(mtlFile, objDirectory);
            }
        }

        if (!subMeshData.indices.empty()) {
            meshData.subMeshes.push_back(subMeshData);
        }
        
        file.close();        
        return meshData;
    }

    static Point3D ParseVertexPosition(std::istringstream& iss) {
        Point3D pos;
        iss >> pos.x >> pos.y >> pos.z;

        return pos;
    }

    static Vector3D ParseVertexNormal(std::istringstream& iss) {
        Vector3D normal;
        iss >> normal.x >> normal.y >> normal.z;
        normal.Normalized();

        return normal;
    }

    static TextureCoord ParseVertexTextureCoords(std::istringstream& iss) {
        TextureCoord uv;
        iss >> uv.u >> uv.v;

        return uv;
    }

    static VertexKey ParseVertexKey(const std::string& vertexStr) {
        VertexKey key = {0, 0, 0};
        size_t firstSlash = vertexStr.find('/');
        size_t secondSlash = vertexStr.find('/', firstSlash + 1);
        
        key.posIdx = std::stoi(vertexStr.substr(0, firstSlash));
        if (firstSlash != std::string::npos) {
            if (firstSlash + 1 < vertexStr.length() && vertexStr[firstSlash + 1] != '/') {
                size_t texEnd = (secondSlash != std::string::npos) ? secondSlash : vertexStr.length();
                key.texIdx = std::stoi(vertexStr.substr(firstSlash + 1, texEnd - firstSlash - 1));
            }
            
            if (secondSlash != std::string::npos && secondSlash + 1 < vertexStr.length()) {
                key.normIdx = std::stoi(vertexStr.substr(secondSlash + 1));
            }
        }

        return key;
    }

    static bool ParseFace(
        std::istringstream& iss, 
        SubMeshData& subMeshData, 
        VertexMap& vertexMap, 
        const RawVertexData& rawData,
        const Material& material
    ) 
    {
        std::vector<VertexKey> faceVertices;
        std::string vertexStr;
        
        while (iss >> vertexStr) {
            VertexKey vertexKey = ParseVertexKey(vertexStr);
            faceVertices.push_back(vertexKey);
        }
        
        if (faceVertices.size() == 3) {
            AddVertexToMesh(subMeshData, rawData, vertexMap, faceVertices[0], material);
            AddVertexToMesh(subMeshData, rawData, vertexMap, faceVertices[1], material);
            AddVertexToMesh(subMeshData, rawData, vertexMap, faceVertices[2], material);
        }
        else if (faceVertices.size() == 4) {
            AddVertexToMesh(subMeshData, rawData, vertexMap, faceVertices[0], material);
            AddVertexToMesh(subMeshData, rawData, vertexMap, faceVertices[1], material);
            AddVertexToMesh(subMeshData, rawData, vertexMap, faceVertices[2], material);
            
            AddVertexToMesh(subMeshData, rawData, vertexMap, faceVertices[0], material);
            AddVertexToMesh(subMeshData, rawData, vertexMap, faceVertices[2], material);
            AddVertexToMesh(subMeshData, rawData, vertexMap, faceVertices[3], material);
        }
        else {
            return false;
        }

        return true;
    }

    static void AddVertexToMesh(
        SubMeshData& subMeshData, 
        const RawVertexData& rawData,
        VertexMap& vertexMap,
        const VertexKey& key,
        const Material& material
    ) 
    {
        VertexKey normalizedKey = {key.posIdx - 1, key.texIdx - 1, key.normIdx - 1};
        if (normalizedKey.posIdx < 0 || normalizedKey.posIdx >= (int)rawData.positions.size()) {
            std::cerr << "Некорректный индекс позиции: " << key.posIdx << std::endl;
            return;
        }
        
        auto it = vertexMap.find(normalizedKey);
        if (it != vertexMap.end()) {
            subMeshData.indices.push_back(it->second);
            return;
        }
        
        Vertex newVertex = GetNewVertex(rawData, material, normalizedKey);
        unsigned int newIndex = subMeshData.vertices.size();
        subMeshData.vertices.push_back(newVertex);
        subMeshData.indices.push_back(newIndex);
        vertexMap[normalizedKey] = newIndex;
    } 

    static Vertex GetNewVertex(const RawVertexData& rawData, const Material& material, const VertexKey& key) 
    {
        Vertex newVertex;
        newVertex.position = rawData.positions[key.posIdx];
        newVertex.color = Color(material.diffuse);
        
        if (key.texIdx >= 0 && key.texIdx < (int)rawData.texCoords.size()) {
            newVertex.uvDiffuse = rawData.texCoords[key.texIdx];
        }
        
        if (key.normIdx >= 0 && key.normIdx < (int)rawData.normals.size()) {
            newVertex.normal = rawData.normals[key.normIdx];
        }

        return newVertex;
    }

    static MaterialMap LoadMTLFile(const std::string& mtlPath, const std::string& directoryPath) {
        MaterialMap materialMap;
        std::ifstream file(directoryPath + "/" + mtlPath);
        
        if (!file.is_open()) {
            std::cerr << "Failed to open MTL: " << mtlPath << std::endl;
            return materialMap;
        }
        
        std::string line;
        Material material;
        
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;
            
            if (prefix == "newmtl") {
                if (!material.name.empty()) {
                    materialMap[material.name] = material;
                }
                material = Material();
                iss >> material.name;
            }
            else if (prefix == "Ka") {
                float r, g, b;
                iss >> r >> g >> b;
                material.ambient = Color(r, g, b, 1.0f);
            }
            else if (prefix == "Kd") {
                float r, g, b;
                iss >> r >> g >> b;
                material.diffuse = Color(r, g, b, 1.0f);
            }
            else if (prefix == "d") {
                float a;
                iss >> a;
                material.diffuse.a = a;
                material.ambient.a = a;
            }
            else if (prefix == "map_Kd") {
                std::string textureFile;
                iss >> textureFile;
                material.diffuseTexture = directoryPath + "/" + textureFile;
            }
        }

        if (!material.name.empty()) {
            materialMap[material.name] = material;
        }
        
        return materialMap;
    }

    static Material GetMaterial(const MaterialMap& materialMap, const std::string& currentMaterial) 
    {
        auto it = materialMap.find(currentMaterial);
        if (it != materialMap.end()) {
            return it->second;
        }
        
        return Material();
    }

    static std::string GetDirectoryPath(const std::string& filepath) {
        size_t lastSlash = filepath.find_last_of("/\\");
        if (lastSlash != std::string::npos) {
            return filepath.substr(0, lastSlash);
        }
        return ".";
    }
};
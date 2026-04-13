#pragma once
#include "../Canvas/RendererData.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "./OBJModelLoader.h"

class ModelLoader {
public:
    static MeshData LoadModel(std::string modelPath) 
    {
        std::string extension = "";
        size_t dotPos = modelPath.find_last_of('.');
        if (dotPos != std::string::npos) {
            extension = modelPath.substr(dotPos);
            std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
        }
        
        if (extension == ".obj") {
            return OBJModelLoader::LoadModel(modelPath);
        }
        
        std::cerr << "Неподдерживаемый формат файла: " << extension << std::endl;
        return MeshData();
    }
};
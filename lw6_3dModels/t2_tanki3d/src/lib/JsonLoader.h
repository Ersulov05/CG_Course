#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <sstream>
#include <fstream>

using json = nlohmann::json;
class JsonLoader {
public:
    static json LoadFromFile(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed load: " + filename);
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string jsonContent = buffer.str();

        return json::parse(jsonContent);
    }
};
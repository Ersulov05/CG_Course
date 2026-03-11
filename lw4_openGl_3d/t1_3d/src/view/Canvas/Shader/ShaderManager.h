#pragma once
#include "./Shader.h"
#include <unordered_map>
#include <memory>
#include <stack>
#include <string>

class ShaderManager
{
public:
    template <typename T, typename... Args>
    void AddShader(const std::string &name, Args &&...args)
    {
        shaders[name] = std::make_unique<T>(std::forward<Args>(args)...);
    }

    void Use()
    {
        current->Use();
    }

    bool SetCurrent(const std::string &name)
    {
        auto it = shaders.find(name);
        if (it != shaders.end())
        {
            current = it->second.get();
            return true;
        }
        return false;
    }

    bool Push(const std::string &name)
    {
        if (current)
            stack.push(current);
        return SetCurrent(name);
    }

    bool Pop()
    {
        if (stack.empty())
            return false;
        current = stack.top();
        stack.pop();
        return true;
    }

    Shader *GetCurrent() { return current; }

private:
    std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
    std::stack<Shader *> stack;
    Shader *current = nullptr;
};

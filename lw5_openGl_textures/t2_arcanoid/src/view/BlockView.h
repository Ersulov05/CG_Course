#pragma once
#include "../model/Block.h"
#include "./Canvas/ICanvas3D.h"
#include "./Meshes/Mesh.h"
#include <unordered_map>

class BlockView {
public:
    static void Draw(const Block& block, ICanvas3D &canvas) {
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(block.GetPosition());
        canvas.GetTransform().Translate(0, block.GetSize().height/2, 0);
        canvas.GetTransform().Scale(block.GetSize());
        
        canvas.DrawMesh(GetColoredBlockMesh(block.GetColorType()));
        canvas.GetTransform().PopMatrix();
    }

private:
    inline static const std::unordered_map<BlockColorType, MeshData> m_coloredBlockMeshes = {
        {BlockColorType::GREEN, Mesh::Cube(0x00FF00FF, {"./textures/green_glass.jpg"})},
        {BlockColorType::YELLOW, Mesh::Cube(0xFFFF00FF, {"./textures/yellow_glass.jpg"})},
        {BlockColorType::ORANGE, Mesh::Cube(0xFFA500FF, {"./textures/orange_glass.jpg"})},
        {BlockColorType::LIGHT_BLUE, Mesh::Cube(0xADD8E6FF, {"./textures/light_blue_glass.jpg"})},
        {BlockColorType::DARK_BLUE, Mesh::Cube(0x00008BFF, {"./textures/dark_blue_glass.jpg"})},
        {BlockColorType::VIOLET, Mesh::Cube(0xEE82EEFF, {"./textures/violet_glass.jpg"})},
        {BlockColorType::RED, Mesh::Cube(0xFF0000FF, {"./textures/red_glass.jpg"})}
    };

    static MeshData GetColoredBlockMesh(BlockColorType colorType) {
        auto it = m_coloredBlockMeshes.find(colorType);
        
        if (it != m_coloredBlockMeshes.end()) {
            return it->second;
        }
        
        static const MeshData defaultMesh = Mesh::Cube(0xFFFFFFFF, {"./textures/green_glass.jpg"});
        return defaultMesh;
    };
};
#pragma once
#include "./Canvas/Canvas3D.h"

class GameView {
public:
    GameView() {
    }

    void Run() {
        m_canvas.GetCamera().SetPosition(Point3D{0, 0, 10});
        m_canvas.Run(
        [this](ICanvas3D &canvas, float deltatime)
        {
            auto &camera = canvas.GetCamera();
            auto line = GetLine(Point3D{-10, 0, 0}, Point3D{10, 0, 0});
            canvas.DrawLine(line);
        });
    }

private:
    Canvas3D m_canvas;

    std::vector<Vertex> GetLine(Point3D start, Point3D end, int vertexCount = 256)
    {
        std::vector<Vertex> vertices;
        vertices.reserve(vertexCount);
        
        for (int i = 0; i < vertexCount; ++i)
        {
            Vertex vertex;
            float t = (float)i / (float)(vertexCount - 1);
            
            Point3D point;
            
            point.x = start.x + t * (end.x - start.x);
            point.y = start.y + t * (end.y - start.y);
            point.z = start.z + t * (end.z - start.z);
            vertex.position = point;
            vertex.color = 0xFFFFFFFF;
            vertices.emplace_back(vertex);
        }
        
        return vertices;
    }
};
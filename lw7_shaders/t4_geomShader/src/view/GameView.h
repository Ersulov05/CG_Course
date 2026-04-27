#pragma once
#include "./Canvas/Canvas3D.h"

class GameView {
public:
    GameView() {
    }

    void Run() {
        m_canvas.GetCamera().SetPosition(Point3D{0, 0, 2});
        m_canvas.Run(
        [this](ICanvas3D &canvas, float deltatime)
        {
            auto &camera = canvas.GetCamera();
            auto line = GetLine(Point3D{-1, 0, 0}, Point3D{1, 0, 0});
            canvas.DrawLine(line);
        });
    }

private:
    Canvas3D m_canvas;

    std::vector<Vertex> GetLine(Point3D start, Point3D end)
    {
        std::vector<Vertex> vertices;
        Vertex vertex;
        vertex.position = start;
        vertex.color = Color(0xFFFFFFFF);
        vertices.push_back(vertex);        
        vertex.position = end;
        vertices.push_back(vertex);
        
        return vertices;
    }
};
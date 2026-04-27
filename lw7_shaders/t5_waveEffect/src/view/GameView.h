#pragma once
#include "./Canvas/Canvas3D.h"
#include "./Meshes/Mesh.h"

class GameView {
public:
    GameView() {
        auto& mouseController = m_canvas.GetMouseController();

        mouseController.OnPressSubscribe([this](const Point& pos){
            if (!m_isSwaped) {
                m_time = 0;
                std::swap(m_firstTexture, m_secondTexture);
                m_rectMesh.subMeshes[0].textures = {m_firstTexture, m_secondTexture};
                m_isSwaped = true;
                m_clickPos = pos;
            }
        });
    }

    void Run() {
        m_canvas.Run(
        [this](ICanvas3D &canvas, float deltatime)
        {        
            UpdateSwaped(deltatime);
            Draw(canvas);
        });
    }

private:
    Canvas3D m_canvas;
    std::string m_firstTexture = "./textures/brick.jpg";
    std::string m_secondTexture = "./textures/bonus.jpg";
    MeshData m_rectMesh = Mesh::Rect(0xFFFFFFFF, {m_firstTexture, m_secondTexture});
    Point m_clickPos;

    bool m_isSwaped = false;
    float m_time = 1;
    float m_addCoef = 0.8;

    void UpdateSwaped(float deltatime) {
        if (!m_isSwaped) {
            return;
        }

        m_time += deltatime * m_addCoef;
        if (m_time > 1) {
            m_time = 1;
            m_isSwaped = false;
        }
    }

    void Draw(ICanvas3D &canvas)
    {
        auto& shaderManager = canvas.GetShaderManager();
        auto* basicShader = dynamic_cast<BasicShader*>(shaderManager.GetByName("basic"));
        if (!basicShader) return;

        basicShader->SetTime(m_time);
        basicShader->SetClickPos(MouseToUV(m_clickPos, canvas.GetWidth(), canvas.GetHeight()));
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(400, 300, 0);
        canvas.GetTransform().Scale(800, 600, 1);
        canvas.DrawMesh(m_rectMesh);
        canvas.GetTransform().PopMatrix();
    }

    glm::vec2 MouseToUV(Point mousePos, int windowWidth, int windowHeight) {
        float u = mousePos.x / windowWidth;
        float v = mousePos.y / windowHeight;
        return glm::vec2(u, v);
    }
};
#pragma once
#include "./Canvas/ICanvas3D.h"
#include "./Meshes/Mesh.h"
#include "../controller/GameController.h"

class UIView {
public:
    static void Draw(ICanvas3D& canvas, const GameController& controller)
    {
        auto& shaderManager = canvas.GetShaderManager();
        shaderManager.Push("ui");
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().ResetTransform();
        DrawHealthBar(canvas, 75, 100);

        canvas.GetTransform().PopMatrix();
        shaderManager.Pop();
    }
private:
    inline static const MeshData m_healthRect = Mesh::Rect(0xFF0000FF);
    inline static const MeshData m_healthBorderRect = Mesh::Rect(0xFFFFFFFF);
    inline static const float HEALTH_BAR_WIDTH = 200;
    inline static const float HEALTH_BAR_HEIGHT = 30;
    inline static const float HEALTH_BORDER_SIZE = 4;
    inline static const float HEALTH_BAR_PADDING = 20;

    static void DrawHealthBar(ICanvas3D& canvas, unsigned int health, unsigned int totalHealth)
    {
        auto width = canvas.GetWidth();
        auto height = canvas.GetHeight();

        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(HEALTH_BAR_WIDTH/2 + HEALTH_BAR_PADDING, height - HEALTH_BAR_HEIGHT/2 - HEALTH_BAR_PADDING, 0);

        float healthRate = (float)health / (float)totalHealth;

        DrawHealth(canvas, health, totalHealth);
        DrawHealthBorder(canvas);

        canvas.GetTransform().PopMatrix();   
    }

    static void DrawHealth(ICanvas3D& canvas, unsigned int health, unsigned int totalHealth)
    {
        float healthRate = (float)health / (float)totalHealth;

        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(-(1 - healthRate)*HEALTH_BAR_WIDTH/2, 0, 0);
        canvas.GetTransform().Scale(healthRate * HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT, 1);

        canvas.DrawMesh(m_healthRect);

        canvas.GetTransform().PopMatrix();   
    }

    static void DrawHealthBorder(ICanvas3D& canvas)
    {
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(0, 0, 0.1);

        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(0, -HEALTH_BAR_HEIGHT/2, 0);
        canvas.GetTransform().Scale(HEALTH_BAR_WIDTH, HEALTH_BORDER_SIZE, 1);
        canvas.DrawMesh(m_healthBorderRect);
        canvas.GetTransform().PopMatrix(); 

        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(0, HEALTH_BAR_HEIGHT/2, 0);
        canvas.GetTransform().Scale(HEALTH_BAR_WIDTH, HEALTH_BORDER_SIZE, 1);
        canvas.DrawMesh(m_healthBorderRect);
        canvas.GetTransform().PopMatrix(); 

        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(-HEALTH_BAR_WIDTH/2, 0, 0);
        canvas.GetTransform().Scale(HEALTH_BORDER_SIZE, HEALTH_BAR_HEIGHT, 1);
        canvas.DrawMesh(m_healthBorderRect);
        canvas.GetTransform().PopMatrix(); 

        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(HEALTH_BAR_WIDTH/2, 0, 0);
        canvas.GetTransform().Scale(HEALTH_BORDER_SIZE, HEALTH_BAR_HEIGHT, 1);
        canvas.DrawMesh(m_healthBorderRect);
        canvas.GetTransform().PopMatrix();         

        canvas.GetTransform().PopMatrix(); 
    }
};
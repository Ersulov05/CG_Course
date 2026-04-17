#pragma once
#include "../Canvas/ICanvas3D.h"
#include "../Meshes/Mesh.h"
#include "../../controller/GameController.h"

class CannonReloadView {
public:
    static void Draw(ICanvas3D& canvas, float reloadTime, float totalReloadTime, int count)
    {
        auto height = canvas.GetHeight();
        auto width = canvas.GetWidth();

        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(width - RELOAD_BAR_WIDTH/2 - RELOAD_BAR_PADDING, height - RELOAD_BAR_HEIGHT/2 - RELOAD_BAR_PADDING, 0);

        for (int i = 0; i < count; ++i)
        {
            DrawReload(canvas, reloadTime, totalReloadTime);
            DrawReloadBorder(canvas);
            canvas.GetTransform().Translate(-RELOAD_BAR_WIDTH - OFFSET, 0, 0);
        }

        canvas.GetTransform().PopMatrix();
    }
private:
    inline static const MeshData m_healthRect = Mesh::Rect(0xBFBF00FF);
    inline static const MeshData m_healthBorderRect = Mesh::Rect(0xFFFFFFFF);
    inline static const float RELOAD_BAR_WIDTH = 30;
    inline static const float RELOAD_BAR_HEIGHT = 70;
    inline static const float RELOAD_BORDER_SIZE = 4;
    inline static const float RELOAD_BAR_PADDING = 20;
    inline static const float OFFSET = 15;

    static void DrawReload(ICanvas3D& canvas, float reloadTime, float totalReloadTime)
    {
        float reloadRate = 1 - (float)reloadTime / (float)totalReloadTime;

        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(0, (1 - reloadRate)*RELOAD_BAR_HEIGHT/2, 0);
        canvas.GetTransform().Scale(RELOAD_BAR_WIDTH, reloadRate * RELOAD_BAR_HEIGHT, 1);

        canvas.DrawMesh(m_healthRect);

        canvas.GetTransform().PopMatrix();   
    }

    static void DrawReloadBorder(ICanvas3D& canvas)
    {
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(0, 0, 0.1);

        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(0, -RELOAD_BAR_HEIGHT/2, 0);
        canvas.GetTransform().Scale(RELOAD_BAR_WIDTH, RELOAD_BORDER_SIZE, 1);
        canvas.DrawMesh(m_healthBorderRect);
        canvas.GetTransform().PopMatrix(); 

        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(0, RELOAD_BAR_HEIGHT/2, 0);
        canvas.GetTransform().Scale(RELOAD_BAR_WIDTH, RELOAD_BORDER_SIZE, 1);
        canvas.DrawMesh(m_healthBorderRect);
        canvas.GetTransform().PopMatrix(); 

        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(-RELOAD_BAR_WIDTH/2, 0, 0);
        canvas.GetTransform().Scale(RELOAD_BORDER_SIZE, RELOAD_BAR_HEIGHT, 1);
        canvas.DrawMesh(m_healthBorderRect);
        canvas.GetTransform().PopMatrix(); 

        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(RELOAD_BAR_WIDTH/2, 0, 0);
        canvas.GetTransform().Scale(RELOAD_BORDER_SIZE, RELOAD_BAR_HEIGHT, 1);
        canvas.DrawMesh(m_healthBorderRect);
        canvas.GetTransform().PopMatrix();         

        canvas.GetTransform().PopMatrix(); 
    }
};
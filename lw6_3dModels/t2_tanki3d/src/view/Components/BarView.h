#pragma once
#include "../Canvas/ICanvas3D.h"
#include "../Meshes/Mesh.h"
#include "../../controller/GameController.h"

struct BarParam {
    float width;
    float height;
    float borderSize;
    Color color;
    Color borderColor;
};

struct BarData
{
    float count;
    float totalCount;
};

class BarView
{
public:
    static void Draw(ICanvas3D& canvas, const BarParam& barParam, const BarData& barData)
    {
        canvas.GetTransform().PushMatrix();
    
        DrawBar(canvas, barParam, barData);
        DrawBarBorder(canvas, barParam, barData);

        canvas.GetTransform().PopMatrix();
    }
private:
    static void DrawBar(ICanvas3D& canvas, const BarParam& barParam, const BarData& barData)
    {
        const auto rect = Mesh::Rect(barParam.color);
        float healthRate = barData.count / barData.totalCount;

        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(-(1 - healthRate)*barParam.width/2, 0, 0);
        canvas.GetTransform().Scale(healthRate * barParam.width, barParam.height, 1);

        canvas.DrawMesh(rect);

        canvas.GetTransform().PopMatrix();   
    }

    static void DrawBarBorder(ICanvas3D& canvas, const BarParam& barParam, const BarData& barData)
    {
        const auto rect = Mesh::Rect(barParam.borderColor);
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(0, 0, 0.1);

        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(0, -barParam.height/2, 0);
        canvas.GetTransform().Scale(barParam.width, barParam.borderSize, 1);
        canvas.DrawMesh(rect);
        canvas.GetTransform().PopMatrix(); 

        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(0, barParam.height/2, 0);
        canvas.GetTransform().Scale(barParam.width, barParam.borderSize, 1);
        canvas.DrawMesh(rect);
        canvas.GetTransform().PopMatrix(); 

        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(-barParam.width/2, 0, 0);
        canvas.GetTransform().Scale(barParam.borderSize, barParam.height, 1);
        canvas.DrawMesh(rect);
        canvas.GetTransform().PopMatrix(); 

        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(barParam.width/2, 0, 0);
        canvas.GetTransform().Scale(barParam.borderSize, barParam.height, 1);
        canvas.DrawMesh(rect);
        canvas.GetTransform().PopMatrix();         

        canvas.GetTransform().PopMatrix(); 
    }
};
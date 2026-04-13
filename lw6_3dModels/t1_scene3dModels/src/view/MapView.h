#pragma once
#include "./Canvas/ICanvas3D.h"
#include "./ModelLoader/ModelLoader.h"
#include "./Meshes/Mesh.h"

class MapView {
public:
    static void Draw(ICanvas3D& canvas) {
        canvas.GetTransform().PushMatrix();
        DrawScene(canvas);
        DrawRoad(canvas);
        DrawCottage(canvas);
        DrawTrees(canvas);
        DrawFlowers(canvas);
        canvas.GetTransform().PopMatrix();
    }
private:
    inline static const MeshData m_cottageModel = ModelLoader::LoadModel("./models/cottage/cottage2.obj");
    inline static const MeshData m_treeModel = ModelLoader::LoadModel("./models/tree/tree.obj");
    inline static const MeshData m_flowerModel = ModelLoader::LoadModel("./models/flower/flower.obj");
    inline static const MeshData m_roadModel = ModelLoader::LoadModel("./models/road/road.obj");
    inline static const MeshData m_sceneModel = Mesh::Cube(0xFFFFFFFF, {"./textures/grass.jpg"}, {14, 0.2, 21});

    inline static const std::vector<Point3D> m_treePositions = {
        Point3D(5, 0, 0),
        Point3D(5.2, 0, 2),
        Point3D(4, 0, 4),
        Point3D(5.5, 0, 6),
        Point3D(5, 0, 8),

        Point3D(-5, 0, 0),
        Point3D(-5.2, 0, -2),
        Point3D(-4, 0, -4),
        Point3D(-5.5, 0, -6),
        Point3D(-5, 0, -8),
    };

    inline static const std::vector<Point3D> m_flowerPositions = {
        Point3D(-4, 0, 6),
        Point3D(-3.5, 0, 6),
        Point3D(-4, 0, 6.5),
        Point3D(-3.5, 0, 6.5),
        Point3D(-4, 0, 7),
        Point3D(-3.5, 0, 7),

        Point3D(3.5, 0, -6),
        Point3D(4, 0, -6),
        Point3D(3.5, 0, -6.5),
        Point3D(4, 0, -6.5),
        Point3D(3.5, 0, -7),
        Point3D(4, 0, -7),
    };

    static void DrawScene(ICanvas3D& canvas) {
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(0, -0.1, 0);
        canvas.GetTransform().Scale(14, 0.2, 21);
        canvas.DrawMesh(m_sceneModel);
        canvas.GetTransform().PopMatrix();
    }

    static void DrawRoad(ICanvas3D& canvas) {
        canvas.GetTransform().PushMatrix();
        canvas.DrawMesh(m_roadModel);
        canvas.GetTransform().PopMatrix();
    }

    static void DrawCottage(ICanvas3D& canvas) {
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate({-4.6, 0, 4});
        canvas.GetTransform().Scale(0.1);
        canvas.DrawMesh(m_cottageModel);
        canvas.GetTransform().PopMatrix();

        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate({4.6, 0, -4});
        canvas.GetTransform().RotateY(180);
        canvas.GetTransform().Scale(0.1);
        canvas.DrawMesh(m_cottageModel);
        canvas.GetTransform().PopMatrix();
    }

    static void DrawTrees(ICanvas3D& canvas) {
        for (auto& treePos: m_treePositions) {
            canvas.GetTransform().PushMatrix();
            canvas.GetTransform().Translate(treePos);
            canvas.GetTransform().Scale(1);
            canvas.DrawMesh(m_treeModel);
            canvas.GetTransform().PopMatrix();
        }
    }

    static void DrawFlowers(ICanvas3D& canvas) {
        for (auto& flowerPos: m_flowerPositions) {
            canvas.GetTransform().PushMatrix();
            canvas.GetTransform().Translate(flowerPos);
            canvas.GetTransform().Scale(1);
            canvas.DrawMesh(m_flowerModel);
            canvas.GetTransform().PopMatrix();
        }
    }
};
#pragma once
#include "./Canvas/ICanvas3D.h"
#include "./Meshes/Mesh.h"
#include "../model/Constants.h"

class SceneView {
public:
    void Draw(ICanvas3D &canvas) {
        DrawFloorScene(canvas);
        DrawDangerArea(canvas);
        DrawForwardWall(canvas);
        DrawLeftWall(canvas);
        DrawRightWall(canvas);
    }

private:
    MeshData m_floorMesh = Mesh::Cube(0xFFFFFFFF, {"./textures/beton.jpg"}, {SCENE_SIZE.width, 1, SCENE_SIZE.depth});
    MeshData m_dangerAreaMesh = Mesh::Cube(0xFF0000FF, {"./textures/beton.jpg"}, {SCENE_SIZE.width, 1, 0.2});
    MeshData m_leftWall = Mesh::Cube(0xFFFFFFFF, {"./textures/brick.jpg"}, {WALL_THICKNESS, WALL_HEIGHT, SCENE_SIZE.depth});
    MeshData m_rightWall = Mesh::Cube(0xFFFFFFFF, {"./textures/brick.jpg"}, {WALL_THICKNESS, WALL_HEIGHT, SCENE_SIZE.depth});
    MeshData m_forwardWall = Mesh::Cube(0xFFFFFFFF, {"./textures/brick.jpg"}, {SCENE_SIZE.width, WALL_HEIGHT, WALL_THICKNESS});

    void DrawFloorScene(ICanvas3D& canvas) {
        auto sceneSize = SCENE_SIZE;
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(0, -0.1, -sceneSize.depth/2);
        canvas.GetTransform().Scale(sceneSize.width, 0.2, sceneSize.depth);
        canvas.DrawMesh(m_floorMesh);
        canvas.GetTransform().PopMatrix();
    }

    void DrawDangerArea(ICanvas3D& canvas) {
        Size3D size = {SCENE_SIZE.width, 0.2, 0.2};
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(0, -size.height/2, size.depth/2);
        canvas.GetTransform().Scale(size);
        canvas.DrawMesh(m_dangerAreaMesh);
        canvas.GetTransform().PopMatrix();
    }

    void DrawForwardWall(ICanvas3D& canvas) {
        Size3D size = {SCENE_SIZE.width, WALL_HEIGHT, WALL_THICKNESS};
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(0, size.height/2, -SCENE_SIZE.depth - size.depth/2);
        canvas.GetTransform().Scale(size);
        canvas.DrawMesh(m_forwardWall);
        canvas.GetTransform().PopMatrix();
    }

    void DrawLeftWall(ICanvas3D& canvas) {
        Size3D size = {WALL_THICKNESS, WALL_HEIGHT, SCENE_SIZE.depth};
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(-SCENE_SIZE.height - size.width/2, size.height/2, -size.depth/2);
        canvas.GetTransform().Scale(size);
        canvas.DrawMesh(m_leftWall);
        canvas.GetTransform().PopMatrix();
    }

    void DrawRightWall(ICanvas3D& canvas) {
        Size3D size = {WALL_THICKNESS, WALL_HEIGHT, SCENE_SIZE.depth};
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(SCENE_SIZE.height + size.width/2, size.height/2, -size.depth/2);
        canvas.GetTransform().Scale(size);
        canvas.DrawMesh(m_rightWall);
        canvas.GetTransform().PopMatrix();
    }
    inline static const float WALL_THICKNESS = 0.1;
    inline static const float WALL_HEIGHT = 1.5;
};
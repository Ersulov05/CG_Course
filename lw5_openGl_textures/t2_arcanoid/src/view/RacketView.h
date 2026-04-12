#pragma once
#include "../model/Racket.h"
#include "./Canvas/ICanvas3D.h"
#include "./Meshes/Mesh.h"

class RacketView {
public:
    RacketView(Racket &racket): m_racket(racket) {
        GenerateRacketMesh();
    }

    void Draw(ICanvas3D &canvas) {
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(m_racket.GetPosition());
        canvas.GetTransform().Translate(0, m_racket.GetSize().height/2, 0);
        canvas.GetTransform().Scale(m_racket.GetSize());
        
        canvas.DrawMesh(m_racketMesh);
        canvas.GetTransform().PopMatrix();
    }

private:
    Racket& m_racket;
    MeshData m_racketMesh;

    void GenerateRacketMesh() {
        auto color = Color(0xFFFFFFFF);

        m_racketMesh = Mesh::Cube(color, {"./textures/wood2.jpg"});
    }
};
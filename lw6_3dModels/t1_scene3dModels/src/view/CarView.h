#pragma once
#include "./Canvas/ICanvas3D.h"
#include "./ModelLoader/ModelLoader.h"
#include "./Meshes/Mesh.h"
#include "../model/Car.h"

class CarView {
public:
    static void Draw(ICanvas3D& canvas, const Car& car, bool isPolice) {
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(car.GetPosition());
        canvas.GetTransform().RotateY(-90);
        canvas.GetTransform().Scale(0.7);
        if (isPolice) {
            canvas.DrawMesh(m_policeCar);
        } else {
            canvas.DrawMesh(m_car);
        }
        canvas.GetTransform().PopMatrix();
    }

private:
    inline static const MeshData m_car = ModelLoader::LoadModel("./models/car01/car01.obj");
    inline static const MeshData m_policeCar = ModelLoader::LoadModel("./models/car_police/car_police.obj");
};
#include <iostream>
#include "./src/view/Canvas/Canvas3D.h"
#include "./src/view/Canvas/ICanvas3D.h"

static MeshData CreateCube(float size = 1.0f)
{
    MeshData mesh;

    float half = size * 0.5f;

    // 8 уникальных позиций вершин
    Point3D positions[8] = {
        Point3D(-half, -half, -half), // 0
        Point3D(half, -half, -half),  // 1
        Point3D(half, half, -half),   // 2
        Point3D(-half, half, -half),  // 3
        Point3D(-half, -half, half),  // 4
        Point3D(half, -half, half),   // 5
        Point3D(half, half, half),    // 6
        Point3D(-half, half, half)    // 7
    };

    // Нормали для каждой грани
    Vector3D normals[6] = {
        Vector3D(0, 0, -1), // передняя
        Vector3D(0, 0, 1),  // задняя
        Vector3D(-1, 0, 0), // левая
        Vector3D(1, 0, 0),  // правая
        Vector3D(0, -1, 0), // нижняя
        Vector3D(0, 1, 0)   // верхняя
    };

    // Индексы вершин для каждой грани (4 вершины на грань)
    int faceIndices[6][4] = {
        {0, 1, 2, 3}, // передняя
        {5, 4, 7, 6}, // задняя
        {4, 0, 3, 7}, // левая
        {1, 5, 6, 2}, // правая
        {4, 5, 1, 0}, // нижняя
        {3, 2, 6, 7}  // верхняя
    };

    Color colors[6] = {
        Color(0xFFFFFF4F),
        Color(0x00FF004F),
        Color(0x0000FF4F),
        Color(0xFFFF004F),
        Color(0x00FFFF4F),
        Color(0xFF00FF4F)};

    // Создаём вершины для граней (24 вершины)
    for (int face = 0; face < 6; face++)
    {
        for (int i = 0; i < 4; i++)
        {
            int vertexIndex = faceIndices[face][i];
            Vertex v;
            v.position = positions[vertexIndex];
            v.normal = normals[face];
            v.color = colors[face]; // все грани красные
            mesh.vertices.push_back(v);
        }
    }

    // Индексы для треугольников (36 индексов)
    for (int i = 0; i < 24; i += 4)
    {
        // Первый треугольник
        mesh.indices.push_back(i);
        mesh.indices.push_back(i + 1);
        mesh.indices.push_back(i + 2);

        // Второй треугольник
        mesh.indices.push_back(i);
        mesh.indices.push_back(i + 2);
        mesh.indices.push_back(i + 3);
    }

    // Создаём рёбра для wireframe (12 рёбер по 2 вершины)
    int edgePairs[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // передняя грань
        {4, 5},
        {5, 6},
        {6, 7},
        {7, 4}, // задняя грань
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7} // соединения
    };

    // Цвет для рёбер (чёрный)
    Color edgeColor(0, 0, 0, 255);

    // for (auto &edge : edgePairs)
    // {
    //     Vertex v1, v2;
    //     v1.position = positions[edge[0]];
    //     v1.normal = Vector3D(0, 0, 0);
    //     v1.color = edgeColor;

    //     v2.position = positions[edge[1]];
    //     v2.normal = Vector3D(0, 0, 0);
    //     v2.color = edgeColor;

    //     mesh.edgeVertices.push_back(v1);
    //     mesh.edgeVertices.push_back(v2);

    //     mesh.edgeIndices.push_back(mesh.edgeVertices.size() - 2);
    //     mesh.edgeIndices.push_back(mesh.edgeVertices.size() - 1);
    // }

    return mesh;
}

MeshData CreateRect(Color color = 0xFFFFFFFF)
{
    MeshData rect;

    Point3D positions[4] = {
        Point3D(-0.5f, -0.5f, 0.0f), // левый нижний
        Point3D(0.5f, -0.5f, 0.0f),  // правый нижний
        Point3D(0.5f, 0.5f, 0.0f),   // правый верхний
        Point3D(-0.5f, 0.5f, 0.0f)   // левый верхний
    };

    Color colors[4] = {
        Color(0xFF0000FF),
        Color(0x00FF00FF),
        Color(0x0000FFFF),
        Color(0xFFFF00FF)};

    // Нормали (для 2D не важны, но нужны для структуры)
    Vector3D normal(0, 0, 1);

    // Добавляем вершины
    for (int i = 0; i < 4; i++)
    {
        Vertex v;
        v.position = positions[i];
        v.normal = normal;
        v.color = color;
        rect.vertices.push_back(v);
    }

    // Индексы для двух треугольников (прямоугольник)
    // Первый треугольник: 0-1-2
    rect.indices.push_back(0);
    rect.indices.push_back(1);
    rect.indices.push_back(2);

    // Второй треугольник: 0-2-3
    rect.indices.push_back(0);
    rect.indices.push_back(2);
    rect.indices.push_back(3);

    return rect;
}

int main()
{
    Canvas3D canvas(800, 600);
    MeshData cube = CreateCube();

    MeshData rect = CreateRect(0x0000FF4F);
    MeshData rect2 = CreateRect(0x00FF004F);

    float angle = 0;

    canvas.Run(
        [&cube, &rect, &rect2, &angle](ICanvas3D &canvas, float deltatime)
        {
            static float fpsTimer = 0.0f;
            static int frameCount = 0;
            static int currentFPS = 0;

            frameCount++;
            fpsTimer += deltatime;

            if (fpsTimer >= 1.0f)
            {
                currentFPS = (frameCount / fpsTimer);
                std::cout << "FPS: " << currentFPS << " | Frame time: " << deltatime * 1000.0f << " ms" << std::endl;

                frameCount = 0;
                fpsTimer = 0.0f;
            }

            canvas.GetTransform().PushMatrix();
            canvas.GetTransform().Translate(0, 0, 6);
            canvas.GetTransform().RotateZ(45);

            canvas.DrawMesh(rect2);
            canvas.GetTransform().PopMatrix();

            canvas.GetTransform().PushMatrix();
            canvas.GetTransform().Translate(-1, 0, 5);
            canvas.GetTransform().RotateZ(45);

            canvas.DrawMesh(rect);
            canvas.GetTransform().PopMatrix();
            angle += 30 * deltatime;
        });

    return 0;
}
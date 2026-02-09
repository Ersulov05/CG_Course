#pragma once

#include "../../controller/AppController.h"
#include "../../model/common/Point.h"
#include "../canvas/CCanvas.h"
#include "./DragState.h"
#include <memory>
#include <iostream>
#include <optional>

class SceneView
{
public:
    SceneView(AppController &controller, Scene &scene)
        : m_controller(controller),
          m_sceneModel(scene)
    {
        m_canvas = std::make_unique<CCanvas>(800, 600);
    }

    void Run()
    {
        m_canvas->RunWindow(
            [this](ICanvas &canvas)
            {
                this->Update();
                this->Render(canvas);
            },
            [this](sf::Event &event)
            {
                this->ProcessEvents(event);
            });
    }

private:
    std::unique_ptr<CCanvas> m_canvas;
    const Scene &m_sceneModel;
    DragState m_dragState;
    std::optional<unsigned> m_dragPictureId;
    AppController &m_controller;

    void Update()
    {
    }

    void ProcessEvents(sf::Event &event)
    {
        switch (event.type)
        {
        case sf::Event::MouseButtonPressed:
            HandleMousePress(event.mouseButton);
            break;

        case sf::Event::MouseButtonReleased:
            HandleMouseRelease(event.mouseButton);
            break;

        case sf::Event::MouseMoved:
            HandleMouseMove(event.mouseMove);
            break;

        case sf::Event::MouseLeft:
            HandleResetDrag();
            break;

        default:
            break;
        }
    }

    void HandleResetDrag()
    {
        if (m_dragState.IsDragging())
        {
            auto dragging = m_dragState.GetDragging();
            if (m_dragPictureId.has_value())
            {
                m_controller.MovePicture(m_dragPictureId.value(), dragging.x, dragging.y);
            }
            m_dragState.ResetDrag();
        }
    }

    void HandleMousePress(const sf::Event::MouseButtonEvent &mouseButton)
    {
        if (mouseButton.button == sf::Mouse::Left)
        {
            Point mousePos(mouseButton.x, mouseButton.y);
            m_dragPictureId = m_controller.GetPictureIdByPoint(mousePos);
            if (m_dragPictureId.has_value())
            {
                m_dragState.StartDrag(mousePos);
            }
        }
    }

    void HandleMouseRelease(const sf::Event::MouseButtonEvent &mouseButton)
    {
        if (mouseButton.button == sf::Mouse::Left)
        {
            HandleResetDrag();
        }
    }

    void HandleMouseMove(const sf::Event::MouseMoveEvent &mouseMove)
    {
        m_dragState.Move(Point(mouseMove.x, mouseMove.y));
    }

    void Render(ICanvas &canvas)
    {
        auto pictures = m_controller.GetPictures();

        for (const auto &picture : pictures)
        {
            RenderPicture(canvas, picture);
        }
    }

    void RenderPicture(ICanvas &canvas, std::shared_ptr<const Picture> picture)
    {
        auto shapes = picture->GetShapes();

        Point offset =
            (m_dragState.IsDragging() &&
             m_dragPictureId.has_value() &&
             picture->GetId() == m_dragPictureId.value())
                ? m_dragState.GetDragging()
                : Point();

        for (const auto &shape : shapes)
        {
            RenderShape(canvas, shape, offset);
        }
    }

    void RenderShape(ICanvas &canvas, std::shared_ptr<const IShape> shape, Point offset)
    {
        canvas.SetColor(shape->GetFillColor());
        auto offsetPoints = GetOffsetPoints(shape->GetPoints(), offset);
        canvas.FillPolygon(offsetPoints);
        canvas.SetColor(shape->GetOutlineColor());
        canvas.DrawPolygon(offsetPoints);
    }

    std::vector<Point> GetOffsetPoints(const std::vector<Point> &points, Point offset)
    {
        if (offset.x == 0 || offset.y == 0)
        {
            return points;
        }
        std::vector<Point> result;

        for (const Point &point : points)
        {
            result.push_back(Point(
                point.x + offset.x,
                point.y + offset.y));
        }

        return result;
    }
};
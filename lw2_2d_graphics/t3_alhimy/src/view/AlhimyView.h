#pragma once
#include "../model/AlhimyModel.h"
#include "./Canvas/CCanvas.h"
#include "./Common/Point.h"
#include "./Popup/Popup.h"
#include "./Button/Button.h"
#include <memory>
#include <iostream>
#include "./DragElementView.h"
#include "./ElementView.h"
#include <memory>
#include "./Constants.h"
#include "./GridPositionManager.h"
#include "./Notification.h"

class AlhimyView
{
public:
    const unsigned LETTER_SIZE = 24;

    AlhimyView(AlhimyModel &alhimyModel)
        : m_alhimyModel(alhimyModel),
          m_canvas(Constants::WIDTH, Constants::HEIGHT),
          m_endGamePopup(m_canvas),
          m_gridPositionManager((Constants::WIDTH / 2 - (Constants::GRID_COLS * Constants::ELEMENT_SIZE.width + (Constants::GRID_COLS - 1) * Constants::GRID_SPACING)) / 2, 100)
    {
        ArrangeElementsInGrid();
        m_alhimyModel.OnEndGameSubscribe(
            [this]()
            { this->m_endGamePopup.Open(); });
        m_sortButton = std::make_unique<Button>(
            m_canvas, Point{Constants::WIDTH / 4 - 100, 500}, Size{150, 20}, L"Сортировать", 0xFFFFFFFF, 0xFFFFFFFF,
            [this]()
            {
                SortElements();
            });

        m_clearButton = std::make_unique<Button>(
            m_canvas, Point{Constants::WIDTH * 3 / 4 - 100, 500}, Size{150, 20}, L"Очистить", 0xFFFFFFFF, 0xFFFFFFFF,
            [this]()
            {
                this->m_dragElements.clear();
            });
    }

    void Run()
    {
        unsigned zIndex = 1;
        m_canvas.RunWindow(
            [this, &zIndex](ICanvas &canvas)
            {
                UpdateNotifications();
                m_canvas.SetColor(0x888888FF);
                m_canvas.FillRect({0, 0}, {Constants::WIDTH, Constants::HEIGHT});
                DrawElementsArea();
                DrawExperimentArea();
                for (auto &element : m_elements)
                {
                    element->Draw(zIndex);
                }
                for (auto &dragElement : m_dragElements)
                {
                    dragElement->Draw(zIndex);
                }
                m_sortButton->Draw(zIndex);
                m_clearButton->Draw(zIndex);
                if (m_endGamePopup.IsOpened())
                {
                    m_endGamePopup.Draw(zIndex);
                }
                DrawNotifications();
            });
    }

private:
    CCanvas m_canvas;
    AlhimyModel &m_alhimyModel;
    GridPositionManager m_gridPositionManager;
    std::vector<std::unique_ptr<DragElementView>> m_dragElements;
    std::vector<std::unique_ptr<ElementView>> m_elements;
    Popup m_endGamePopup;
    std::unique_ptr<Button> m_sortButton;
    std::unique_ptr<Button> m_clearButton;
    std::vector<Notification> m_notifications;

    bool
    IsIncludeExperimentArea(const Point &position, const Size &size)
    {
        return position.x >= Constants::EXPERIMENT_AREA_POSITION.x &&
               position.x + size.width <= Constants::EXPERIMENT_AREA_POSITION.x + Constants::EXPERIMENT_AREA_SIZE.width &&
               position.y >= Constants::EXPERIMENT_AREA_POSITION.y &&
               position.y + size.height <= Constants::EXPERIMENT_AREA_POSITION.y + Constants::EXPERIMENT_AREA_SIZE.height;
    }

    void DrawElementsArea()
    {
        m_canvas.SetColor(0xFFFFFFFF);
        m_canvas.DrawRect({1, 1}, {Constants::WIDTH / 2 - 2, Constants::HEIGHT - 2});
    }

    void DrawExperimentArea()
    {
        m_canvas.SetColor(0xFFFFFFFF);
        m_canvas.DrawRect(Constants::EXPERIMENT_AREA_POSITION, Constants::EXPERIMENT_AREA_SIZE);
    }

    void DragElementReleaseHandle(DragElementView *elementPtr)
    {
        if (IsIncludeExperimentArea(elementPtr->GetPosition(), elementPtr->GetSize()))
        {
            auto firstElementName = elementPtr->GetElement().GetName();
            auto intersectElementsPtr = GetIntersectDragElements(elementPtr);
            for (const auto &intersectElementPtr : intersectElementsPtr)
            {
                auto secondElementName = intersectElementPtr->GetElement().GetName();
                auto createdElementsData = m_alhimyModel.MixElements(firstElementName, secondElementName);

                if (!createdElementsData.createdElements.empty())
                {
                    std::wstring notificationText = firstElementName + L" + " + secondElementName + L" = ";

                    for (size_t i = 0; i < createdElementsData.createdElements.size(); ++i)
                    {
                        if (i > 0)
                            notificationText += L", ";
                        notificationText += createdElementsData.createdElements[i].GetName();
                    }

                    ShowNotification(notificationText);
                }

                for (auto &newElement : createdElementsData.newlyCreatedElements)
                {
                    auto position = m_gridPositionManager.GetNext();
                    CreateElementView(newElement, position, true);
                }
                // ShowNotification(firstElementName + L" + " + secondElementName + L" = " +);
                if (createdElementsData.createdElements.size() != 0)
                {
                    for (auto &element : createdElementsData.createdElements)
                    {
                        CreateDragElementView(element, elementPtr->GetPosition());
                    }
                    RemoveDragElement(elementPtr);
                    RemoveDragElement(intersectElementPtr);
                    break;
                }
            }
            return;
        }

        RemoveDragElement(elementPtr);
    }

    void RemoveDragElement(const DragElementView *elementToRemove)
    {
        auto it = std::find_if(
            m_dragElements.begin(), m_dragElements.end(),
            [elementToRemove](const auto &element)
            {
                return element.get() == elementToRemove;
            });

        if (it != m_dragElements.end())
        {
            m_dragElements.erase(it);
        }
    }

    void MoveDragElementToEnd(DragElementView *elementToMove)
    {
        auto it = std::find_if(
            m_dragElements.begin(), m_dragElements.end(),
            [elementToMove](const auto &element)
            {
                return element.get() == elementToMove;
            });

        if (it != m_dragElements.end())
        {
            auto element = std::move(*it);

            m_dragElements.erase(it);
            m_dragElements.push_back(std::move(element));
        }
    }

    std::vector<const DragElementView *> GetIntersectDragElements(DragElementView *elementToMove)
    {
        std::vector<const DragElementView *> intersectingElements;

        Point elementPos = elementToMove->GetPosition();
        Size elementSize = elementToMove->GetSize();

        for (auto &element : m_dragElements)
        {
            if (element.get() == elementToMove)
                continue;

            if (IsIntersecting(elementPos, elementSize, element->GetPosition(), element->GetSize()))
            {
                intersectingElements.push_back(element.get());
            }
        }

        return intersectingElements;
    }

    bool IsIntersecting(const Point &pos1, const Size &size1, const Point &pos2, const Size &size2) const
    {
        bool xOverlap = (pos1.x < pos2.x + size2.width) &&
                        (pos2.x < pos1.x + size1.width);

        bool yOverlap = (pos1.y < pos2.y + size2.height) &&
                        (pos2.y < pos1.y + size1.height);

        return xOverlap && yOverlap;
    }

    void ArrangeElementsInGrid()
    {
        const auto &elements = m_alhimyModel.GetCreatedElements();

        for (const auto &element : elements)
        {
            auto position = m_gridPositionManager.GetNext();
            CreateElementView(element, position, true);
        }
    }

    void CreateElementView(const Element &element, const Point &position, bool isPressed = false)
    {
        m_elements.emplace_back(
            std::make_unique<ElementView>(
                m_canvas, element, position,
                [this, isPressed](const Point &position, const Element &element)
                {
                    CreateDragElementView(element, position, isPressed);
                }));
    }

    void CreateDragElementView(const Element &element, const Point &position, bool isPressed = false)
    {
        m_dragElements.emplace_back(std::make_unique<DragElementView>(
            m_canvas, element, position,
            [this](DragElementView *elementPtr)
            {
                DragElementReleaseHandle(elementPtr);
            },
            [this](DragElementView *elementPtr)
            {
                MoveDragElementToEnd(elementPtr);
            },
            isPressed));
    }

    void SortElements()
    {
        std::sort(
            m_elements.begin(), m_elements.end(),
            [](const auto &a, const auto &b)
            {
                return a->GetElement().GetName() < b->GetElement().GetName();
            });

        m_gridPositionManager.Reset();

        for (auto &element : m_elements)
        {
            auto position = m_gridPositionManager.GetNext();
            element->SetPosition(position);
        }
    }

    void ShowNotification(const std::wstring &text)
    {
        m_notifications.emplace_back(Notification(text, {Constants::WIDTH / 2, 500}));
    }

    void UpdateNotifications()
    {
        m_notifications.erase(
            std::remove_if(m_notifications.begin(), m_notifications.end(),
                           [](const auto &notification)
                           {
                               return notification.IsExpired();
                           }),
            m_notifications.end());
    }

    void DrawNotifications()
    {
        for (auto &notification : m_notifications)
        {
            notification.Draw(m_canvas);
        }
    }
};
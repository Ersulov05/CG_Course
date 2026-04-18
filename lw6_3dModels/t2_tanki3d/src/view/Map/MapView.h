#pragma once
#include "../Canvas/ICanvas3D.h"
#include "../../model/Map/Map.h"
#include "./TerrarianView.h"
#include "./WallView.h"
#include "./HeadquartersView.h"

class MapView {
public:
    static void Draw(ICanvas3D& canvas, const Map& map) 
    {
        for (auto& terrarian : map.GetTerrarians()) 
        {
            TerrarianView::Draw(canvas, terrarian);
        }
        for (auto& wall : map.GetWalls()) 
        {
            WallView::Draw(canvas, wall);
        }
        HeadquartersView::Draw(canvas, map.GetHeadquarters());
    }
private:
};
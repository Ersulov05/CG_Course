#pragma once
#include "../Canvas/ICanvas3D.h"
#include "../../model/Map/Map.h"
#include "./TerrarianView.h"
#include "./WallView.h"
#include "./HeadquartersView.h"
#include "../Canvas/FpvCamera3D.h"

class MapView {
public:
    static void Draw(ICanvas3D& canvas, const Map& map, const FpvCamera3D& camera) 
    {
        for (auto& terrarian : map.GetTerrarians()) 
        {
            TerrarianView::Draw(canvas, terrarian);
        }
        for (auto& wall : map.GetWalls()) 
        {
            WallView::Draw(canvas, wall);
        }
        for (auto& wall : map.GetHeadquartersWalls()) 
        {
            WallView::Draw(canvas, wall);
        }
        HeadquartersView::Draw(canvas, map.GetHeadquarters(), camera);
    }
private:
};
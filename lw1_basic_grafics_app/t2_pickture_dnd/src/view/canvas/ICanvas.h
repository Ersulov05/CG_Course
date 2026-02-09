#pragma once

#include "../../model/common/Point.h"
#include "../../model/common/Color.h"
#include <iomanip>
#include <vector>

class ICanvas
{
public:
	virtual ~ICanvas() = default;
	virtual void DrawPolygon(const std::vector<Point> &points) = 0;
	virtual void FillPolygon(const std::vector<Point> &points) = 0;
	virtual void Clear(Color color = 0x000000FF) = 0;
	virtual void Display() = 0;
	virtual void SetColor(Color color) = 0;
};

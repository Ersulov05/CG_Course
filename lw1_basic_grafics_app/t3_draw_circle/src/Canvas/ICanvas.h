#pragma once

#include "../Common/Color.h"
#include "../Common/Point.h"
#include <iomanip>
#include <vector>

class ICanvas
{
public:
	virtual ~ICanvas() = default;
	virtual void DrawCircle(const Point &center, unsigned radius, unsigned size = 1, bool smoothing = false) = 0;
	virtual void FillCircle(const Point &center, unsigned radius, bool smoothing = false) = 0;
	virtual void Clear(Color color = 0x000000FF) = 0;
	virtual void Display() = 0;
	virtual void SetColor(Color color) = 0;
};

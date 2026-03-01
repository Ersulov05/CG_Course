#pragma once

#include "../../common/Point.h"
#include "../../common/Color.h"
#include "../../common/Size.h"
#include "./MouseController.h"
#include "./KeyboardController.h"
#include <iomanip>
#include <vector>

class ICanvas
{
public:
	virtual ~ICanvas() = default;
	virtual unsigned GetWidth() const = 0;
	virtual unsigned GetHeight() const = 0;
	virtual void DrawLine(const Point &p0, const Point &p1, float thickness = 1.0f) = 0;
	virtual void DrawPolygon(const std::vector<Point> &points, float thickness = 1.0f, bool closed = true) = 0;
	virtual void FillPolygon(const std::vector<Point> &points) = 0;
	virtual void DrawCircle(const Point &center, float radius, float thickness = 1.0f, int segments = 32) = 0;
	virtual void FillCircle(const Point &center, float radius, int segments = 32) = 0;
	virtual void DrawRect(const Point &position, const Size &size, float thickness = 1.0f) = 0;
	virtual void FillRect(const Point &position, const Size &size) = 0;
	virtual void SetColor(Color color) = 0;
	virtual MouseController &GetMouseController() = 0;
	virtual KeyboardController &GetKeyboardController() = 0;

	virtual void PushMatrix() = 0;
	virtual void PopMatrix() = 0;
	virtual void Translate(float x, float y) = 0;
	virtual void Translate(const Point &p) = 0;
	virtual void Rotate(float angleDegrees) = 0;
	virtual void Scale(float scaleX, float scaleY) = 0;
	virtual void Scale(float scale) = 0;
	virtual void Scale(const Point &scale) = 0;
	virtual void ResetTransform() = 0;
};

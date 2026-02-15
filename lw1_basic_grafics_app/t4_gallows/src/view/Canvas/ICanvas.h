#pragma once

#include "../Common/Point.h"
#include "../Common/Color.h"
#include "../Common/Size.h"
#include "./MouseController.h"
#include <iomanip>
#include <vector>

class ICanvas
{
public:
	virtual ~ICanvas() = default;
	virtual unsigned GetWidth() = 0;
	virtual unsigned GetHeight() = 0;
	virtual void DrawChar(wchar_t c, const Point &position, unsigned int characterSize = 30) = 0;
	virtual void DrawString(const std::wstring &text, const Point &position, unsigned int characterSize = 30) = 0;
	virtual Size GetTextSize(const std::wstring &text, unsigned int characterSize) const = 0;
	virtual Size GetTextSize(wchar_t c, unsigned int characterSize) const = 0;
	virtual void DrawPolygon(const std::vector<Point> &points) = 0;
	virtual void FillPolygon(const std::vector<Point> &points) = 0;
	virtual void DrawRect(const Point &position, const Size &size) = 0;
	virtual void FillRect(const Point &position, const Size &size) = 0;
	virtual void DrawEllipse(float cx, float cy, float rx, float ry) = 0;
	virtual void FillEllipse(float cx, float cy, float rx, float ry) = 0;
	virtual void Clear(Color color = 0x000000FF) = 0;
	virtual void Exit() = 0;
	virtual void Display() = 0;
	virtual void SetColor(Color color) = 0;
	virtual MouseController &GetMouseController() = 0;
};

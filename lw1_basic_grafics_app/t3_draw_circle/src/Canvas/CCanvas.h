#pragma once

#include "../Common/Point.h"
#include "../Common/Color.h"
#include "./ICanvas.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <memory>
#include <functional>
#include <cmath>

class CCanvas : public ICanvas
{
public:
	using UpdateCallback = std::function<void(ICanvas &, float)>;

	CCanvas(unsigned width, unsigned height)
		: m_width(width), m_height(height)
	{
		m_renderTexture = std::make_unique<sf::RenderTexture>();
		if (!m_renderTexture->create(width, height))
		{
			throw std::runtime_error("Failed create render texture");
		}
		m_color = 0xFFFFFFFF;
	}

	void DrawCircle(const Point &center, unsigned radius, unsigned size, bool smoothing) override
	{
		unsigned innerRadius =
			size < radius
				? size > 0
					  ? radius - size
					  : radius - 1
				: 0;
		unsigned outerRadius = radius;

		for (int y = 0; y <= radius * 0.7071; ++y)
		{
			float exactInnerX = y < innerRadius
									? sqrtf(innerRadius * innerRadius - y * y)
									: 0.0f;
			float exactOuterX = sqrtf(outerRadius * outerRadius - y * y);

			int innerX = static_cast<int>(exactInnerX + 0.5f);
			int outerX = static_cast<int>(exactOuterX + 0.5f);

			for (int x = innerX; x <= outerX; ++x)
			{
				DrawCirclePoints(center, x, y, m_color);
			}

			if (smoothing)
			{
				if (innerX > 0 && innerRadius > 0)
				{
					SmoothInnerCircleArea(center, radius, exactInnerX, innerX, y);
				}
				SmoothOuterCircleArea(center, radius, exactOuterX, exactOuterX, y);
			}
		}
	}

	void FillCircle(const Point &center, unsigned radius, bool smoothing)
	{
		DrawCircle(center, radius, radius, smoothing);
	}

	void Clear(Color color) override
	{
		m_renderTexture->clear(sf::Color(color));
	}

	void SetColor(Color color)
	{
		m_color = color;
	}

	void Display()
	{
		m_renderTexture->display();
	}

	void RunWindow(UpdateCallback updateCallback)
	{
		sf::RenderWindow window(sf::VideoMode(m_width, m_height), "Canvas");
		window.setFramerateLimit(300);

		sf::Clock clock;

		while (window.isOpen())
		{
			ProcessEvents(window);

			m_renderTexture->clear(sf::Color(0x1A1A1AFF));
			float deltaTime = clock.restart().asSeconds();

			if (updateCallback)
			{
				updateCallback(*this, deltaTime);
			}

			m_renderTexture->display();

			sf::Sprite sprite(m_renderTexture->getTexture());
			window.clear();
			window.draw(sprite);
			window.display();
		}
	}

private:
	std::unique_ptr<sf::RenderTexture> m_renderTexture;
	u_int32_t m_width;
	u_int32_t m_height;
	Color m_color;

	void ProcessEvents(sf::RenderWindow &window)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
			}
		}
	}

	void DrawPixel(const Point &point, Color color)
	{
		sf::RectangleShape rect;
		rect.setPosition(point.x, point.y);
		rect.setFillColor(sf::Color(color));
		rect.setSize(sf::Vector2f(1.0f, 1.0f));
		m_renderTexture->draw(rect);
	}

	void DrawCirclePoints(Point center, int x, int y, Color color)
	{
		DrawPixel({center.x + x, center.y + y}, color);
		DrawPixel({center.x + x, center.y - y}, color);
		DrawPixel({center.x - x, center.y + y}, color);
		DrawPixel({center.x - x, center.y - y}, color);

		DrawPixel({center.x + y, center.y + x}, color);
		DrawPixel({center.x + y, center.y - x}, color);
		DrawPixel({center.x - y, center.y + x}, color);
		DrawPixel({center.x - y, center.y - x}, color);
	}

	void SmoothInnerCircleArea(Point center, int radius, float exactInnerX, int innerX, int y)
	{
		uint8_t r = (m_color >> 24) & 0xFF;
		uint8_t g = (m_color >> 16) & 0xFF;
		uint8_t b = (m_color >> 8) & 0xFF;
		uint8_t a = m_color & 0xFF;

		float pixelLeftEdge = innerX - 1;
		float coverage = exactInnerX - pixelLeftEdge;

		if (coverage > 0.0f && coverage < 1.0f)
		{
			uint8_t aa_a = static_cast<uint8_t>(a * coverage * 0.5);
			Color aaColor = (r << 24) | (g << 16) | (b << 8) | aa_a;

			int x = innerX - 1;
			DrawCirclePoints(center, x, y, aaColor);
		}
	}

	void SmoothOuterCircleArea(Point center, int radius, float exactOuterX, int outerX, int y)
	{
		uint8_t r = (m_color >> 24) & 0xFF;
		uint8_t g = (m_color >> 16) & 0xFF;
		uint8_t b = (m_color >> 8) & 0xFF;
		uint8_t a = m_color & 0xFF;

		float pixelRightEdge = outerX + 1;
		float coverage = pixelRightEdge - exactOuterX;

		if (coverage > 0.0f && coverage < 1.0f)
		{
			uint8_t aa_a = static_cast<uint8_t>(a * coverage * 0.5);
			Color aaColor = (r << 24) | (g << 16) | (b << 8) | aa_a;

			int x = outerX + 1;
			if (x <= static_cast<int>(radius))
			{
				DrawCirclePoints(center, x, y, aaColor);
			}
		}
	}
};
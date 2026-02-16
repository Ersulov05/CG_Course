#pragma once

#include "../Common/Point.h"
#include "../Common/Size.h"
#include "../Common/Color.h"
#include "./ICanvas.h"
#include "./MouseController.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <memory>
#include <functional>
#include <optional>

class CCanvas : public ICanvas
{
public:
	using UpdateCallback = std::function<void(ICanvas &)>;

	CCanvas(unsigned width = 800, unsigned height = 600)
		: m_width(width), m_height(height)
	{
		m_renderTexture = std::make_unique<sf::RenderTexture>();
		if (!m_renderTexture->create(width, height))
		{
			throw std::runtime_error("Failed create render texture");
		}
		m_color = 0xFFFFFFFF;

		if (!m_font.loadFromFile("Strogo.ttf"))
		{
			std::cerr << "Warning: Could not load font arial.ttf" << std::endl;
		}
	}

	unsigned GetWidth()
	{
		return m_width;
	}

	unsigned GetHeight()
	{
		return m_height;
	}

	void DrawPolygon(const std::vector<Point> &points) override
	{

		if (points.size() < 3)
			return;

		sf::ConvexShape polygon;
		polygon.setPointCount(points.size());
		polygon.setFillColor(sf::Color::Transparent);
		polygon.setOutlineColor(sf::Color(m_color));
		polygon.setOutlineThickness(1.0f);

		for (size_t i = 0; i < points.size(); ++i)
		{
			polygon.setPoint(i, sf::Vector2f(points[i].x, points[i].y));
		}

		m_renderTexture->draw(polygon);
	}

	void DrawRect(const Point &position, const Size &size)
	{
		DrawPolygon({{position.x, position.y},
					 {position.x + size.width, position.y},
					 {position.x + size.width, position.y + size.height},
					 {position.x, position.y + size.height}});
	}

	void FillRect(const Point &position, const Size &size)
	{
		FillPolygon({{position.x, position.y},
					 {position.x + size.width, position.y},
					 {position.x + size.width, position.y + size.height},
					 {position.x, position.y + size.height}});
	}

	void FillPolygon(const std::vector<Point> &points) override
	{
		if (points.size() < 3)
			return;

		sf::ConvexShape polygon;
		polygon.setPointCount(points.size());
		polygon.setFillColor(sf::Color(m_color));

		for (size_t i = 0; i < points.size(); ++i)
		{
			polygon.setPoint(i, sf::Vector2f(points[i].x, points[i].y));
		}

		m_renderTexture->draw(polygon);
	}

	void DrawEllipse(float cx, float cy, float rx, float ry)
	{
		sf::ConvexShape ellipse;
		const int points = 100;

		ellipse.setFillColor(sf::Color::Transparent);
		ellipse.setOutlineColor(sf::Color(m_color));
		ellipse.setOutlineThickness(2);
		ellipse.setPointCount(points);
		for (int i = 0; i < points; ++i)
		{
			double angle = 2 * M_PI * i / points;
			double x = cx + rx * cos(angle);
			double y = cy + ry * sin(angle);
			ellipse.setPoint(i, sf::Vector2f(x, y));
		}

		m_renderTexture->draw(ellipse);
	}

	void FillEllipse(float cx, float cy, float rx, float ry)
	{
		sf::ConvexShape ellipse;
		const int points = 100;

		ellipse.setFillColor(sf::Color(m_color));
		ellipse.setOutlineColor(sf::Color(m_color));
		ellipse.setOutlineThickness(2);
		ellipse.setPointCount(points);
		for (int i = 0; i < points; ++i)
		{
			double angle = 2 * M_PI * i / points;
			double x = cx + rx * cos(angle);
			double y = cy + ry * sin(angle);
			ellipse.setPoint(i, sf::Vector2f(x, y));
		}

		m_renderTexture->draw(ellipse);
	}

	Size GetTextSize(const std::wstring &text, unsigned int characterSize) const
	{
		if (text.empty() || !m_font.hasGlyph(text[0]))
			return {0, 0};

		sf::Text tempText(text, m_font, characterSize);
		sf::FloatRect bounds = tempText.getLocalBounds();

		return {bounds.width, bounds.height};
	}

	Size GetTextSize(wchar_t c, unsigned int characterSize) const
	{
		std::wstring wstr(1, c);
		return GetTextSize(wstr, characterSize);
	}

	void DrawString(const std::wstring &text, const Point &position, unsigned int characterSize) override
	{
		if (text.empty())
			return;

		sf::Text sfText(text, m_font, characterSize);
		sf::FloatRect bounds = sfText.getLocalBounds();
		sfText.setFillColor(sf::Color(m_color));

		float baselineOffset = m_font.getUnderlinePosition(characterSize);

		sfText.setPosition(sf::Vector2f(position.x, position.y - baselineOffset));

		m_renderTexture->draw(sfText);
	}

	void DrawChar(wchar_t c, const Point &position, unsigned int characterSize) override
	{
		std::wstring wstr(1, c);
		DrawString(wstr, position, characterSize);
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
		m_window.emplace(sf::VideoMode(m_width, m_height), "Canvas");
		auto &window = m_window.value();
		window.setFramerateLimit(300);

		sf::Clock clock;

		while (window.isOpen())
		{
			ProcessEvents(window);

			m_renderTexture->clear(sf::Color(0x1A1A1AFF));
			float deltaTime = clock.restart().asSeconds();

			if (updateCallback)
			{
				updateCallback(*this);
			}

			m_renderTexture->display();

			sf::Sprite sprite(m_renderTexture->getTexture());
			window.clear();
			window.draw(sprite);
			window.display();
		}
	}

	MouseController &GetMouseController()
	{
		return m_mouseController;
	}

	void Exit()
	{
		if (m_window.has_value())
		{
			m_window.value().close();
		}
	}

private:
	std::unique_ptr<sf::RenderTexture> m_renderTexture;
	u_int32_t m_width;
	u_int32_t m_height;
	Color m_color;
	MouseController m_mouseController;
	sf::Font m_font;
	std::optional<sf::RenderWindow> m_window;

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
			m_mouseController.ProcessEvents(event);
		}
	}
};
#pragma once

#include "../Common/Point.h"
#include "../Common/Color.h"
#include "./ICanvas.h"
#include "./MouseController.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <memory>
#include <functional>

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

		if (!m_font.loadFromFile("arialmt.ttf"))
		{
			std::cerr << "Warning: Could not load font arial.ttf" << std::endl;
		}
	}

	void DrawPolygon(const std::vector<Point> &points) override
	{
		if (points.size() < 2)
			return;

		sf::VertexArray lines(sf::LineStrip, points.size());
		for (size_t i = 0; i < points.size(); ++i)
		{
			lines[i].position = sf::Vector2f(points[i].x, points[i].y);
			lines[i].color = sf::Color(m_color);
		}

		m_renderTexture->draw(lines);
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

	// void DrawString(const std::string &text, const Point &position, unsigned int characterSize = 30)
	// {
	// 	if (text.empty())
	// 		return;

	// 	sf::Text sfText(sf::String::fromUtf8(text.begin(), text.end()), m_font, characterSize);
	// 	sfText.setFillColor(sf::Color(m_color));
	// 	sfText.setPosition(sf::Vector2f(position.x, position.y));

	// 	m_renderTexture->draw(sfText);
	// }

	void DrawWideString(const std::wstring &text, const Point &position, unsigned int characterSize = 30)
	{
		if (text.empty())
			return;

		sf::Text sfText(text, m_font, characterSize);
		sfText.setFillColor(sf::Color(m_color));
		sfText.setPosition(sf::Vector2f(position.x, position.y));

		m_renderTexture->draw(sfText);
	}

	void DrawChar(wchar_t c, const Point &position, unsigned int characterSize = 30)
	{
		std::wstring wstr(1, c);
		DrawWideString(wstr, position, characterSize);
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

private:
	std::unique_ptr<sf::RenderTexture> m_renderTexture;
	u_int32_t m_width;
	u_int32_t m_height;
	Color m_color;
	MouseController m_mouseController;
	sf::Font m_font;

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
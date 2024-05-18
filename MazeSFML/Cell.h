#pragma once
#include "SFML/Graphics.hpp"
#include <vector>

#define SIZE 20

class Cell
{
private:
	float relativeX, relativeY;
	unsigned int row, column;
	bool visited;
	sf::RectangleShape* rec;

public:
	Cell* parent;
	unsigned int value;
	bool path;
	bool wall[4];

	Cell(const unsigned int& row, const unsigned int&  column);
	virtual ~Cell();
	void setVisited(const bool& visited);
	bool getVisited() const;
	void setRelativePos(const float& x, const float& y);
	sf::Vector2i getRowCol() const;
	void updateWall(int h, int v);
	void render(sf::RenderTarget* window, const float& width, sf::RectangleShape* line, const sf::Color& color);
	void renderBlock(sf::RenderTarget* window, const sf::Color& color, const float& width);
	void updateVLine(std::vector<sf::RectangleShape*>& vline, const float& width);
};


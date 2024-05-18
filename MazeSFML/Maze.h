#pragma once
#include "Cell.h"
#include <vector>

#define WIDTH (400.f / (float)SIZE)

class Maze
{
private:
	float relativeX, relativeY;
	sf::RectangleShape* line;
	Cell* current;
	std::vector<Cell*> stack;

	void mazeGenerate();
	void searchSP(Cell* cell);
	void drawPath();

public:
	bool status[4];
	std::vector<Cell*> grid;
	std::vector<sf::RectangleShape*> vline;
	Maze();
	virtual ~Maze();

	void setRelativePos(const int& x, const int& y);
	int index(const int& j, const int& i);
	void update(float& dt);
	void render(sf::RenderTarget* window);
};


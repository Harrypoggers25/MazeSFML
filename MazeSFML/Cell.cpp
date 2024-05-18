#include "Cell.h"

Cell::Cell(const unsigned int& row, const unsigned int& column)
{
	relativeX = 0.f, relativeY = 0.f;
	parent = nullptr;
	value = SIZE * SIZE;
	path = false;

	this->row = row;
	this->column = column;
	visited = false;
	wall[0] = true; //top
	wall[1] = true; //right
	wall[2] = true; //bottom
	wall[3] = true; //left

	rec = new sf::RectangleShape();
}

Cell::~Cell()
{
	delete rec;
}

void Cell::setVisited(const bool& visited)
{
	this->visited = visited;
}

bool Cell::getVisited() const
{
	return visited;
}

void Cell::setRelativePos(const float& x, const float& y)
{
	relativeX = x, relativeY = y;
}

sf::Vector2i Cell::getRowCol() const
{
	return sf::Vector2i((int)column, (int)row);
}

void Cell::updateWall(int h, int v)
{
	if (v == -1) wall[0] = false;
	else if (h == 1) wall[1] = false;
	else if (v == 1) wall[2] = false;
	else if (h == -1) wall[3] = false;
}

void Cell::render(sf::RenderTarget* window, const float& width, sf::RectangleShape* line, const sf::Color& color)
{
	float i = (float)this->row;
	float j = (float)this->column;

	if (this->visited) renderBlock(window, color, width);
	else renderBlock(window, sf::Color::Black, width);

	if (wall[0]) {
		line->setPosition(j * width, i * width);
		line->move(relativeX, relativeY);
		line->setSize(sf::Vector2f(width, 1));
		window->draw(*line);
	}
	if (wall[1]) {
		line->setPosition((j + 1) * width, i * width);
		line->move(relativeX, relativeY);
		line->setSize(sf::Vector2f(1, width));
		window->draw(*line);
	}
	if (wall[2]) {
		line->setPosition(j * width, (i + 1) * width);
		line->move(relativeX, relativeY);
		line->setSize(sf::Vector2f(width, 1));
		window->draw(*line);
	}
	if (wall[3]) {
		line->setPosition(j * width, i * width);
		line->move(relativeX, relativeY);
		line->setSize(sf::Vector2f(1, width));
		window->draw(*line);
	}
}

void Cell::renderBlock(sf::RenderTarget* window, const sf::Color& color, const float& width)
{
	float i = (float)this->row;
	float j = (float)this->column;

	rec->setPosition(j * width, i * width);
	rec->move(relativeX, relativeY);
	rec->setSize(sf::Vector2f(width, width));

	this->rec->setFillColor(color);
	window->draw(*this->rec);
}

void Cell::updateVLine(std::vector<sf::RectangleShape*>& vline, const float& width)
{
	float y = (float)this->row;
	float x = (float)this->column;
	sf::RectangleShape* line;

	for (size_t i = 0; i < 4; i++) {
		if (wall[i]) {
			vline.push_back(new sf::RectangleShape());
			vline.back()->setFillColor(sf::Color::White);
			line = vline.back();
			if (i == 0) {
				line->setPosition(x * width, y * width);
				line->move(relativeX, relativeY);
				line->setSize(sf::Vector2f(width, 1));
			}
			if (i == 1) {
				line->setPosition((x + 1) * width, y * width);
				line->move(relativeX, relativeY);
				line->setSize(sf::Vector2f(1, width));
			}
			if (i == 2) {
				line->setPosition(x * width, (y + 1) * width);
				line->move(relativeX, relativeY);
				line->setSize(sf::Vector2f(width, 1));
			}
			if (i == 3) {
				line->setPosition(x * width, y * width);
				line->move(relativeX, relativeY);
				line->setSize(sf::Vector2f(1, width));
			}
		}
	}
}


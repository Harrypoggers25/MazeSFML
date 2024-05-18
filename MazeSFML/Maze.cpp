#include "Maze.h"
#include <cstdlib>
#include <iostream>

#define END SIZE * SIZE - 1

Maze::Maze()
{
	relativeX = 0, relativeY = 0;
	status[0] = false;
	status[1] = false;
	status[2] = false;
	status[3] = false;

	std::srand((unsigned)time(NULL));

	for (size_t i = 0; i < SIZE; i++) {
		for (size_t j = 0; j < SIZE; j++) {
			grid.push_back(new Cell(i, j));
		}
	}
	line = new sf::RectangleShape();
	line->setFillColor(sf::Color::White);

	if (!grid.empty()) {
		stack.push_back(grid[0]);
		current = stack.back();
	}
	else {
		current = nullptr;
	}

}

Maze::~Maze()
{
	std::cout << grid.size() << " Cells deleted" << std::endl;
	std::cout << vline.size() << " Lines deleted" << std::endl;

	size_t size = grid.size();

	for (size_t i = 0; i < size; i++) {
		delete grid.back();
		grid.pop_back();
	}
	delete line;
	for (size_t i = 0; i < vline.size(); i++) {
		delete vline.back();
		vline.pop_back();
	}
}

void Maze::setRelativePos(const int& x, const int& y)
{
	for (size_t i = 0; i < vline.size(); i++) {
		vline[i]->move(-relativeX, -relativeY);
		vline[i]->move(x, y);
	}
	relativeX = x, relativeY = y;
	for (size_t i = 0; i < grid.size(); i++) {
		grid[i]->setRelativePos(x, y);
	}
}

int Maze::index(const int& j, const int& i)
{
	return (unsigned int)j + SIZE * i;
}

void Maze::mazeGenerate()
{
	current->setVisited(true);
	int i = current->getRowCol().y;
	int j = current->getRowCol().x;
	std::vector<Cell*> neighbor;

	if (i > 0 && !grid[index(j, i - 1)]->getVisited()) neighbor.push_back(grid[index(j, i - 1)]);
	if (j < SIZE - 1 && !grid[index(j + 1, i)]->getVisited()) neighbor.push_back(grid[index(j + 1, i)]);
	if (i < SIZE - 1 && !grid[index(j, i + 1)]->getVisited()) neighbor.push_back(grid[index(j, i + 1)]);
	if (j > 0 && !grid[index(j - 1, i)]->getVisited()) neighbor.push_back(grid[index(j - 1, i)]);

	if (!neighbor.empty()) {
		current = neighbor[std::rand() % neighbor.size()];
		stack.push_back(current);
		stack.back()->parent = grid[index(j, i)];

		int h = current->getRowCol().x - j;
		int v = current->getRowCol().y - i;

		current->updateWall(-h, -v);
		grid[index(j, i)]->updateWall(h, v);
	}
	else if (!stack.empty()) stack.pop_back();
	if (!stack.empty()) current = stack.back();
	else for (i = 0; i < grid.size(); i++) {
		grid[i]->setVisited(false);
		if (grid[i]->parent == nullptr) grid[i]->value = 0;
		current = nullptr;
		status[0] = true;
	}
}

void Maze::searchSP(Cell* cell)
{
	for (size_t i = 0; i < grid.size(); i++) if (!grid[i]->getVisited()) current = grid[i];

	for (size_t i = 0; i < grid.size(); i++) {
		if (grid[i]->value <= current->value && !grid[i]->getVisited()) {
			current = grid[i];
		}
	}
	current->setVisited(true);
	if (current != cell) {
		for (size_t i = 1; i < grid.size(); i++) {
			if (grid[i]->parent == current) grid[i]->value = current->value + 1;
		}
	}
	else {
		current->path = true;
		status[1] = true;
	}
}

void Maze::drawPath()
{
	current->path = true;
	current = current->parent;
	if (current == nullptr) {
		status[2] = true;
		for (size_t i = 0; i < grid.size(); i++) {
			if (!grid[i]->path) grid[i]->setVisited(false);
			else stack.push_back(grid[i]);
			grid[i]->updateVLine(vline, WIDTH);
		}
	}
}

void Maze::update(float& dt)
{
	if (dt > 10) {
		if (!status[0]) mazeGenerate();
		else if (!status[1]) searchSP(grid[END]);
		else if (!status[2]) drawPath();
		dt = 0;
	}
}

void Maze::render(sf::RenderTarget* window)
{
	if (status[0] && status[1] && status[2]) {
		for (size_t i = 0; i < stack.size(); i++) if (stack[i]->path) stack[i]->renderBlock(window, sf::Color::Red, WIDTH);
		for (size_t i = 0; i < vline.size(); i++) {
			window->draw(*vline[i]);
		}
	}
	else {
		for (size_t i = 0; i < grid.size(); i++) {
			if (!stack.empty()) grid[i]->render(window, WIDTH, line, sf::Color::Blue);
			else if (grid[i]->path) grid[i]->render(window, WIDTH, line, sf::Color::Red);
			else if (grid[i]->getVisited()) grid[i]->render(window, WIDTH, line, sf::Color::Magenta);
			else grid[i]->render(window, WIDTH, line, sf::Color::Black);
		}
		if (!status[0]) current->renderBlock(window, sf::Color::Magenta, WIDTH);
	}
	
}

#pragma once
#include "SFML/Graphics.hpp"
#include "Maze.h"
#include <iostream>

#define RAD (3.1415926f / 180.f)
#define STEP 0.1f
#define SENSE 0.3f
#define FOV 70.f
#define RES 400.f
#define WALL_H 400.f
#define WALL_W 2.f
#define WALL_H_CONST 6000.f

class Person {
private:
	int mouseX;
	float relativeX, relativeY;
	sf::CircleShape body;
	sf::RectangleShape head;
	sf::RectangleShape* line;
	Maze* maze;
	std::vector<sf::RectangleShape*> vline;
	std::vector<sf::RectangleShape*> vwall;

	bool isHit(float x, float y, uint8_t wall);
	float getDistance(const float& x, const float& y, const float& dx, const float& dy);

public:
	Person(Maze* maze);
	virtual ~Person();
	void setMaze(Maze* maze);
	void setPosition(const float& x, const float& y);
	void setRelativePos(const float& x, const float& y);
	const sf::Vector2f& getPosition() const;
	void move(const float& forward, const float& right);
	void rotate(const float& angle);
	void updateEvent(sf::Event& event);
	void updateOnKeyPress();
	void updateLine();
	void initVWall();
	void render(sf::RenderTarget* window);
	void render3D(sf::RenderTarget* window);
};


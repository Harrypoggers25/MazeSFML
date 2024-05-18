#pragma once
#include "Maze.h"
#include "Person.h"


class State
{
private:
	Maze* maze;
	Person* person;
	sf::Sprite gun;
	sf::Texture* tgun;

public:
	State();
	virtual ~State();

	void setup();
	void updateEvent(sf::Event event);
	void update(float& dt);
	void render(sf::RenderTarget* window);
};


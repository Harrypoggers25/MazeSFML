#pragma once
#include "State.h"

class Game
{
private:
	sf::RenderWindow* window;
	State* state;
	float dt;

public:
	Game();
	virtual ~Game();
	void run();
	void setup();
	void updateEvent();
	void update();
	void render();
};


#include "State.h"


State::State()
{
	maze = new Maze();
	maze->setRelativePos(400.f, 0.f);
	person = new Person(maze);
	person->setRelativePos(400.f, 0.f);

	tgun = new sf::Texture();
	tgun->loadFromFile("awp.png");

	gun.setScale(0.5f, 0.5f);
	gun.setPosition(800.f, 400.f);
	gun.move(-273.5f, -176.f);
	gun.setTexture(*tgun);
}

State::~State()
{
	delete maze;
	delete person;
	delete tgun;
}

void State::setup()
{
}

void State::updateEvent(sf::Event event)
{
	person->updateEvent(event);
}

void State::update(float& dt)
{
	person->updateOnKeyPress();
	maze->update(dt);
	if (maze->status[2] && !maze->status[3]) {
		maze->setRelativePos(800.f, 0.f);
		person->setRelativePos(800.f, 0.f);
		person->initVWall();
		person->updateLine();
		maze->status[3] = true;
	}
}

void State::render(sf::RenderTarget* window)
{
	maze->render(window);
	person->render(window);
	if (maze->status[3]) {
		person->render3D(window);
		window->draw(gun);
	}
}


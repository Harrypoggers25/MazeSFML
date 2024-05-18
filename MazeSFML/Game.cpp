#include "Game.h"

Game::Game()
{
	window = new sf::RenderWindow(sf::VideoMode(1201, 401), "Maze in C++!", sf::Style::Titlebar | sf::Style::Close);
	state = new State();
	window->setMouseCursorVisible(false);
}

Game::~Game()
{
	delete window;
	delete state;
}

void Game::run()
{
	setup();
	sf::Clock clock;
	dt = 0;
	while (window->isOpen()) {
		sf::Mouse::setPosition(sf::Vector2i(window->getPosition().x + 600, window->getPosition().y + 200));
		dt += clock.restart().asMilliseconds();
		updateEvent();
		update();
		render();
	}
}

void Game::setup()
{
	state->setup();
}

void Game::updateEvent()
{
	sf::Event event;
	while (window->pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window->close();
		else if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Escape) window->close();
		}
		state->updateEvent(event);
	}
}

void Game::update()
{
	state->update(dt);
}


void Game::render()
{
	window->clear();
	state->render(window);
	window->display();
}

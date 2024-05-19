#include "Person.h"

Person::Person(Maze* maze)
{
	setMaze(maze);
	body.setFillColor(sf::Color::Green);
	body.setRadius(2.f);
	body.setOrigin(2.f, 2.f);

	head.setFillColor(sf::Color::Green);
	head.setSize(sf::Vector2f(3.f, 2.f));
	head.setOrigin(-2.f, 1.f);

	for (size_t i = 0; i < (size_t)RES; i++) {
		line = new sf::RectangleShape();
		line->setFillColor(sf::Color::Yellow);
		line->setSize(sf::Vector2f(WIDTH, 2.f));
		line->setOrigin(-2.f, 1.f);
		line->setRotation(-FOV / 2.f + i * FOV / RES);
		vline.push_back(line);
	}

	relativeX = 0, relativeY = 0;
	setPosition(WIDTH / 2.f, WIDTH / 2.f);
	rotate(90.f);

	this->mouseX = 0;
}

Person::~Person()
{
	int count = 0;
	while (!vline.empty()) {
		delete vline.back();
		vline.pop_back();
		count++;
	}
	std::cout << count << " Vlines deleted" << std::endl;

	count = 0;
	while (!vwall.empty()) {
		delete vwall.back();
		vwall.pop_back();
		count++;
	}
	std::cout << count << " Vwalls deleted" << std::endl;
}

bool Person::isHit(float x, float y, uint8_t wall)
{
	x = floorf(x / WIDTH);
	y = floorf(y / WIDTH);

	
	//std::cout << " x: " << x << " y: " << y << " wall: " << (int)wall << std::endl;
	int i = maze->index((int) x, (int)y);
	if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) return true;
	else if (maze->grid[i]->wall[(int)wall]) return true;
	
	return false;
}

float Person::getDistance(const float& x, const float& y, const float& dx, const float& dy)
{
	float Dx = dx - x, Dy = dy - y;
	return sqrtf(Dx * Dx + Dy * Dy);
}

void Person::setMaze(Maze* maze)
{
	this->maze = maze;
}

void Person::setPosition(const float& x, const float& y) 
{
	float dx = x, dy = y;
	body.setPosition(dx, dy);
	head.setPosition(dx, dy);
	for (size_t i = 0; i < vline.size(); i++) {
		vline[i]->setPosition(dx, dy);
	}
	//line->setPosition(x, y);
}

void Person::setRelativePos(const float& x, const float& y)
{
	body.move(-relativeX, -relativeY);
	head.move(-relativeX, -relativeY);
	for (size_t i = 0; i < vline.size(); i++) {
		vline[i]->move(-relativeX, -relativeY);
	}
	relativeX = x, relativeY = y;
	body.move(x, y);
	head.move(x, y);
	for (size_t i = 0; i < vline.size(); i++) {
		vline[i]->move(x, y);
	}
}

const sf::Vector2f& Person::getPosition() const
{
	return body.getPosition();
}

void Person::move(const float& forward, const float& right) 
{
	float a = head.getRotation() * RAD;
	float x = body.getPosition().x + forward * cosf(a) + right * sin(a);
	float y = body.getPosition().y + forward * sinf(a) + right * -cos(a);
	setPosition(x, y);

	updateLine();
}

void Person::rotate(const float& angle)
{
	//line->rotate(angle);
	for (size_t i = 0; i < vline.size(); i++) {
		vline[i]->rotate(angle);
	}
	body.rotate(angle);
	head.rotate(angle);

	updateLine();
}

void Person::updateEvent(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed) {
		//if (event.key.code == sf::Keyboard::W) move(STEP, 0.f);
		//if (event.key.code == sf::Keyboard::A) move(0.f, STEP);
		//if (event.key.code == sf::Keyboard::S) move(-STEP, 0.f);
		//if (event.key.code == sf::Keyboard::D) move(0.f, -STEP);
		if (event.key.code == sf::Keyboard::Right) rotate(5.f);
		if (event.key.code == sf::Keyboard::Left) rotate(-5.f);
	}
	if (event.type == sf::Event::MouseMoved) {
		if (this->mouseX == 0) this->mouseX = event.mouseMove.x;
		//std::cout << this->mouseX - event.mouseMove.x << std::endl;
		//if (this->mouseX - event.mouseMove.x > 0) rotate(-5.f * SENSE);
		//if (this->mouseX - event.mouseMove.x < 0) rotate(5.f * SENSE);

		rotate(-(this->mouseX - event.mouseMove.x) * SENSE);
	}
	
}

void Person::updateOnKeyPress()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) move(STEP, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) move(0.f, STEP);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) move(-STEP, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) move(0.f, -STEP);
}

void Person::updateLine()
{
	const float x = body.getPosition().x - relativeX,
				y = body.getPosition().y - relativeY,
				nx = floorf(x / WIDTH),
				ny = floorf(y / WIDTH),
				w = WIDTH;
	for (size_t i = 0; i < vline.size(); i++) {
		const float a = vline[i]->getRotation(),
			tan = tanf(a * RAD),
			cos = cosf((head.getRotation() - a) * RAD),
			atan = 1 / tan;

		float dx = x, dy = y;
	
		// Horizontal
		float dh = 2000.f;
		if (a > 0.f && a < 180.f) {						//BOT
			dy += w * (ny + 1) - y;
			dx += atan * (w * (ny + 1) - y);
			while (!isHit(dx, dy, 0)) {
				dy += w;
				dx += atan * w;
			}
			dh = getDistance(x, y, dx, dy);
		}
		else if (a > 180.f && a < 360.f) {				//TOP
			dy += w * ny - y;
			dx += atan * (w * ny - y);
			while (!isHit(dx, dy - w / 2, 2)) {
				dy += -w;
				dx += -atan * w;
			}
			dh = getDistance(x, y, dx, dy);
		}
	
		dx = x, dy = y;
		//// Vertical
		float dv = 2000.f;
		if (a > 270.f || a < 90) {						//RIGHT
			dx += w * (nx + 1) - x;
			dy += tan * (w * (nx + 1) - x);
			while (!isHit(dx, dy, 3)) {
				dx += w;
				dy += tan * w;
			}
			dv = getDistance(x, y, dx, dy);
		} 
		else if (a > 90.f && a < 270) {					//LEFT
			dx += w * nx - x;
			dy += tan * (w * nx - x);
			while (!isHit(dx - w / 2, dy, 1)) {
				dx += -w;
				dy += -tan * w;
			}
			dv = getDistance(x, y, dx, dy);
		} 
		/*std::cout << "x: " << x << "  y: " << y << std::endl;
		std::cout << "dx: " << dx << " dy: " << dy << std::endl;
		std::cout << "D: " << dh << std::endl << std::endl;*/
	
		//Compare
		float d = 0;
		if (dh < dv) d = dh;
		else d = dv;
		vline[i]->setSize(sf::Vector2f(d, 2.f));

		if (maze->status[2]) {
			d = WALL_H_CONST * 1.f / (d * cos);
			if (d > WALL_H) d = WALL_H;
			float shade = (uint8_t)(255.f * d / WALL_H);
			//if (shade > 255) shade = 255;
			vwall[i]->setSize(sf::Vector2f(WALL_W, d));
			vwall[i]->setOrigin(0.f, d / 2.f);
			vwall[i]->setPosition(i * WALL_W, WALL_H / 2.f);
			vwall[i]->setFillColor(sf::Color(shade, shade, shade)); //red
			//vwall[i]->setFillColor(sf::Color(shade, 0, 0)); //red
			//vwall[i]->setFillColor(sf::Color(0, shade, 0)); //green
			//vwall[i]->setFillColor(sf::Color(0, 0, shade)); //blue
			//vwall[i]->setFillColor(sf::Color(shade, shade, 0)); //yellow
			//vwall[i]->setFillColor(sf::Color(0, shade, shade)); //cyan
			//vwall[i]->setFillColor(sf::Color(shade, 0, shade)); //purple
		}
	}
}

void Person::initVWall()
{
	for (size_t i = 0; i < vline.size(); i++) {
		vwall.push_back(new sf::RectangleShape());
	}
}

void Person::render(sf::RenderTarget* window)
{
	for (size_t i = 0; i < vline.size(); i++) {
		window->draw(*vline[i]);
	}
	window->draw(body);
	window->draw(head);
}

void Person::render3D(sf::RenderTarget* window)
{
	for (size_t i = 0; i < vwall.size(); i++) {
		window->draw(*vwall[i]);
	}
}

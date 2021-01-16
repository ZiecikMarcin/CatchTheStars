#include "SFML-2.5.1\include\SFML\Graphics.hpp"
#include "SFML-2.5.1\include\SFML\Audio.hpp"
#include <vector>
#include "Hero.h"
#include "Enemy.h"
#include "Points.h"

sf::Vector2f viewSize(1024, 768);
sf::VideoMode vm(viewSize.x, viewSize.y);
sf::RenderWindow window(vm, "Catch the Clown!", sf::Style::Default);

sf::Music bgMusic;
sf::SoundBuffer fireBuffer;
sf::SoundBuffer hitBuffer;
sf::Sound fireSound(fireBuffer);
sf::Sound hitSound(hitBuffer);

void spawnPoints();
void changeColor();
bool checkCollision(sf::Sprite sprite1, sf::Sprite sprite2);
void reset();

sf::Texture background;
sf::Sprite backgroundSprite;
sf::Texture parallaxOne;
sf::Sprite parallaxOneSprite;
sf::Texture parallaxTwo;
sf::Sprite parallaxTwoSprite;
sf::Texture stars;
sf::Sprite starsSprite;

sf::Font headingFont;
sf::Text headingText;
sf::Font scoreFont;
sf::Text scoreText;
sf::Text tutorialText;
sf::Text livesText;

Hero hero;
std::vector<Enemy*> enemies;
std::vector<Points*> points;

float currentTime;
float prevTime = 0.0f;
int score = 0;
bool gameover = true;
int currentColor = 0;
int lives = 3;

sf::Texture heroTexture;
sf::Sprite heroSprite;
sf::Vector2f playerPosition;
bool playerMoving = false;

void init() {
	// Audio
	bgMusic.openFromFile("Assets/audio/bgmusic.ogg");
	bgMusic.play();
	hitBuffer.loadFromFile("Assets/audio/hit.wav");
	fireBuffer.loadFromFile("Assets/audio/fire.wav");
	//load texture
	background.loadFromFile("Assets/graphics/background_0.png");
	parallaxOne.loadFromFile("Assets/graphics/background_1.png");
	parallaxTwo.loadFromFile("Assets/graphics/background_2.png");
	stars.loadFromFile("Assets/graphics/enemy.png");
	//attach texture to sprite
	float bgScale = 3.6;
	backgroundSprite.setTexture(background);
	backgroundSprite.setScale(bgScale, bgScale);
	parallaxOneSprite.setTexture(parallaxOne);
	parallaxOneSprite.setScale(bgScale, bgScale);
	parallaxTwoSprite.setTexture(parallaxTwo);
	parallaxTwoSprite.setScale(bgScale, bgScale);
	starsSprite.setTexture(stars);
	starsSprite.setScale(2, 2);
	starsSprite.setColor(sf::Color::Yellow);
	sf::FloatRect starsBounds = starsSprite.getLocalBounds();
	starsSprite.setOrigin(starsBounds.width / 2, starsBounds.height / 2);
	starsSprite.setPosition(viewSize.x * 0.5f, viewSize.y - 40);
	//set text variables
	headingFont.loadFromFile("Assets/fonts/SnackerComic.ttf");
	scoreFont.loadFromFile("Assets/fonts/arial.ttf");
	headingText.setFont(headingFont);
	headingText.setString("Catch the Stars");
	headingText.setCharacterSize(84);
	headingText.setFillColor(sf::Color::Yellow);
	sf::FloatRect headingbounds = headingText.getLocalBounds();
	headingText.setOrigin(headingbounds.width / 2, headingbounds.height / 2);
	headingText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.10f));
	// Tutorial Text
	tutorialText.setFont(scoreFont);
	tutorialText.setString("Press Spacebar switch Color and Start Game\n            Arrows to move, Shift to Sprint");
	tutorialText.setCharacterSize(35);
	tutorialText.setFillColor(sf::Color::Yellow);
	sf::FloatRect tutorialbounds = tutorialText.getLocalBounds();
	tutorialText.setOrigin(tutorialbounds.width / 2, tutorialbounds.height / 2);
	tutorialText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.26f));
	//Lives text

	//score text variables
	scoreText.setFont(scoreFont);
	scoreText.setString("Score: 0");
	scoreText.setCharacterSize(45);
	scoreText.setFillColor(sf::Color::Yellow);
	sf::FloatRect scorebounds = scoreText.getLocalBounds();
	scoreText.setOrigin(scorebounds.width / 2, scorebounds.height / 2);
	scoreText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.10f));
	//lives text variables
	livesText.setFont(scoreFont);
	livesText.setString("Lives: " + std::to_string(lives));
	livesText.setCharacterSize(35);
	livesText.setFillColor(sf::Color::Red);
	sf::FloatRect livesBounds = livesText.getLocalBounds();
	livesText.setOrigin(livesBounds.width / 2, livesBounds.height / 2);
	livesText.setPosition(sf::Vector2f(150, viewSize.y * 0.10f));
	//load hero
	hero.init("Assets/graphics/idle.png", "Assets/graphics/run.png", 4, 6, 1.0f, sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.8f), 200);
	srand((int)time(0));
}

void reset() {
	score = 0;
	lives = 3;
	currentTime = 0.0f;
	prevTime = 0.0;
	scoreText.setString("Score: 0");
	livesText.setString("Lives: " + std::to_string(lives));
	for (Enemy *enemy : enemies) {
		delete(enemy);
	}
	for (Points *point : points) {
		delete(point);
	}
	enemies.clear();
	points.clear();
}

void spawnPoints() {
	int randLoc = rand() % 1000 + 24;
	sf::Vector2f enemyPos;
	float speed = 300 + rand() % 120;
	enemyPos = sf::Vector2f(randLoc, -5);
	Enemy* enemy = new Enemy();
	enemy->init("Assets/graphics/enemy.png", enemyPos, speed);
	enemies.push_back(enemy);
}

bool checkCollision(sf::Sprite sprite1, sf::Sprite sprite2) {
	sf::FloatRect shape1(sprite1.getPosition().x - 15, sprite1.getPosition().y - 15, 30, 30);
	sf::FloatRect shape2 = sprite2.getGlobalBounds();

	if (shape1.intersects(shape2)) {
		return true;
	}
	else {
		return false;
	}
}

void changeColor() {
	switch (currentColor) {
	case 0:
		starsSprite.setColor(sf::Color::Red);
		currentColor = 1;
		break;
	case 1:
		starsSprite.setColor(sf::Color::Cyan);
		currentColor = 2;
		break;
	case 2:
		starsSprite.setColor(sf::Color::Yellow);
		currentColor = 0;
		break;
	}
}

void updateInput(float dt) {
	sf::Event event;
	float speed;
	while (window.pollEvent(event)) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
			speed = hero.m_sprintValue;
		}
		else {
			speed = hero.m_speedValue;
		}
		//Up
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Up) {
				hero.jump(750.0f);
			}
			if (event.key.code == sf::Keyboard::Space) {
				if (gameover) {
					gameover = false;
					reset();
				}
				else {
					changeColor();
				}
			}
		}
		if (event.type == sf::Event::KeyPressed) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				hero.move(-speed);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				hero.move(speed);
			}
		}
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			hero.move(0);
		}

		//Escape
		if (event.key.code == sf::Keyboard::Escape || event.type == sf::Event::Closed)
			window.close();
	}
}

void update(float dt) {
	hero.update(dt);
	currentTime += dt;
	// Spawn
	if (currentTime >= prevTime + 3.0f) {
		spawnPoints();
		prevTime = currentTime;
	}
	// Update Enemies
	for (int i = 0; i < enemies.size(); i++) {
		Enemy *enemy = enemies[i];
		enemy->update(dt);
		if (enemy->getSprite().getPosition().y > viewSize.y - 5) {
			enemies.erase(enemies.begin() + i);
			delete(enemy);
			lives -= 1;
			livesText.setString("Lives: " + std::to_string(lives));
			if (lives == 0)
				gameover = true;
		}
	}

	for (int j = 0; j < enemies.size(); j++) {
		Enemy* enemy = enemies[j];
		if (checkCollision(hero.getSprite(), enemy->getSprite())) {
			//hitSound.play();
			if (enemy->getSprite().getColor() == starsSprite.getColor()) {
				score++;
				std::string finalScore = "Score: " + std::to_string(score);
				scoreText.setString(finalScore);
				sf::FloatRect scorebounds = scoreText.getLocalBounds();
				scoreText.setOrigin(scorebounds.width / 2, scorebounds.height / 2);
				scoreText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.10f));
				enemies.erase(enemies.begin() + j);
				delete(enemy);
				printf("star collected \n");
			}
			else {
				enemies.erase(enemies.begin() + j);
				delete(enemy);
				lives -= 1;
				livesText.setString("Lives: " + std::to_string(lives));
				if (lives == 0)
					gameover = true;
			}
		}
	}
}

void draw() {
	window.draw(backgroundSprite);
	window.draw(parallaxOneSprite);
	window.draw(parallaxTwoSprite);
	window.draw(starsSprite);
	window.draw(hero.getSprite());
	if (gameover) {
		window.draw(headingText);
		window.draw(tutorialText);
	}
	else {
		window.draw(scoreText);
		window.draw(livesText);
	}
	for (Enemy *enemy : enemies) {
		window.draw(enemy->getSprite());
	}
}

int main() {
	sf::Clock clock;
	//init game objects
	init();

	while (window.isOpen()) {
		//handle events		
		//update objects in scene
		sf::Time dt = clock.restart();
		updateInput(dt.asSeconds());
		if (!gameover) {
			update(dt.asSeconds());
		}
		window.clear(sf::Color::Black);
		//render game objects
		draw();
		window.display();
	}
	return 0;
}
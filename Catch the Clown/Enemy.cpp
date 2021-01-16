#include "Enemy.h"
Enemy::Enemy() {}
Enemy::~Enemy() {}
void Enemy::init(std::string textureName, sf::Vector2f position, float speed) {
	m_speed = speed;
	m_position = position;
	// Load a Texture
	m_texture.loadFromFile(textureName.c_str());
	// Create Sprite and Attach a Texture
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(m_position);
	m_sprite.setOrigin(m_texture.getSize().x / 2, m_texture.getSize().y / 2);
	int randColor = rand() % 3;
	switch (randColor) {
	case 0:
		m_sprite.setColor(sf::Color::Yellow);
		break;
	case 1:
		m_sprite.setColor(sf::Color::Red);
		break;
	case 2:
		m_sprite.setColor(sf::Color::Cyan);
		break;
	}
}

void Enemy::update(float dt) {
	m_sprite.move(0, m_speed * dt);
}

sf::Sprite Enemy::getSprite() {
	return m_sprite;
}
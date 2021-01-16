#include "Hero.h"
Hero::Hero() {

}
Hero::~Hero() {

}

void Hero::init(std::string textureName, std::string runTextureName, int frameCount, int runFrameCount, float animDuration, sf::Vector2f position, float mass) {
	m_position = position;
	m_mass = mass;
	m_grounded = false;
	m_speed = 0;
	m_frameCount = frameCount;
	m_runFrameCount = runFrameCount;
	m_animDuration = animDuration;
	//load texture
	m_texture.loadFromFile(textureName.c_str());
	m_runTexture.loadFromFile(runTextureName.c_str());
	m_spriteSize = sf::Vector2i(137, 44);
	//create sprite and attach a texture
	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_spriteSize.x, m_spriteSize.y));
	m_sprite.setScale(3.5, 3.5);
	m_sprite.setPosition(m_position);
	m_sprite.setOrigin(16, 28);
}

void Hero::update(float dt) {
	//Animate Sprite
	m_elapsedTime += dt;
	int animFrame;
	if (m_speed == 0) {		
		m_sprite.setTexture(m_texture);
		animFrame = static_cast<int> ((m_elapsedTime / m_animDuration) * m_frameCount) % m_frameCount;
	}
	else {
		m_sprite.setTexture(m_runTexture);
		animFrame = static_cast<int> ((m_elapsedTime / m_animDuration) * m_runFrameCount) % m_runFrameCount;
	}
	if (m_speed<0)
		m_sprite.setScale(-3.5, 3.5);
	else
		m_sprite.setScale(3.5, 3.5);
	m_sprite.setTextureRect(sf::IntRect(0, animFrame * m_spriteSize.y, m_spriteSize.x, m_spriteSize.y));
	
	//Update position
	m_velocity -= m_mass * m_gravity * dt;
	m_position.y -= m_velocity * dt;
	m_position.x += m_speed * dt;
	m_sprite.setPosition(m_position);
	if (m_position.y >= 670) {
		m_position.y = 670;
		m_velocity = 0;
		m_grounded = true;
		jumpCount = 0;
	}
	if (m_position.x >= 1010) {
		m_position.x = 1010;
	}
	if (m_position.x <= 14) {
		m_position.x = 14;
	}
}

void Hero::jump(float velocity) {
	if (jumpCount < 2) {
		jumpCount++;
		m_velocity = velocity;
		m_grounded = false;
	}
}

void Hero::move(float speed) {
	m_speed = speed;
}

sf::Sprite Hero::getSprite() {
	return m_sprite;
}

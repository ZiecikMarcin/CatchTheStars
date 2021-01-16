#pragma once
#include "SFML-2.5.1\include\SFML\Graphics.hpp"
class Hero
{
public:
	Hero();
	~Hero();
	void init(std::string textureName, std::string runTextureName, int frameCount, int runFrameCount, float animDuration, sf::Vector2f position, float mass);
	void update(float dt);
	void jump(float velocity);
	void move(float speed);
	const float m_speedValue = 300;
	const float m_sprintValue = m_speedValue * 3;
	sf::Sprite getSprite();

private:
	sf::Texture m_texture;
	sf::Texture m_runTexture;
	sf::Sprite m_sprite;
	sf::Vector2f m_position;
	int jumpCount = 0;
	float m_mass;
	float m_velocity;
	float m_speed;
	const float m_gravity = 9.80f;	
	bool m_grounded;
	int m_frameCount;
	int m_runFrameCount;
	float m_animDuration;
	float m_elapsedTime;
	sf::Vector2i m_spriteSize;
};


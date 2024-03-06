#include "player.h"
#include <iostream>
#include "util.h"
#include <cmath>
using namespace std;

void Player::movePlayer(float dt)
{
	float delta_time = dt / 1000.0f;

	float move = 0.0f;
	if (graphics::getKeyState(graphics::SCANCODE_A))
	{
		move -= 1.0f;
		left = true;
	}
	if (graphics::getKeyState(graphics::SCANCODE_D))
	{
		move += 1.0f;
		left = false;
	}
	if (graphics::getKeyState(graphics::SCANCODE_W)) {
		graphics::playSound(m_state->getFullAssetPath("jump.wav"), 1.0f);
	}

	m_vx = std::min(m_max_velocity, m_vx + delta_time * move * m_accel_horizontal);
	m_vx = std::max(-m_max_velocity, m_vx);
	m_vx -= 0.2f * m_vx / (0.1f + fabs(m_vx));
	if (fabs(m_vx) < 0.01f)
		m_vx = 0.0f;
	m_pos_x += delta_time * m_vx;


	if (m_vy == 0.0f)
		m_vy -= graphics::getKeyState(graphics::SCANCODE_W) ? m_accel_vertical : 0.0f;
	m_vy += delta_time * m_gravity;
	m_pos_y += m_vy * delta_time;


}

void Player::resetPosition() {// Reset player's position to a default starting position
	
	
	m_pos_x = 3.9f;
	m_pos_y = 3.9f;
	m_width /= 4.0f;
	m_height /= 4.0f;

	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

	// Reset any other necessary player state (velocity, score, etc.)
	m_vx = 0.0f;
	m_vy = 0.0f;
	m_score = 0;
	m_health = 50;
}
void Player::update(float dt)
{
	movePlayer(dt);

	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;


	GameObject::update(dt);
}

void Player::init()
{
	if (isActive()){
		setDebugMode(true);}

	m_pos_x = 3.9f;
	m_pos_y = 3.9f;
	m_width /= 4.0f;
	m_height /= 4.0f;

	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

	m_brush_player.fill_opacity = 1.0f;
	m_brush_player.outline_opacity = 0.0f;
	m_brush_player.texture = m_state->getFullAssetPath("intro.png");

	
	for (unsigned int i = 1; i <= 12; ++i)
	{
		std::string filename = "r" + std::to_string(i) + ".png";
		//std::cout << "Filename: " << filename << std::endl;
		m_sprites.push_back(m_state->getFullAssetPath(filename));
	}

	for (unsigned int i = 11; i <= 18; ++i)
	{
		std::string filename = "idle" + std::to_string(i) + ".png";
		//std::cout << "Filename: " << filename << std::endl;
		m_sprites_standing.push_back(m_state->getFullAssetPath(filename));
	}

	for (unsigned int i = 1; i <= 12; ++i)
	{
		std::string filename = "j" + std::to_string(i) + ".png";
		//std::cout << "Filename: " << filename << std::endl;
		m_sprites_jump.push_back(m_state->getFullAssetPath(filename));
	}
	}
	



void Player::draw()
{
	int s = fmod(1000.f - m_pos_x * 3.0f, m_sprites.size());
	m_brush_player.texture = m_sprites[s];

	if (left)
	{
		graphics::setScale(-1.0f, 1.0f);
	}

	if (m_vx == 0 && m_vy == 0)
	{
		int k = fmod(graphics::getGlobalTime() / 200.0f, m_sprites_standing.size());
		m_brush_player.texture = m_sprites_standing[k];
	}


	if (m_vy != 0)
	{
		
		int y = fmod(1000.f - m_pos_y * 5.0f, m_sprites_jump.size());
		m_brush_player.texture = m_sprites_jump[y];
	}

	graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, 2.0f, 2.0f, m_brush_player);

	if (m_state->m_debugging)
	{
		debugDraw();
	}
}

void Player::debugDraw()
{
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 1, 0.3f, 0);
	SETCOLOR(debug_brush.outline_color, 1, 0.1f, 0);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;
	graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, m_width /= 4.0f, m_height /= 4.0f, debug_brush);


	char s[20];
	sprintf_s(s, "(%5.2f, %5.2f)", m_pos_x, m_pos_y);
	SETCOLOR(debug_brush.fill_color, 1, 1, 1);
	debug_brush.fill_opacity = 1.0f;
	graphics::drawText(m_state->getCanvasWidth() * 0.5f - 0.4f, m_state->getCanvasHeight() * 0.5f - 0.6f, 0.15f, s, debug_brush);

}
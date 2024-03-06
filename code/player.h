#pragma once
#include "gameobject.h"
#include <sgg/graphics.h>
#include "box.h"

#if (STAGE > 1)
#include <array>
#endif

#if (STAGE >= 4)
#include "sprite.h"
#endif

class Player : public Box, public GameObject
{
    graphics::Brush m_brush_player;
    std::vector<std::string> m_sprites;
    std::vector<std::string> m_sprites_standing;
    std::vector<std::string> m_sprites_duck;
    std::vector<std::string> m_sprites_jump;
    std::vector<Player> m_player;
    bool left;
    bool jump;
    bool standing;
    bool debug_mode = false;
    const float m_gravity = 10.0f;
    const float m_accel_vertical = 8.0f;
    const float m_accel_horizontal = 30.0f;
    const float m_max_velocity = 10.0f;
    int m_health;
    int m_score = 0;

    void movePlayer(float dt);
public:

    float m_vx = 0.0f;
    float m_vy = 0.0f;
    void setDebugMode(bool d) { debug_mode = d; }
    bool getDebugMode() { return debug_mode; }
    void resetPosition();
    void update(float dt) override;
    void draw() override;
    void init() override;
    Player(std::string name) : GameObject(name), m_health(50) {}
    int getHealth() { return m_health; }
    int getScore() { return m_score; }
    int setScore(int score) { return this->m_score += score; }
    int setHealth(int health) { return this->m_health += health; }
protected:
    void debugDraw();

};
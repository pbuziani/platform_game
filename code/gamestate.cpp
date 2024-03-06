#include "gamestate.h"
#include "level.h"
#include "player.h"
#include <thread>
#include <chrono>
#include "util.h"


GameState::GameState()
{
}
//starting screen
void GameState::StartScreen() 
{
	graphics::Brush m_brush_start;
	m_brush_start.outline_opacity = 0.0f;
	m_brush_start.texture = m_unique_instance->getFullAssetPath("start.png"); //eikona me kanones kai space


	graphics::drawRect(m_canvas_width / 2, m_canvas_height / 2, m_canvas_width, m_canvas_height, m_brush_start);

	graphics::setFont(getFullAssetPath("OpenSans-Bold.ttf"));
	graphics::Brush br;
	br.outline_opacity = 0.0f;
	SETCOLOR(br.fill_color, 1.0f, 1.0f, 1.0f);
	graphics::drawText(m_canvas_width / 2 - 4.0f, m_canvas_height / 2 - 1.8f, 0.5f, "Press SPACE to start", br);
	graphics::drawText(m_canvas_width / 2 - 7.0f, m_canvas_height / 2 + 1.8f, 0.5f, "Collect as  many points as you can by reaching the stars!", br);
	graphics::drawText(m_canvas_width / 2 - 4.0f, m_canvas_height / 2 + 3.6f, 0.5f, "Find the portal to win the game!", br);
	graphics::drawText(m_canvas_width / 2 - 5.0f, m_canvas_height / 2 + 4.8f, 0.5f, "Be careful not to drink the poison! Don't fall!", br);

	if (graphics::getKeyState(graphics::SCANCODE_SPACE))
	{
		// Transition to the game state 
		m_gameStatus = GameStatus::GAME;
		graphics::playSound(m_unique_instance->getFullAssetPath("transition.wav"), 1.0f);
		m_current_level->init();

	}

}
//screen if the player loses
void GameState::EndLostScreen()
{
    graphics::Brush m_brush_end;
    m_brush_end.outline_opacity = 0.0f;
    m_brush_end.texture = m_unique_instance->getFullAssetPath("end.png"); // End screen image

    graphics::drawRect(m_canvas_width / 2, m_canvas_height / 2, m_canvas_width, m_canvas_height, m_brush_end);

    graphics::setFont(getFullAssetPath("OpenSans-Bold.ttf"));
    graphics::Brush br;
    br.outline_opacity = 0.0f;
    SETCOLOR(br.fill_color, 1.0f, 1.0f, 1.0f);

    if (m_player->getHealth() <= 0)
    {
        graphics::playSound(m_unique_instance->getFullAssetPath("end bad.wav"), 1.0f);

        // You lose scenario
        graphics::drawText(m_canvas_width / 2 - 5.0f, m_canvas_height / 2 - 1.8f, 0.5f, "Game  Over!", br);
        graphics::drawText(m_canvas_width / 2 - 5.0f, m_canvas_height / 2 - 1.0f, 0.5f, "You  lost  all  your  health  points!", br);
    }
     
    graphics::drawText(m_canvas_width / 2 - 5.0f, m_canvas_height / 2 + 0.5f, 0.5f, "Click  to  Restart", m_brush_end);

    // Check for mouse click to restart the game
    graphics::MouseState mouse;
    graphics::getMouseState(mouse);

    if (mouse.button_left_pressed)
    {
        RestartGame();
    }
}

void GameState::EndWinScreen()
{
    graphics::Brush m_brush_end;
    m_brush_end.outline_opacity = 0.0f;
    m_brush_end.texture = m_unique_instance->getFullAssetPath("start.png"); // End screen image

    graphics::playSound(m_unique_instance->getFullAssetPath("ending.wav"), 1.0f);

    // You Win scenario
    graphics::drawRect(m_canvas_width / 2, m_canvas_height / 2, m_canvas_width, m_canvas_height, m_brush_end);
    graphics::setFont(getFullAssetPath("OpenSans-Bold.ttf"));
    graphics::drawText(m_canvas_width / 2 - 5.0f, m_canvas_height / 2 - 1.8f, 0.5f, "You Win!", m_brush_end);
    graphics::drawText(m_canvas_width / 2 - 5.0f, m_canvas_height / 2 - 1.0f, 0.5f, "Final Score: " + std::to_string(m_player->getScore()), m_brush_end);

    graphics::drawText(m_canvas_width / 2 - 5.0f, m_canvas_height / 2 + 0.5f, 0.5f, "Click to Restart", m_brush_end);

    // Check for mouse click to restart the game
    graphics::MouseState mouse;
    graphics::getMouseState(mouse);

    if (mouse.button_left_pressed)
    {
        RestartGame();
    }
}

void GameState::RestartGame()
{
    m_unique_instance->getPlayer()->resetPosition(); // reset position of player

    for (auto& box : m_current_level->getBlocks())
    {
        const_cast<Box&>(box).bsetActive(true); // setAlive the objects
    }

    m_gameStatus = GameStatus::GAME;
    graphics::playSound(m_unique_instance->getFullAssetPath("transition.wav"), 1.0f);
    m_current_level->init();
}

void GameState::draw()
{
    if (m_gameStatus == GameStatus::START)
    {
        StartScreen();
    }
    else if (m_gameStatus == GameStatus::GAME)
    {
        // Transition to the game state or level
        if (!m_current_level)
        {
            m_gameStatus = GameStatus::END_LOSE; // Go back to the start screen if there's no level
            return;
        }

        m_current_level->draw();

        if (m_player->getHealth() <= 0)
        {
            m_gameStatus = GameStatus::END_LOSE;
        }
    }
    else if (m_gameStatus == GameStatus::END_LOSE)
    {
        EndLostScreen();
    }
    else if (m_gameStatus == GameStatus::END_WIN)
    {
        EndWinScreen();
    }
}




void GameState::update(float dt)
{
	// Skip an update if a long delay is detected 
   // to avoid messing up the collision simulation
	if (dt > 500) // ms
		return;

	float sleep_time = std::max(0.0f, 17.0f - dt);

	std::this_thread::sleep_for(std::chrono::duration<float, std::milli>());


	if (m_gameStatus == GameStatus::GAME)
	{
		if (!m_current_level)
			return;

		m_current_level->update(dt);

		m_debugging = graphics::getKeyState(graphics::SCANCODE_0); // gia ekswterikh plhroforia aneksartito toy epipedoy
	}
}

GameState* GameState::getInstance()
{
	if (m_unique_instance == nullptr)
	{
		m_unique_instance = new GameState();
	}
	return m_unique_instance;
}
bool GameState::init()
{
	m_current_level = new Level("1.lvl");
	m_current_level->init();

	m_player = new Player("Player");
	m_player->init();

	graphics::preloadBitmaps(getAssetDir());
	graphics::setFont(m_asset_path + "OpenSans-Regular.ttf");

	return true;
}

GameState::~GameState()
{
	if (m_player)
		delete m_player;
	if (m_current_level)
		delete m_current_level;
}

std::string GameState::getAssetDir()
{
	return m_asset_path;
}

std::string GameState::getFullAssetPath(const std::string& asset)
{
	return m_asset_path + asset;
}


GameState* GameState::m_unique_instance = nullptr;
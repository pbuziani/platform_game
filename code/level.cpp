#include "level.h"
#include <sgg/graphics.h>
#include "player.h"
#include "util.h"
#include <iostream>
#include <fstream>     
#include <iostream>    
#include <string>      
#include <vector>      

void Level::drawBlock(int i, float sizew, float sizeh) // it draws/creates a block of type box
{
	graphics::resetPose(); 

	Box& box = m_blocks[i];
	float x = box.m_pos_x + m_state->m_global_offset_x;
	float y = box.m_pos_y + m_state->m_global_offset_y;
	m_block_brush.texture = m_state->getFullAssetPath(m_block_names[i]);

	if (box.bisActive()) {
		graphics::drawRect(x, y, sizew * m_block_size, sizeh * m_block_size, m_block_brush);
	}

	if (m_state->m_debugging) {
		graphics::drawRect(x, y, sizew * m_block_size, sizeh * m_block_size, m_block_brush_debug);

	}//size of box

}


void Level::checkCollisions() // it checks collisions for platform boxes (t 0) 
{
	// Check collisions with blocks
	for (auto& box : m_blocks)
	{
		if (box.m_id == 0){
			float offset = 0.0f;
			if (offset = m_state->getPlayer()->intersectDown(box))
			{
				m_state->getPlayer()->m_pos_y += offset;
				m_state->getPlayer()->m_vy = 0.0f;
				break;
			}
		}
	}
	for (auto& box : m_blocks)
	{
		if (box.m_id == 0) {
			float offset = 0.0f;
			if (offset = m_state->getPlayer()->intersectSideways(box))
			{
				m_state->getPlayer()->m_pos_x += offset;
				m_state->getPlayer()->m_vy = 0.0f;
				break;
			}
		}
	}



}

void Level::checkItemCollisions(std::vector<Box>& items, int scoreModifier,int healthModifier, int id )
{
	for (int i = 0; i < m_blocks.size(); i++)
	{
		if (m_blocks[i].m_id == id)
		{
			if (items[i].bisActive() && m_state->getPlayer()->intersect(items[i]))
			{
				if (id == 3) // Special box with m_id == 3
				{
					m_state->getPlayer()->setScore(scoreModifier);
					m_state->m_gameStatus = GameState::GameStatus::END_WIN; // Set the game status to indicate a win

				}
				else
				{ 
					
					m_state->getPlayer()->setHealth(healthModifier);
					m_state->getPlayer()->setScore(scoreModifier) ;
					items[i].bsetActive(false);
					if (id == 1) {
						graphics::playSound(m_state->getFullAssetPath("bonus.wav"), 1.0f);
					}
					else if (id == 2) {
						graphics::playSound(m_state->getFullAssetPath("hit.wav"), 1.0f);
					}
				}

				
				std::cout << "score: " << m_state->getPlayer()->getScore() << std::endl;
			}
		}
	}
}



void Level::update(float dt)
{
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->update(dt);

	// Check collisions with good items
	checkItemCollisions(m_blocks, 10, 0, 1);

	//Check collisions with bad items
	checkItemCollisions(m_blocks, 0, -10, 2);

	checkItemCollisions(m_blocks, 10, 0, 3);

	checkCollisions();

	GameObject::update(dt);
}




void Level::draw()
{
	float w = m_state->getCanvasWidth();
	float h = m_state->getCanvasHeight();

	float offset_x = m_state->m_global_offset_x / 2.0f + w / 2.0f;
	float offset_y = m_state->m_global_offset_y / 2.0f + h / 2.0f;



	//draw background
	graphics::drawRect(offset_x, offset_y, w * 20.0f, h * 2, m_brush_background); //w and h kosmou



	//player
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->draw();

	if (m_state->getPlayer()->m_pos_y > 5 * m_state->getCanvasHeight())
	{
		
		m_state->getPlayer()->resetPosition();  // if the player falls it resets its position but the he loses all his points.
	}

	//items
	for (int i = 0; i < m_blocks.size(); i++) {
		if (m_blocks[i].m_id == 3)
		{
			drawBlock(i, 4.0f, 4.0f); // thelw to portal na einai 4x4
		}
		else {
			drawBlock(i, 1.0f, 1.0f); //everything else same size of 1x1
		}
	}

	//Drawing score and health
	graphics::setFont(m_state->getFullAssetPath("font.otf"));

	text_brush.outline_opacity = 1.0f;
	SETCOLOR(text_brush.fill_color, 0.7f, 0.0f, 1.0f);

	std::string scoreText = "Score: " + std::to_string(m_state->getPlayer()->getScore());
	std::string healthText = "Health: " + std::to_string(m_state->getPlayer()->getHealth());


	graphics::drawText(1.0f, 1.0f, 1.0f, scoreText, text_brush);
	graphics::drawText(1.0f, 2.0f, 1.0f, healthText, text_brush);
}
void Level::init()
{
	// Clear existing data
	m_blocks.clear();
	m_block_names.clear();

	// Load level from file

	std::ifstream fp(m_state->getFullAssetPath("map2.txt"));
	if (!fp.is_open())
	{
		std::cerr << "Error opening file." << std::endl;
		return;
	}

	// Read the level data into a 2D vector
	std::vector<std::vector<char>> levelData;
	std::string line;
	while (std::getline(fp, line))
	{
		levelData.push_back(std::vector<char>(line.begin(), line.end()));
	}

	fp.close();

	// Iterate through the 2D vector and create blocks based on the characters
	for (int j = 0; j < levelData.size(); ++j)
	{
		for (int i = 0; i < levelData[j].size(); ++i)
		{
			char ch = levelData[j][i];
			if (ch == '#')
			{
				m_blocks.push_back(Box(i  , j  , 2 * m_block_size, 2 * m_block_size, 0));
				m_block_names.push_back("box2.png");
			}
			else if (ch == '$')
			{
				m_blocks.push_back(Box(i  , j  , 2 * m_block_size, 2 * m_block_size, 1));
				m_block_names.push_back("star.png");
			}
			else if (ch == '!')
			{
				m_blocks.push_back(Box(i  , j, 2 * m_block_size, 2 * m_block_size, 2));
				m_block_names.push_back("poison.png");
			}
			else if (ch == '%')
			{
				m_blocks.push_back(Box(i , j, 4 * m_block_size, 4 * m_block_size, 3));
				m_block_names.push_back("Ball.png");
			}
		}
	}

	// Set up brush properties
	m_block_brush.outline_opacity = 0.0f;
	m_block_brush_debug.fill_opacity = 0.1f;
	SETCOLOR(m_block_brush_debug.fill_color, 0.2f, 1.0f, 0.1f);
	SETCOLOR(m_block_brush_debug.outline_color, 0.3f, 1.0f, 0.2f);
}



Level::Level(const std::string& name)
	: GameObject(name)
{
	m_brush_background.outline_opacity = 0.0f;
	m_brush_background.texture = m_state->getFullAssetPath("darknew.png");
}

Level::~Level()
{
	for (auto p_go : m_static_objects)
		delete p_go;
	for (auto p_go : m_dynamic_objects)
		delete p_go;
}

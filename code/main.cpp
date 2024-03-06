#include <sgg/graphics.h>

#include "gamestate.h"
#include "level.h"




void draw() //H SEIRA SXEDIASHS EINAI SHMANTIKH
{
	GameState::getInstance()->draw();
}

void update(float dt)
{
	GameState::getInstance()->update(dt);

}



int main(int argc, char** argv)
{
	graphics::createWindow(800, 800 , "GAME"); 

	GameState::getInstance()->init();

	//Level* level = new Level("MyLevel");
	graphics::setDrawFunction(draw);
	graphics::setUpdateFunction(update);

	graphics::setCanvasSize(GameState::getInstance()-> getCanvasWidth(),GameState::getInstance()->getCanvasHeight());
	graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);
	
	graphics::startMessageLoop();

	//delete level;

	return 0;
}
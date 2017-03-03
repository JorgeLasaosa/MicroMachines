#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include <GL/glew.h>
#include <vector>
#include "GameObject.h"
#include "Player.h"
#include "SpriteRenderer.h"
#include "Wallblock.h"
#include "Iceblock.h"
#include "Diamondblock.h"

class GameLevel {
public:
    std::vector< std::vector<GameObject*> > field;
    std::vector< std::vector<GameObject*> > fieldStart;

    std::vector<Wallblock> wallN;      // Wall North
    std::vector<Wallblock> wallS;      // Wall South
    std::vector<Wallblock> wallE;      // Wall East
    std::vector<Wallblock> wallW;      // Wall West

    Player* pengo;

	GameLevel();

	// Loads level from file
	void load(const GLchar* filePath);

	// Render level
	void draw(SpriteRenderer& renderer);
	void drawGenerating(SpriteRenderer& renderer);

	bool checkCollision(glm::vec2 pos);

	virtual ~GameLevel();

private:
};

#endif // GAMELEVEL_H

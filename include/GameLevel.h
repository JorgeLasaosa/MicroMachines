#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include <GL/glew.h>
#include <vector>
#include "GameObject.h"
#include "Player.h"
#include "SpriteRenderer.h"

class GameLevel {
public:
    std::vector<GameObject> blocks;
    std::vector<GameObject> blocksStart;

    std::vector<GameObject> wallN;      // Wall North
    std::vector<GameObject> wallS;      // Wall South
    std::vector<GameObject> wallE;      // Wall East
    std::vector<GameObject> wallW;      // Wall West

    Player* pengo;

	GameLevel();

	// Loads level from file
	void load(const GLchar* filePath);

	// Render level
	void draw(SpriteRenderer& renderer);
	void drawGenerating(SpriteRenderer& renderer);

	virtual ~GameLevel();

private:
};

#endif // GAMELEVEL_H

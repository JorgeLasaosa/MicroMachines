#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include <GL/glew.h>
#include <vector>
#include <queue>
#include "GameObject.h"
#include "Player.h"
#include "SpriteRenderer.h"
#include "Wallblock.h"
#include "Iceblock.h"
#include "Diamondblock.h"

class Iceblock; // For compile
class GameLevel {
public:
    std::vector< std::vector<GameObject*> > field, fieldStart;
    std::queue< glm::vec2 > mazeNodesStart;
    std::vector< GameObject* > activeObjects;
    std::vector< Iceblock* > deadBlocks;

    std::vector<Wallblock> wallN;      // Wall North
    std::vector<Wallblock> wallS;      // Wall South
    std::vector<Wallblock> wallE;      // Wall East
    std::vector<Wallblock> wallW;      // Wall West

    Player* pengo;

	GameLevel();

	// Loads level from file
	void load(const GLchar* filePath);
	bool generate();

	// Render level
	void draw(SpriteRenderer& renderer);
	void drawGenerating(SpriteRenderer& renderer);

	bool checkCollision(glm::vec2 pos) const;
	GameObject* getObjFromPosition(glm::vec2 pos) const;
	void moveBlocks(GLfloat interpolation);
	void destroyBlocks(GLfloat interpolation);

	virtual ~GameLevel();

private:
	glm::vec2 genNodeActual;
};

#endif // GAMELEVEL_H

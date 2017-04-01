#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include <queue>
#include "GameObject.h"
#include "Player.h"
#include "SpriteRenderer.h"
#include "Wallblock.h"
#include "Iceblock.h"
#include "Diamondblock.h"
#include "Snobee.h"
#include "SnobeeEgg.h"
#include "FloatingText.h"


enum LevelState {
	LEVEL_START,
	LEVEL_SHOWING_EGGS,
	LEVEL_PLAY,
	LEVEL_KILL,
	LEVEL_LAST,
	LEVEL_WIN,
	LEVEL_LOSE,
	LEVEL_LOSE2,
	LEVEL_BONUS,
	LEVEL_TMP
};


class Iceblock; // For compile
class SnobeeEgg; // For compile
class GameLevel {
public:
	LevelState state;

    std::vector< std::vector<GameObject*> > field, fieldStart;
    std::vector< GameObject* > activeObjects;
    std::vector< Iceblock* > deadBlocks, eggBlocks;
    std::vector< Snobee* > enemies;
    std::vector< SnobeeEgg* > eggs;
    std::vector< FloatingText* > floatingTexts;
    std::queue< glm::vec2 > mazeNodesStart;

    std::vector<Wallblock> wallN;      // Wall North
    std::vector<Wallblock> wallS;      // Wall South
    std::vector<Wallblock> wallE;      // Wall East
    std::vector<Wallblock> wallW;      // Wall West

    Player* pengo;
    GLint deadEnemies, liveEnemies;
    GLint showEggsCount;

    Texture creaturesTexture;
    Texture eggsTexture;

    GLint bonusOffset;

	GameLevel();

	void update();

	// Loads level from file
	void load(const GLchar* filePath);
	bool generate();

	// Render level
	void draw(SpriteRenderer& renderer);
	void drawGenerating(SpriteRenderer& renderer);

	bool checkCollision(glm::vec2 pos) const;
	GameObject* getObjFromPosition(glm::vec2 pos) const;
	void moveBlocks(GLfloat interpolation);
	void moveEnemies(GLfloat interpolation);
	void destroyBlocks(GLfloat interpolation);
	void clearFromTop(SpriteRenderer& renderer, GLfloat to);
	void respawnPengo();
	glm::vec2 nearestAvailablePosition(GLint row, GLint col) const;
	void respawnEnemiesAtCorners();
	void clear();

	virtual ~GameLevel();

private:
	glm::vec2 genActualNode;
};

#endif // GAMELEVEL_H

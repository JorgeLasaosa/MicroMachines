#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include <queue>
#include <string>
#include "GameObject.h"
#include "Player.h"
#include "SpriteRenderer.h"
#include "Wallblock.h"
#include "Iceblock.h"
#include "Diamondblock.h"
#include "Snobee.h"
#include "SnobeeEgg.h"
#include "FloatingText.h"
#include "Camera.h"


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
class Snobee; // For compile
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
    Camera* camera;
    GLint deadEnemies, liveEnemies;
    GLint showEggsCount;
    GLint numEggs;

    Texture creaturesTexture;
    Texture eggsTexture;

    GLint bonusOffset;

	GameLevel(GLint numEggs, Camera* camera);

	void update();

	// Loads level from file
	void load(const std::string& filePath);
	bool generate();

	// Render level
	void draw(SpriteRenderer& renderer);
	void draw(Cube3DRenderer& cube3DRenderer);
	void drawGenerating(SpriteRenderer& renderer);
	void drawGenerating(Cube3DRenderer& cube3DRenderer);

	bool checkCollision(glm::vec2 pos) const;
	bool checkWalls(glm::vec2 pos) const;
	GameObject* getObjFromPosition(glm::vec2 pos) const;
	void moveBlocks(GLfloat interpolation);
	void moveEnemies(GLfloat interpolation);
	void destroyBlocks(GLfloat interpolation);
	void clearFromTop(SpriteRenderer& renderer, GLfloat to);
	void clearFromTop(Cube3DRenderer& renderer, GLfloat to);
	void respawnPengo();
	glm::vec2 nearestAvailablePosition(GLint row, GLint col) const;
	void respawnEnemiesAtCorners();
	void respawnBlocks();
	void clear();

	virtual ~GameLevel();

private:
	glm::vec2 genActualNode;
};

#endif // GAMELEVEL_H

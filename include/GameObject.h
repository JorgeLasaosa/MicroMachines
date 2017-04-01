#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "SpriteFrame.h"
#include "Game.h"

enum State {
    MOVING,
    STOPPED,
    PUSHING,
    DESTROYING,
    DYING,
    DEAD,
    DEAD2,
    NUMB,
};

enum Move {
    MOVE_UP,
    MOVE_LEFT,
    MOVE_DOWN,
    MOVE_RIGHT,
};

enum Shape {
    SHAPE_RECTANGLE,
    SHAPE_CIRCLE,
    SHAPE_CIRCLE_SMALL,
    SHAPE_DOT,
};

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject {
    public:

        // Object state
        glm::vec2 position, origPos, size, destination;
        GLboolean isPushable;
        GLfloat velocity;
        Move movement;
        State state, lastState;
        Shape shape;
        GLint killing;

        // Render state
        Texture sprite;
        SpriteFrame frame;
        GLfloat frameHandler;
        GLint frameIndex;

        // Constructor(s)
        GameObject();
        GameObject(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& sprite, GLboolean isPushable = false, Shape shape = SHAPE_RECTANGLE);
        virtual ~GameObject();

        // Gets
        glm::vec2 getPosition() {
            return position;
        }
        glm::vec2 getDestination() {
            return destination;
        }
        void setDestination(glm::vec2 dst) {
            destination = dst;
        }
        void setPosition(glm::vec2 pos) {
            position = pos;
        }
        void setFrameHandler(GLfloat fh) {
            frameHandler = fh;
        }
        GLfloat getFrameHandler() {
            return frameHandler;
        }
        SpriteFrame* getSpriteFrame() {
            return &frame;
        }
        GLint getFrameIndex() {
            return frameIndex;
        }
        void setFrameIndex(GLint fi) {
            frameIndex = fi;
        }
        Shape getShape() {
            return shape;
        }
        glm::vec2 getSize() {
            return size;
        }

        bool overlaps(GameObject* obj);
        // Actions
        bool move(GLfloat interpolation);

        // Draw sprite
        void configureFrame(GLint frame_width, GLint frame_height, glm::vec2 index);
        void changeIndexFrame(glm::vec2 index);
        virtual void draw(SpriteRenderer& renderer);
        virtual void draw(SpriteRenderer& renderer, GLfloat interpolation);
};

#endif // GAMEOBJECT_H

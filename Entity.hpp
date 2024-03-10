//
//  Entity.hpp
//  SDLProject
//
//  Created by Andy Ng on 3/5/24.
//  Copyright © 2024 ctg. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1

#ifdef _WINDOWS
#endif

#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

class Entity {
public:
    
   
    glm::vec3 mPosition;
    glm::vec3 mMovement;
    glm::vec3 mAcceleration;
    glm::vec3 mVelocity;
    glm::mat4 mModelMatrix;
    float mSpeed;
       
    GLuint textureID;
    
    float width = 1;
    float height = 1;
    
    int* right = NULL;
    int* left = NULL;
    int* up = NULL;
    int* down = NULL;

    int* indices = NULL;
    int frames = 0;
    int index = 0;
    float time = 0;
    int cols = 0;
    int rows = 0;
    
    bool hitsTop = false;
    bool hitsBottom = false;
    bool hitsLeft = false;
    bool hitsRight = false;
    
    Entity();
    ~Entity();
    
    void drawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);
    bool collisionCheck(Entity* collidableObj);
    void collisionXCheck(Entity* collidableObjects, int objectCount);
    void collisionYCheck(Entity* collidableObjects, int objectCount);
    void update(float deltaTime, Entity* platforms, int platformCount);
    void render(ShaderProgram* program);
};


#endif /* Entity_hpp */

//
//  Entity.cpp
//  SDLProject
//
//  Created by Andy Ng on 3/5/24.
//  Copyright © 2024 ctg. All rights reserved.
//

#include "Entity.hpp"

Entity::Entity()
{
    mPosition = glm::vec3(0);
    mMovement = glm::vec3(0);
    mAcceleration = glm::vec3(0);
    mVelocity = glm::vec3(0);
    mSpeed = 0;
    
    mModelMatrix = glm::mat4(1.0f);
}

Entity::~Entity(){};

void Entity::drawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index)
{
    float uCoord = (float)(index % cols) / (float)cols;
    float vCoord = (float)(index / cols) / (float)rows;
    
    float width = 1.0f / (float)cols;
    float height = 1.0f / (float)rows;
    
    float texCoords[] =
    { uCoord, vCoord + height, uCoord + width, vCoord + height, uCoord + width, vCoord,
        uCoord, vCoord + height, uCoord + width, vCoord, uCoord, vCoord
    };
    
    float vertices[]  =
    { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());
    
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}



bool Entity::collisionCheck(Entity* other) {
  
    float xDistance = fabs(mPosition.x - other->mPosition.x) - ((width + other->width) / 2.0f);
    float yDistance = fabs(mPosition.y - other->mPosition.y) - ((height + other->height) / 2.0f);
    
    if (xDistance < 0.0f && yDistance < 0.0f) return true;
    return false;
}


void Entity::collisionXCheck(Entity* collidableObjects, int objectCount)
{
   for (int i = 0; i < objectCount; i++)
   {
       Entity* collidableObj = &collidableObjects[i];
       if (collisionCheck(collidableObj))
       {
           float xDistance = fabs(mPosition.x - collidableObj->mPosition.x);
           float xOverlap = fabs(xDistance - (width / 2.0f) - (collidableObj->width / 2.0f));
           if (mVelocity.x > 0) {
               mPosition.x -= xOverlap;
               mVelocity.x = 0;
               hitsRight = true;
               collidableObj->hitsLeft = true;
           }else if (mVelocity.x < 0) {
               mPosition.x += xOverlap;
               mVelocity.x = 0;
               hitsLeft = true;
               collidableObj->hitsRight = true;
} }
} }

void Entity::collisionYCheck(Entity* collidableObjects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity * collidableObj = &collidableObjects[i];
        if (collisionCheck(collidableObj))
        {
            float yDistance = fabs(mPosition.y - collidableObj->mPosition.y);
            float yOverlap = fabs(yDistance - (height / 2.0f) - (collidableObj->height / 2.0f));
            if ( mVelocity.y > 0) {
                mPosition.y -= yOverlap;
                mVelocity.y = 0;
                hitsTop = true;
                collidableObj->hitsBottom = true;
            }else if (mVelocity.y < 0) {
                mPosition.y += yOverlap;
                mVelocity.y = 0;
                hitsBottom = true;
                collidableObj->hitsTop = true;
            }
        }
    }
}

void Entity::update(float deltaTime, Entity* platforms, int platformCount){
    
    hitsTop = false;
    hitsBottom = false;
    hitsLeft = false;
    hitsRight = false;
    
    if (indices != NULL) {
        if (glm::length(mMovement) != 0) { // if all 3 dimensions are not 0 length = sqrt(x^2 + y^2 + z^2)
            time += deltaTime;

            if (time >= 0.25f)
            {
                time = 0.0f;
                index++;
                if (index >= frames)
                {
                    index = 0;
                }
            }
        } else {
            index = 0;
        }
    }
    
    mVelocity.x = mMovement.x * mSpeed;
    mVelocity += mAcceleration * deltaTime;
    
    mPosition.y += mVelocity.y * deltaTime; // move x-position
    collisionYCheck(platforms, platformCount);
    
    mPosition.x += mVelocity.x * deltaTime; // move y-position
    collisionXCheck(platforms, platformCount);
    
    mModelMatrix = glm::mat4(1.0f);
    mModelMatrix = glm::translate(mModelMatrix, mPosition);
}

void Entity::render(ShaderProgram* program) {
   
    program->set_model_matrix(mModelMatrix);
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    
    if (indices != NULL) {
        drawSpriteFromTextureAtlas(program, textureID, indices[index]);
        return;
    }
    

    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());
    
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}




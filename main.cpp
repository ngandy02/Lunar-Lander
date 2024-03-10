/**
* Author: Andy Ng
* Assignment: Lunar Lander
* Date due: 2024-09-03, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.hpp"

#include <iostream>
#include <vector>
using namespace std;

#define PLATFORM_COUNT 31
struct GameEntities {
    Entity* player;
    Entity* platforms;
};

const int FONTBANK_SIZE =16;

GameEntities entity;

SDL_Window* displayWindow;
bool gameIsRunning = true;

float status = 0;


ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

GLuint fontTextureID;

GLuint loadTexture(const char* filePath) {
    // STEP 1: Loading the image file
    int width, height, n;
    unsigned char* image = stbi_load(filePath, &width, &height, &n, STBI_rgb_alpha);
    
    if (image == NULL) {
        cout << "Unable to load image. Make sure the path is correct";
        assert(false);
    }
    
    // STEP 2: Generating and binding a texture ID to our image
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    // STEP 3: Setting our texture filter parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // STEP 4: Releasing our file from memory and returning our texture id
    stbi_image_free(image);
    return textureID;
}

void initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Lunar Lander", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    // Load up our shaders
    program.load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    // Defines every translation, rotations, or scaling applied to an object
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
     
    program.set_projection_matrix(projectionMatrix);
    program.set_view_matrix(viewMatrix);
    
    glUseProgram(program.get_program_id());
    
    glClearColor(0.17f, 0.13f, 0.07f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
   
    // Initialize Game Objects
    
    // Initialize Player
    entity.player = new Entity();
    entity.player->mPosition = glm::vec3(0, 3.35, 0);
    entity.player->mMovement = glm::vec3(0);
    entity.player->mAcceleration = glm::vec3(0, -0.2f, 0);
    entity.player->mSpeed = 1.2f;
    entity.player->textureID = loadTexture("assets/charizard.png");
    
    entity.player->height = 0.8f;
    entity.player->width = 0.8f;
    
        
    // Initialize game platform
    entity.platforms = new Entity[PLATFORM_COUNT];
    GLuint platformTextureID =loadTexture("assets/platform.png");
    GLuint landingSiteTextureID = loadTexture("assets/landingplatform.png");
    fontTextureID = loadTexture("assets/characters.png");
    
    // Building game world
   
        float x = -5.5f;
        float y = -3.25f;
        for (int i = 0; i < 11; ++i) {
            if (i == 3 || i == 4 || i == 7 || i == 8){
                entity.platforms[i].textureID = landingSiteTextureID;
                entity.platforms[i].mPosition = glm::vec3(x, y, 0);
            }
            else{
                entity.platforms[i].textureID = platformTextureID;
                entity.platforms[i].mPosition = glm::vec3(x, y, 0);
            }
            x += 1;
        }
        x = -5.0f;
        y = -2.25f;
        for (int i = 11; i <18; ++i) {
            entity.platforms[i].textureID = platformTextureID;
            entity.platforms[i].mPosition = glm::vec3(x, y, 0);
            y += 1;
        }
        x = 5.0f;
        y = -2.25f;
        for (int i = 18; i < 25; ++i){
            entity.platforms[i].textureID = platformTextureID;
            entity.platforms[i].mPosition = glm::vec3(x, y, 0);
            y += 1;
        }
        x = 1;
        y = 1;
        for (int i = 25; i <27; ++i) {
            if (i == 26){
                entity.platforms[i].textureID = landingSiteTextureID;
                entity.platforms[i].mPosition = glm::vec3(x, y, 0);
            }
            else{
                entity.platforms[i].textureID = platformTextureID;
                entity.platforms[i].mPosition = glm::vec3(x, y, 0);
            }
            x += 1;
        }
        x = -3;
        y = 0;
        for(int i = 27; i < 29; ++i){
            entity.platforms[i].textureID = platformTextureID;
            entity.platforms[i].mPosition = glm::vec3(x, y, 0);
            y += 1;
        }
    
        x = -2;
        y = 0;
        for(int i = 29; i < 30; ++i){
            entity.platforms[i].textureID = platformTextureID;
            entity.platforms[i].mPosition = glm::vec3(x, y, 0);
            
        }
        
    
        x = 2;
        y = -1;
        for(int i = 30; i < PLATFORM_COUNT; ++i){
            entity.platforms[i].textureID = platformTextureID;
            entity.platforms[i].mPosition = glm::vec3(x, y, 0);
            x += 1;
        }
        
        for (int i = 0; i < PLATFORM_COUNT; i++) {
            entity.platforms[i].update(0, NULL, 0);
        }
     
    
 
}

void processInput() {
    
    entity.player->mMovement = glm::vec3(0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        break;
                        
                    case SDLK_RIGHT:
                        break;
                        
                   
                }
                break;
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        entity.player->mAcceleration.x += -1.0f;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        entity.player->mAcceleration.x += 1.0f;
    }
    
    if (glm::length(entity.player->mMovement) > 1.0f) {
        entity.player->mMovement = glm::normalize(entity.player->mMovement);
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void update() {
    if (entity.player->hitsLeft || entity.player->hitsRight || entity.player->hitsTop) {
        status = -1;
    }
    if (entity.player->hitsBottom == true) {
        if (entity.platforms[3].hitsTop == true || entity.platforms[4].hitsTop == true ||entity.platforms[7].hitsTop == true ||
            entity.platforms[8].hitsTop == true ||   entity.platforms[26].hitsTop == true) {
            status = 1;
        }
        else {
            status = -1;
        }
    }
    if (status == -1 || status == 1) {
        return;
    }
   float ticks = (float)SDL_GetTicks() / 1000.0f;
   float deltaTime = ticks - lastTicks;
   lastTicks = ticks;
   deltaTime += accumulator;
   if (deltaTime < FIXED_TIMESTEP) {
       accumulator = deltaTime;
       return;
   }
   while (deltaTime >= FIXED_TIMESTEP) {
       entity.player->update(FIXED_TIMESTEP, entity.platforms, PLATFORM_COUNT);
       deltaTime -= FIXED_TIMESTEP;
   }
   accumulator = deltaTime;
}


void drawText(ShaderProgram *program, GLuint fontTextureID, std::string text,
              float screeenSize, float spacing, glm::vec3 position) {
    float width = 1.0f / FONTBANK_SIZE;
    float height = 1.0f / FONTBANK_SIZE;
    std::vector<float> vertices;
    std::vector<float> texCoordinates;
    for(int i = 0; i < text.size(); i++) {
        int index = (int)text[i];
        float offset = (screeenSize + spacing) * i;
        
        float uCoord = (float)(index % FONTBANK_SIZE) / FONTBANK_SIZE;
        float vCoord = (float)(index / FONTBANK_SIZE) / FONTBANK_SIZE;
        vertices.insert(vertices.end(), {
            offset + (-0.5f * screeenSize), 0.5f * screeenSize,
            offset + (-0.5f * screeenSize), -0.5f * screeenSize,
            offset + (0.5f * screeenSize), 0.5f * screeenSize,
            offset + (0.5f * screeenSize), -0.5f * screeenSize,
            offset + (0.5f * screeenSize), 0.5f * screeenSize,
            offset + (-0.5f * screeenSize), -0.5f * screeenSize,
        });
        texCoordinates.insert(texCoordinates.end(), {
            uCoord, vCoord,
            uCoord, vCoord + height,
            uCoord + width, vCoord,
            uCoord + width, vCoord + height,
            uCoord + width, vCoord,
            uCoord, vCoord + height,
        });
    }
    
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    
    program->set_model_matrix(modelMatrix);
    glUseProgram(program->get_program_id());
    
    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->get_position_attribute());
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texCoordinates.data());
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());
    
    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));
    
    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}


void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        entity.platforms[i].render(&program);
    }
    entity.player->render(&program);
    if (status == -1) {
        drawText(&program, fontTextureID, "Mission Failed!", 1, -0.5, glm::vec3(-3.375f, 1, 0));
    }
    else if (status == 1) {
        drawText(&program, fontTextureID, "Mission Accomplish!", 1, -0.5, glm::vec3(-4.125f, 1, 0));
    }
    SDL_GL_SwapWindow(displayWindow);
}


void shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    initialize();
    
    while (gameIsRunning) {
        processInput();
        update();
        render();
    }
    
    shutdown();
    
    return 0;
}

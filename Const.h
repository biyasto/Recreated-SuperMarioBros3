#pragma once
#include <string>


#define TITLE L"Mario"
#define SCREEN_WIDTH 780
#define SCREEN_HEIGHT 780

#define TEXTURE_MARIO "tex-mario"

const std::string SPRITE_PATH = "Assets/Sprites/";
const std::string SPRITE_DATABASE = SPRITE_PATH + "/SpriteDatabases/";
const std::string ANIMATION_PATH = "Assets/Animations/";

// Sprites
const std::string MARIO_DB = "MarioDB.xml";

// Animations
const std::string MARIO_ANIM = "MarioAnim.xml";

// Game object tags
const std::string PLAYER_TAG = "Player";
const std::string CAMERA_TAG = "Camera";
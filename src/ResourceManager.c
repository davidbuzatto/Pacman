/**
 * @file ResourceManager.c
 * @author Prof. Dr. David Buzatto
 * @brief ResourceManager implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <stdio.h>
#include <stdlib.h>

#include "ResourceManager.h"
#include "raylib/raylib.h"

ResourceManager rm = { 0 };

void loadResourcesResourceManager( void ) {
    rm.icon = LoadImage( "resources/images/icon.png" );
    rm.spriteMap = LoadTexture( "resources/images/sprites.png" );
    /*rm.soundExample = LoadSound( "resources/sfx/powerUp.wav" );
    rm.musicExample = LoadMusicStream( "resources/musics/overworld1.ogg" );*/
}

void unloadResourcesResourceManager( void ) {
    UnloadImage( rm.icon );
    UnloadTexture( rm.spriteMap );
    /*UnloadSound( rm.soundExample );
    UnloadMusicStream( rm.musicExample );*/
}
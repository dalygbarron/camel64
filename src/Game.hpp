#pragma once

#include "Menu.hpp"
#include "Spatial.hpp"
#include "Skybox.hpp"

/**
 * Contains all the state of the game at the current time.
 */
class Game {
    private:
        static unsigned int const MAX_DUDES = 128;
        Skybox sky;
        Spatial camera;
        unsigned int nWalls;
        Wall *walls = NULL;
        Menu *menu = NULL;
};
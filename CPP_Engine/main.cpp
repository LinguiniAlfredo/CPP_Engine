#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

#include "Scene.h"
#include "Utils/Timer.h"
#include "Entities/Entity.h"
#include "Entities/Player.h"
#include "Entities/Surface.h"
#include "Entities/Box.h"
#include "Entities/Pickup.h"
#include "Entities/Switch.h"
#include "Entities/Lift.h"
#include "Components/Collision.h"

/*
	TODO:
		Engine:
			- ***Implement new inheritance to each entity
			- Stop using pointers so much
			- Tilemap implementation
			- Entity hashmap in scene with indexing
			- Port to OpenGL for custom shaders
			- Animation system
			- Editor
			- Refactor Scene to be entity manager and handle all memory allocations
*/

const int SCREEN_WIDTH = 1920 / 2;
const int SCREEN_HEIGHT = 1080 / 2;
const int INTERNAL_SCREEN_WIDTH = 320;
const int INTERNAL_SCREEN_HEIGHT = 180;
const int CENTER_X = INTERNAL_SCREEN_WIDTH / 2;
const int CENTER_Y = INTERNAL_SCREEN_HEIGHT / 2;
const int NUM_TILES_WIDE = INTERNAL_SCREEN_WIDTH / 8;
const int NUM_TILES_HIGH = INTERNAL_SCREEN_HEIGHT / 8;

bool debug = false;

enum scene {
	MAIN_MENU,
	LEVEL_1,
	LEVEL_2,
	LEVEL_3,
	GAME_OVER,
	TOTAL
};
Scene* currentScene;

enum entityType {
	PLAYER,
	PICKUP,
	SURFACE,
	BOX
};

bool init();

void initMainMenu();
void initLevelOne();
void initLevelTwo();

bool changeScene(int scene);

void close();


SDL_Window* window = nullptr;

SDL_Surface* screenSurface = nullptr;

SDL_Renderer* renderer = nullptr;


bool changeScene(int scene)
{
	if (currentScene != nullptr) {
		delete currentScene;
		currentScene = nullptr;
	}

	switch (scene) {
		case MAIN_MENU:
			initMainMenu();
			break;

		case LEVEL_1:
			initLevelOne();
			break;

		case LEVEL_2:
			initLevelTwo();
			break;

		case LEVEL_3:
			break;

		case GAME_OVER:
			break;

		default:
			break;
	}

	return true;
}

void initMainMenu()
{
	Scene* mainMenu = new Scene();
	////mainMenu->addGameObject(title);
	////mainMenu->addGameObject(menu);
	currentScene = mainMenu;
	
}

void initLevelOne()
{
	Scene* level = new Scene(renderer);

	level->addEntity(new Player(level, "resources/textures/guy.png", { 7, NUM_TILES_HIGH - 4 }));

	// add floor across bottom
	for (int i = 0; i < NUM_TILES_WIDE; i++) {
		level->addEntity(new Surface(level, "resources/textures/ground_tile.png", { i, NUM_TILES_HIGH - 1 }));
	}

	currentScene = level;
}

void initLevelTwo()
{
	Scene* level = new Scene(renderer);

	level->addEntity(new Player(level, "resources/textures/guy.png", { 7, NUM_TILES_HIGH - 4 }));

	// add floor across bottom
	for (int i = 0; i < NUM_TILES_WIDE; i++) {
		if (i > NUM_TILES_WIDE - 8 && i < NUM_TILES_WIDE - 2) {
			continue;
		}
		level->addEntity(new Surface(level, "resources/textures/ground_tile.png", { i, NUM_TILES_HIGH - 1 }));
	}

	// add steps up to platform
	for (int i = 14; i < 17; i++) {
		level->addEntity(new Surface(level, "resources/textures/ground_tile.png", { i, NUM_TILES_HIGH - 2 }));
		if (i != 14) {
			level->addEntity(new Surface(level, "resources/textures/ground_tile.png", { i, NUM_TILES_HIGH - 3 }));
		}
		if (i == 16) {
			level->addEntity(new Surface(level, "resources/textures/ground_tile.png", { i, NUM_TILES_HIGH - 4 }));
		}
	}

	level->addEntity(new Surface(level, "resources/textures/platform.png", { NUM_TILES_WIDE / 2, NUM_TILES_HIGH - 4 }));

	for (int i = 0; i < 14; i++) {
		level->addEntity(new Surface(level, "resources/textures/grass_1.png", { 18 + i, NUM_TILES_HIGH - 2 }, false));
	}

	level->addEntity(new Box(level, new Pickup(level, "flower", false), { 10, NUM_TILES_HIGH - 4 }));

	level->addEntity(new Surface(level, "resources/textures/sign.png", { NUM_TILES_WIDE - 2, NUM_TILES_HIGH - 2 }, false));


	// TODO - Find better API for switches/blocks
	// create array of blocks to attach to switch object
	std::vector<Surface*> blueSwitchBlocks;
	for (int i = 0; i < 5; i++) {
		blueSwitchBlocks.push_back(new Surface(level, "resources/textures/trans_block_blue.png", { NUM_TILES_WIDE - 7 + i, NUM_TILES_HIGH - 1 }, false));
	}
	level->addEntity(new Switch(level, blueSwitchBlocks, { NUM_TILES_WIDE / 2 + 11, NUM_TILES_HIGH - 10 }));

	// create array of blocks to attach to switch object
	std::vector<Surface*> redSwitchBlocks;
	for (int i = 0; i < 3; i++) {
		redSwitchBlocks.push_back(new Surface(level, "resources/textures/trans_block_red.png", { NUM_TILES_WIDE / 2 + 10 + i, NUM_TILES_HIGH - 7 }, false));
	}
	level->addEntity(new Switch(level, redSwitchBlocks, "red", { NUM_TILES_WIDE / 2 + 4, NUM_TILES_HIGH - 7 }));

	// create array of blocks to attach to switch object
	std::vector<Surface*> greenSwitchBlocks;
	for (int i = 0; i < 3; i++) {
		greenSwitchBlocks.push_back(new Surface(level, "resources/textures/block_green.png", { NUM_TILES_WIDE - 6 + i, 5 }, true));
	}
	level->addEntity(new Switch(level, greenSwitchBlocks, "green", { 2, 3 }, true));

	level->addEntity(new Pickup(level, "key", { NUM_TILES_WIDE - 5, 3 }, true));

	level->addEntity(new Lift(level, "resources/textures/lift.png", { 0, NUM_TILES_HIGH - 5 }));

	currentScene = level;
}

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else {
		window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == nullptr) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (renderer == nullptr) {
				printf("renderer could not be created : %s", SDL_GetError());

			}
			else {
				SDL_RenderSetLogicalSize(renderer, INTERNAL_SCREEN_WIDTH, INTERNAL_SCREEN_HEIGHT);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_Image could not init : %s", IMG_GetError());
					success = false;
				}
				else {
					screenSurface = SDL_GetWindowSurface(window);
				}
			}
		}
	}
	return success;
}

void close()
{		
	delete currentScene;
	currentScene = nullptr;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = nullptr;
	window = nullptr;

	IMG_Quit();
	SDL_Quit();
}

void toggleDebug()
{
	debug = !debug;
}

int main( int argc, char* args[] )
{
	if (!init()) {
		printf("failed to init");
	}
	else {
		if (!changeScene(LEVEL_2)) {
			printf("failed to load entities");
		}
		else {
			bool quit = false;

			SDL_Event e;

			Timer frameTimer;
			Timer deltaTimer;
			int countedFrames = 0;
			float deltaTime = 0;
			float totalTime = 0;
			float fps = 0;

			frameTimer.start();
			deltaTimer.start();


			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
					}

					if (e.type == SDL_KEYDOWN) {
						switch (e.key.keysym.sym) {
							case SDLK_0:
								changeScene(MAIN_MENU);
								break;
							case SDLK_1:
								changeScene(LEVEL_1);
								break;
							case SDLK_2:
								changeScene(LEVEL_2);
								break;
							case SDLK_BACKQUOTE:
								toggleDebug();
								break;
							case SDLK_ESCAPE:
								quit = true;
								break;
							default:
								break;
						}
					}
					for (Entity* ent: currentScene->getEntities()) {
						ent->handleEvent(e);
					}
				}

				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderClear(renderer);

				for (Entity* ent: currentScene->getEntities()) {
					ent->update(deltaTime);
					ent->draw();

					if (debug) {
						ent->getCollider()->render(ent->isColliding());
					}
				}
				currentScene->clearTrash();

				SDL_RenderPresent(renderer);

				totalTime = frameTimer.getTicks() / 1000.f;
				deltaTime = deltaTimer.getTicks() / 1000.f;
				fps = countedFrames / totalTime;
				printf("%.3g FPS\n", fps); // TODO - display frames on screen
				countedFrames++;
				deltaTimer.start();
			}
		}
	}
	close();

	return 0;
}

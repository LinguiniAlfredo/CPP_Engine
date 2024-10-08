#pragma once
#include <SDL.h>
#include <vector>
class Entity;
class Player;

class Scene
{
public:
	Scene();
	Scene(SDL_Renderer* renderer);
	Scene(SDL_Renderer* renderer, SDL_Rect* camera);
	Scene(Scene& src);
	~Scene();

	void addEntity(Entity* ent);
	void removeEntity(Entity* ent);
	void binIt(Entity* ent);
	void clearTrash();

	std::vector<Entity*> getEntities(); // TODO - Make a hash map of indexed entities instead
	int getTileSize();
	SDL_Renderer* getRenderer();
	Entity* getPlayer();
	SDL_Rect* getCamera();

private:
	std::vector<Entity*> entities;
	std::vector<Entity*> trashBin;
	const int TILESIZE = 8;
	SDL_Renderer* renderer;
	SDL_Rect* camera;
};


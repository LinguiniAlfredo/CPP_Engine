#pragma once
#include <SDL.h>
#include "Entity.h"
#include "vec2.h"
#include <string>
class Texture;

class Surface :
	public Entity
{
public:
	Surface(SDL_Renderer* renderer);
	Surface(SDL_Renderer* renderer, std::string textureFile, vec2 currentPosition);
	~Surface();

	void draw();
	void update(float deltaTime);

	void setScene(Scene* scene);

	bool isColliding();
	bool hasCollider();
	SDL_Rect* getCollider();

	vec2 getPosition();

private:
	Scene* scene;

	Texture* texture;
	vec2 currentPosition;
	const std::string textureFile = "resources/textures/wall.png";

	SDL_Rect* collider;
	bool colliding = false;

	void checkCollisions(float deltaTime);
};

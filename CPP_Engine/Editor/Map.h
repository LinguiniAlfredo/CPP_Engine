#pragma once
#include <SDL.h>
#include <string>
#include <vector>
class Vec2;

class Map
{
public:
	Map(std::string filename);
	~Map();

	void load();
    void writeSingleTile(Vec2 pos, int tileType);

    std::vector<int> getData();

private:
    std::vector<int> data;
    std::string filename;
    const int width = 40;
    const int height = 22;
};
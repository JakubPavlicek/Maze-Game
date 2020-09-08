#pragma once

class Maze
{
private:
	int end;
	int player;
	void resetMaze();
public:
	float height;
	float width;
	void movePlayer();
	int createMaze(float height, float width);
};
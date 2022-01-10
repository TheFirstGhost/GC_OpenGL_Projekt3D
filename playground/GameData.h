#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <memory>
//#include <glfw3.h>

// Include GLM
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "RenderingObject.h"
//#include "playground.h"

const int
	FORWARD = 1,
	BACKWARD = 2,
	LEFT = 3,
	RIGHT = 4,
	STRAIGHT = 5,
	UP = 6,
	DOWN = 7;

struct World
{
public:
	std::shared_ptr<RenderingObject> worldObject;

	std::vector<std::vector<int>> map;

	glm::vec3 cubePos;

	World();
	World(std::shared_ptr <RenderingObject> _worldObject, std::vector<std::vector<int>> _map, int cubePosX, int cubePosY, int cubePosZ);
	bool checkFallDown(void);
	bool checkWin(void);

private:

};

struct Cube
{
public:
	std::shared_ptr<RenderingObject> cubeObject;

	glm::vec3 pos;
	glm::vec3 rot;

	int rotPos;

	int lastDirection;
	int lastHight;
	int lastLevel;
	int currLevel;
	int rotPosOffset;

	Cube();
	Cube(std::shared_ptr <RenderingObject> _cubeObject, glm::vec3 _pos);
	Cube(std::shared_ptr <RenderingObject> _cubeObject, double x, double y, double z);
	void role(const int direction);
	void roleUp(const int direction);
	void goDown(const int direction);
	bool roleFinish( void );
	void fall();
	void reset();
	void setColor(GLuint programID);
	
private:
	int calculateRotPos( void );
	void setRotPosOffset(const int direction);

};

#endif
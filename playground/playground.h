#ifndef PLAYGROUND_H
#define PLAYGROUND_H

// Include GLEW
#include <GL/glew.h>

// Include GLM
#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <playground/parse_stl.h>

#include "RenderingObject.h"
#include "GameData.h"

//program ID of the shaders, required for handling the shaders with OpenGL
GLuint programID;

//global variables to handle the MVP matrix
GLuint View_Matrix_ID;
glm::mat4 V;
GLuint Projection_Matrix_ID;
glm::mat4 P;
GLuint Model_Matrix_ID;

RenderingObject ground;

RenderingObject cubeObj;
RenderingObject worldObj;

std::shared_ptr<World> testWorld;
std::shared_ptr<Cube> testCube;

World world;
Cube cube;

int map2[3][6] = {
	{1, 1, 1, 0, 0, 0},
	{1, 0, 0, 0, 1, 0},
	{1, 1, 1, 0, 0, 0},
};

/*std::vector<std::vector<int>> map = {
	{ 1, 1, 1, 0, 0, 0 },
	{ 1, 0, 0, 0, 1, 0 },
	{ 1, 1, 1, 0, 0, 0 }
};*/

std::vector<std::vector<int>> map = {
	{-1, 2, 2, 2 },
	{-1, 2, 1, 2 },
	{-1, 2, 1, 2 },
	{ 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1 },
	{ 1, 1, 2, 1, 1 },
	{ 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1 }
};

std::vector<std::vector<int>> map1 = {
	{ 4, 4, 1, 1, 1 },
	{ 4, 1, 1, 1, 1, 1, 1, 1 },
	{ 4, 1, 1, 1, 1, 1, 1, 1 },
	{ 4, 1, 1, 1, 1, 1, 1, 1 },
	{ 4, 1, 1, 1, 1, 1, 1, 1 },
	{ 4, 1, 1, 1, 1, 1, 1, 1 },
	{ 4, 4, 4, 4, 1, 1, 1, 1 },
	{ -1, 4, 4, 4, 1, 1, 1 },
	{ -1, 4, 4, 4, 1, 1, 1 },
	{ -1, 4, 4, 4, 1, 1, 1 },
	{ -1, 4, 4, 4, 1, 1, 1 },
	{ -1, 4, 4, 4, 1, 1, 1 },
	{ -1, 4, 4, 4, 1, 1, 1, -1, -1, 1, 1, 1 },
	{ 4, 4, 4, 1, 1, 1, 1, -1, -1, 1, -2, 1 },
	{ 4, 4, 4, 1, 1, 1, 1, -1, -1, 1, 1, 1 },
	{ 4, 4, 4, 1, 1, 1, 1, -1, -1, 2, 1, 2 },
	{ 4, 4, 4, 1, 1, 1, -1, -1, -1, 2, 1, 2 },
	{ 4, 4, 4, 1, 1, 1, -1, -1, -1, 1, 1, 1 },
	{ 4, 4, 4, 4, 1, 1, -1, -1, -1, 1, 1, 1 },
	{ -1, 4, 4, 4, 2, 2, -1, -1, -1, 1, 1, 1 },
	{ -1, 4, 4, 4, 2, 2, 2, 2, 1, 1, 1, 1, 1 },
	{ -1, 4, 4, 4, 2, 2, 2, 2, 2, 1, 1, 1, 1 },
	{ -1, 4, 4, 4, 2, 2, 2, 2, 2, 1, 1, 1, 1 },
	{ -1, -1, 4, 4, 2, 2, 2, 2, 2, 1, 1, 1, 1 },
	{ -1, -1, -1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3 }
};

float curr_x;
float curr_y;
float curr_z;
float curr_angle;

float cam_z;

int lastKey;
int lastLevel;
int cycle;

bool falling;


int main( void ); //<<< main function, called at startup
bool checkNextCubePos(const int dir, const int level);
bool checkGoDown();
bool checkIllegalValues(vec2 pos);
void updateAnimationLoop(); //<<< updates the animation loop
bool initializeWindow(); //<<< initializes the window using GLFW and GLEW
bool initializeMVPTransformation();
bool initializeVertexbuffer(); //<<< initializes the vertex buffer array and binds it OpenGL
bool cleanupVertexbuffer(); //<<< frees all recources from the vertex buffer
bool closeWindow(); //<<< Closes the OpenGL window and terminates GLFW
void updataMovingObjectTransformation();
void fall();


#endif

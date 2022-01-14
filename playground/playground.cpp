#include "playground.h"

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

// Include GLM
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>

int i = 0;

int main( void )
{
  //Initialize window
  bool windowInitialized = initializeWindow();
  if (!windowInitialized) return -1;

  //Initialize vertex buffer
  bool vertexbufferInitialized = initializeVertexbuffer();
  if (!vertexbufferInitialized) return -1;

  // Create and compile our GLSL program from the shaders
  programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

  initializeMVPTransformation();

  cam_z = 200;

  falling = false;

  gameTime = 0;

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);


  initializeMVPTransformation();

	//start animation loop until escape key is pressed
	do{

    updateAnimationLoop();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	
  //Cleanup and close window
  cleanupVertexbuffer();
  glDeleteProgram(programID);
	closeWindow();
  
	return 0;
}

bool checkNextCubePos(const int dir, const int level) //checks if ne next Position of the cube is posible to roll
{

	switch (dir)
	{
	case 1:
		if (checkIllegalValues(vec2(world.cubePos.x, world.cubePos.y - 1))) return true; //make sure that the next value is not out of range
		else if (level == STRAIGHT) {
			//
			if (world.cubePos.z >= world.map[world.cubePos.x][world.cubePos.y - 1] && world.cubePos.z == world.map[world.cubePos.x - 1][world.cubePos.y - 1]) return true; 
			else if (world.cubePos.z == world.map[world.cubePos.x][world.cubePos.y - 1]) return true;
			else return false;
		}
		else if (level == UP) {
			if (world.cubePos.z == world.map[world.cubePos.x][world.cubePos.y - 1] - 1) return true;
			else return false;
		}
		else if (level == DOWN) {
			if (world.cubePos.z == world.map[world.cubePos.x][world.cubePos.y - 1] + 1) return true;
			else return false;
		}
		break;
	case 2:
		if (checkIllegalValues(vec2(world.cubePos.x - 2, world.cubePos.y - 1))) return true;
		else if (level == STRAIGHT) {
			if (world.cubePos.z >= world.map[world.cubePos.x - 2][world.cubePos.y - 1] && world.cubePos.z == world.map[world.cubePos.x - 1][world.cubePos.y - 1]) return true;
			else if (world.cubePos.z == world.map[world.cubePos.x - 2][world.cubePos.y - 1]) return true;
			else return false;
		}
		else if (level == UP) {
			if (world.cubePos.z == world.map[world.cubePos.x - 2][world.cubePos.y - 1] - 1) return true;
			else return false;
		}
		else if (level == DOWN) {
			if (world.cubePos.z == world.map[world.cubePos.x - 2][world.cubePos.y - 1] + 1) return true;
			else return false;
		}
		break;
	case 3:
		if (checkIllegalValues(vec2(world.cubePos.x - 1, world.cubePos.y - 2))) return true;
		else if (level == STRAIGHT) {
			if (world.cubePos.z >= world.map[world.cubePos.x - 1][world.cubePos.y - 2] && world.cubePos.z == world.map[world.cubePos.x - 1][world.cubePos.y - 1]) return true;
			else if (world.cubePos.z == world.map[world.cubePos.x - 1][world.cubePos.y - 2]) return true;
			else return false;
		}
		else if (level == UP) {
			if (world.cubePos.z == world.map[world.cubePos.x - 1][world.cubePos.y - 2] - 1) return true;
			else return false;
		}
		else if (level == DOWN) {
			if (world.cubePos.z == world.map[world.cubePos.x - 1][world.cubePos.y - 2] + 1) return true;
			else return false;
		}
		break;
	case 4:
		if (checkIllegalValues(vec2(world.cubePos.x - 1, world.cubePos.y))) return true;
		else if (level == STRAIGHT) {
			if (world.cubePos.z >= world.map[world.cubePos.x - 1][world.cubePos.y] && world.cubePos.z == world.map[world.cubePos.x - 1][world.cubePos.y - 1]) return true;
			if (world.cubePos.z == world.map[world.cubePos.x - 1][world.cubePos.y]) return true;
			else return false;
		}
		else if (level == UP) {
			if (world.cubePos.z == world.map[world.cubePos.x - 1][world.cubePos.y] - 1) return true;
			else return false;
		}
		else if (level == DOWN) {
			if (world.cubePos.z == world.map[world.cubePos.x - 1][world.cubePos.y] + 1) return true;
			else return false;
		}
		break;
	}

	return false;
}

bool checkGoDown() {
	if (world.cubePos.z > world.map[world.cubePos.x - 1][world.cubePos.y - 1]) return true;
	else return false;
}

bool checkIllegalValues(vec2 pos) 
{
	if (pos.x < 0 || pos.x > world.map.size()-1) return true;
	else if (pos.y < 0 || pos.y > world.map[pos.x].size()-1) return true;
	//else if (map[pos.x][pos.y] == -1) return true;
	else return false;
}

void updateAnimationLoop()
{
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Use our shader
  glUseProgram(programID);

  frameTime = glfwGetTime() - gameTime;
  gameTime = glfwGetTime();
  cube.move = frameTime*100;
  cycleTime += frameTime;
  int vertexColorLocation = glGetUniformLocation(programID, "myTime");
  glUniform1f(vertexColorLocation, gameTime);

  if (falling) {
	  cube.fall();
	  if (cube.pos.z < -80) {
		  falling = false;  
		  cube.reset();
		  world.cubePos.x = (int)(round(cube.pos.x) / 20);
		  world.cubePos.y = (int)(round(cube.pos.y) / 20);
		  world.cubePos.z = (int)(round(cube.pos.z - 10) / 20);
	  }
  }
  else if (won) {
	  cube.won();
	  if (cube.pos.z > 100) {
		  won = false;
		  cube.reset();
		  world.cubePos.x = (int)(round(cube.pos.x) / 20);
		  world.cubePos.y = (int)(round(cube.pos.y) / 20);
		  world.cubePos.z = (int)(round(cube.pos.z - 10) / 20);
	  }
  }
  else if (glfwGetKey(window, GLFW_KEY_W) && (lastKey == GLFW_KEY_W || lastKey == 0) && cycleTime < 0.2) {
	  if (checkNextCubePos(FORWARD, STRAIGHT)) cube.role(FORWARD);
	  else if (checkNextCubePos(FORWARD, UP)) cube.roleUp(FORWARD);
	  else if (checkGoDown()) cube.goDown(FORWARD);
	  if (lastKey != GLFW_KEY_W) {
		  lastKey = GLFW_KEY_W;
		  cycleTime = 0;
	  }
  }
  else if (glfwGetKey(window, GLFW_KEY_S) && (lastKey == GLFW_KEY_S || lastKey == 0) && cycleTime < 0.2) {
	  if (checkNextCubePos(BACKWARD, STRAIGHT)) cube.role(BACKWARD);
	  else if (checkNextCubePos(BACKWARD, UP)) cube.roleUp(BACKWARD);
	  else if (checkGoDown()) cube.goDown(BACKWARD);
	  if (lastKey != GLFW_KEY_S) {
		  lastKey = GLFW_KEY_S;
		  cycleTime = 0;
	  }
  }
  else if (glfwGetKey(window, GLFW_KEY_A) && (lastKey == GLFW_KEY_A || lastKey == 0) && cycleTime < 0.2) {
	  if (checkNextCubePos(LEFT, STRAIGHT)) cube.role(LEFT);
	  else if (checkNextCubePos(LEFT, UP)) cube.roleUp(LEFT);
	  else if (checkGoDown()) cube.goDown(LEFT);
	  if (lastKey != GLFW_KEY_A) {
		  lastKey = GLFW_KEY_A;
		  cycleTime = 0;
	  }
  }
  else if (glfwGetKey(window, GLFW_KEY_D) && (lastKey == GLFW_KEY_D || lastKey == 0) && cycleTime < 0.2) {
	  if (checkNextCubePos(RIGHT, STRAIGHT)) cube.role(RIGHT);
	  else if (checkNextCubePos(RIGHT, UP)) cube.roleUp(RIGHT);
	  else if (checkGoDown()) cube.goDown(RIGHT);
	  if (lastKey != GLFW_KEY_D) {
		  lastKey = GLFW_KEY_D;
		  cycleTime = 0;
	  }
  }
  else if (glfwGetKey(window, GLFW_KEY_O)) cam_z += 0.01;
  else if (glfwGetKey(window, GLFW_KEY_L)) cam_z -= 0.01;
  else {  
	  if (cube.roleFinish()) {
		 //if((int)cube.pos.x % 20 == 0 && cube.pos.x > 0)
			//cube.rot.x -= ((int) (cube.rot.x * 100) % 157) / 100;
		 //if((int)cube.pos.y % 20 == 0 && cube.pos.y > 0)
			//cube.rot.y -= ((int) (cube.rot.y * 100) % 157) / 100;
		 //corect cube...
		 lastKey = 0;
		 if (world.checkFallDown()) falling = true;
		 else if (world.checkWin()) {
			 std::cout << "Won\n";
			 won = true;
		 }
	  }
	  cycleTime = 0;
	  world.cubePos.x = (int)(round(cube.pos.x) / 20);
	  world.cubePos.y = (int)(round(cube.pos.y) / 20);
	  world.cubePos.z = (int)(round(cube.pos.z - 10) / 20);
	  cube.currLevel = world.cubePos.z;
  }

  //world.cubePos.x = (int)(round(cube.pos.x) / 20);
  //world.cubePos.y = (int)(round(cube.pos.y) / 20);

  //checkCubePos();

  initializeMVPTransformation();
  
  if (i++ == 5000) {
      std::cout << "POS_X: " << cube.pos.x << "\n";
	  std::cout << "POS_Y: " << cube.pos.y << "\n";
	  //std::cout << "ROT_X: " << cube.rot.x << "\n";
      //std::cout << "CUBEPOS_X: " << world.cubePos.x << "\n";
      //std::cout << "CUBEPOS_Y: " << world.cubePos.y << "\n";
	  std::cout << "WORLD_CUBEPOS_Z: " << world.cubePos.z << "\n";
	  std::cout << "CUBE_POS_Z:" << cube.pos.z << "\n";
      //std::cout << "CUBE_ROTPOS: " << cube.rotPos << "\n";
	  //std::cout << "CUBE_ROTPOSOFFSET: " << cube.rotPosOffset << "\n";
	  std::cout << "CUBE_LASTLEVEL: " << cube.lastLevel << "\n";
	  std::cout << "CUBE_CURRLEVEL: " << cube.currLevel << "\n";
	  std::cout << "LASTKEY: " << lastKey << "\n";
	  //std::cout << "Time: " << timeValue << "\n";
	  std::cout << "FrameTime: " << frameTime << "\n";
	  std::cout << "CUBE_POS_x %20: " << (int)cube.pos.x % 20 << "\n";
      i = 0;
  }
  // Send our transformation to the currently bound shader, 
  // in the "MVP" uniform
  glUniformMatrix4fv(View_Matrix_ID, 1, GL_FALSE, &V[0][0]);
  glUniformMatrix4fv(Projection_Matrix_ID, 1, GL_FALSE, &P[0][0]);
  glUniformMatrix4fv(Model_Matrix_ID, 1, GL_FALSE, &ground.M[0][0]);
  ground.DrawObject();
  
  worldObj.DrawObject();

  glUniformMatrix4fv(Model_Matrix_ID, 1, GL_FALSE, &finish.M[0][0]);
  finish.DrawObject();

  updataMovingObjectTransformation();
  glUniformMatrix4fv(Model_Matrix_ID, 1, GL_FALSE, &cubeObj.M[0][0]);
  cubeObj.DrawObject();

  // Swap buffers
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void updataMovingObjectTransformation()
{
  cubeObj.M = glm::translate(glm::mat4(1.0f), { cube.pos.x, cube.pos.z, cube.pos.y });
  cubeObj.M = glm::rotate(cubeObj.M, cube.rot.x, { 0.0f,0.0f,1.0f });

  //For correct rotation to left...
  if (cube.rotPos == 0) cubeObj.M = glm::rotate(cubeObj.M, cube.rot.y, { -1.0f,0.0f,0.0f });
  else if(cube.rotPos == 1) cubeObj.M = glm::rotate(cubeObj.M, cube.rot.y, { 0.0f,-1.0f,0.0f });
  else if (cube.rotPos == 2) cubeObj.M = glm::rotate(cubeObj.M, cube.rot.y, { 1.0f,0.0f,0.0f });
  else if (cube.rotPos == 3) cubeObj.M = glm::rotate(cubeObj.M, cube.rot.y, { 0.0f,1.0f,0.0f });
  
  //cubeObj.M = glm::rotate(cubeObj.M, curr_angle, { 1.0f,0.0f,0.0f });
  
}

bool initializeWindow()
{
  // Initialise GLFW
  if (!glfwInit())
  {
    fprintf(stderr, "Failed to initialize GLFW\n");
    getchar();
    return false;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(1024, 768, "Example: simple cube", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
    getchar();
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
    return false;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Dark blue background
  glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
  return true;
}

bool initializeMVPTransformation()
{
  // Get a handle for our "MVP" uniform
  Model_Matrix_ID = glGetUniformLocation(programID, "M");
  Projection_Matrix_ID = glGetUniformLocation(programID, "P");
  View_Matrix_ID = glGetUniformLocation(programID, "V");
    
  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  P = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 10000.0f);
   
  // Camera matrix
  V = glm::lookAt(
    glm::vec3(cube.pos.x-200, 200, cube.pos.y+cam_z), // Camera is at (4,3,-3), in World Space
    glm::vec3(cube.pos.x, 0, cube.pos.y), // and looks at the origin
    glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
  );
  
  return true;
  
}

bool initializeVertexbuffer()
{
  //####################### FIRST OBJECT: GROUND ###################
  /*ground = RenderingObject();
  ground.InitializeVAO();
  
  //create vertex data
  std::vector< glm::vec3 > vertices = std::vector< glm::vec3 >();
  vertices.push_back({-120,-50,-120});
  vertices.push_back({-120,-50,120 });
  vertices.push_back({ 120,-50,120 });
  vertices.push_back({ -120,-50,-120 });
  vertices.push_back({ 120,-50,120 });
  vertices.push_back({ 120,-50,-120 });
  ground.SetVertices(vertices);
  
  //create normal data
  std::vector< glm::vec3 > normals = std::vector< glm::vec3 >();
  ground.computeVertexNormalsOfTriangles(vertices, normals);
  ground.SetNormals(normals);

  //create texture data
  ground.textureSamplerID = glGetUniformLocation(programID, "myTextureSampler");
  float scaling = 5.0f;
  std::vector< glm::vec2 > uvbufferdata;
  uvbufferdata.push_back({ 0.0f, 0.0f });
  uvbufferdata.push_back({ 0.0f, scaling });
  uvbufferdata.push_back({ scaling, scaling });
  uvbufferdata.push_back({ 0.0f,0.0f });
  uvbufferdata.push_back({ scaling,scaling });
  uvbufferdata.push_back({ scaling,0.0f });
  ground.SetTexture(uvbufferdata, "brick_2.bmp");*/

  //####################### SECOND OBJECTS ###################
  cubeObj = RenderingObject();
  cubeObj.InitializeVAO();
  cubeObj.LoadSTL("Cube.stl");
  cubeObj.SetColor(1.0, 1.0, 1.0, 0.7); //If Speed is greater then 0. Colors change.
  cube = Cube(std::make_shared<RenderingObject>(cubeObj), 40, 60, 30);
  //testCube = new std::make_shared<Cube>(tempCube);

  worldObj = RenderingObject();
  worldObj.InitializeVAO();
  worldObj.LoadSTL("World1.stl");
  world = World(std::make_shared<RenderingObject>(worldObj), map1, 1, 3, 0);

  makeFinish();


  return true;
}

void makeFinish() {
	finish = RenderingObject();
	finish.InitializeVAO();
	finish.LoadSTL("Cube.stl");
	finish.SetColor(1.0, 1.0, 1.0, 0.8); //If Speed is greater then 0. Colors change.
	glm::vec3 pos = world.getFinish();
	std::cout << pos.y * 20 << "\n";
	finish.M = glm::translate(glm::mat4(1.0f), { pos.x * 20 + 20, pos.z * 20 - 9, pos.y * 20 + 20});

	/*std::vector< glm::vec3 > vertices = std::vector< glm::vec3 >();
	glm::vec3 pos = world.getFinish();
	vertices.push_back({ pos.x * 20, pos.y * 20, pos.z * 20 + 12 });
	vertices.push_back({ pos.x * 20 + 20, pos.y * 20, pos.z * 20 + 12 });
	vertices.push_back({ pos.x * 20, pos.y * 20 + 20, pos.z * 20 + 12 });
	vertices.push_back({ pos.x * 20, pos.y * 20 + 20, pos.z * 20 + 12 });
	vertices.push_back({ pos.x * 20 + 20, pos.y * 20, pos.z * 20 + 12 });
	vertices.push_back({ pos.x * 20 + 20, pos.y * 20 + 20, pos.z * 20 + 12 });
	finish.SetVertices(vertices);

	std::vector< glm::vec3 > normals = std::vector< glm::vec3 >();
	finish.computeVertexNormalsOfTriangles(vertices, normals);
	finish.SetNormals(normals);

	finish.SetColor(1.0, 1.0, 1.0, 0.7);*/
}

bool cleanupVertexbuffer()
{
  // Cleanup VBO
  glDeleteVertexArrays(1, &ground.VertexArrayID);
  return true;
}

bool closeWindow()
{
  glfwTerminate();
  return true;
}

void fall() {
	float test = cube.pos.z;
	do {
		cube.pos.z -= 0.1;
		test = cube.pos.z;
		updateAnimationLoop();
	} while ( test > 0);
	cube.reset();
}



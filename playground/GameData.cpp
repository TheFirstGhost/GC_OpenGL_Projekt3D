#include "GameData.h"
#include <iostream>
#include <ctime>

World::World(std::shared_ptr <RenderingObject> _worldObject, std::vector<std::vector<int>> _map, int cubePosX, int cubePosY, int cubePosZ) {
	worldObject = _worldObject;
	map = _map;
	cubePos = vec3(cubePosX, cubePosY, cubePosZ);
}

World::World() {
	//worldObject = null;
}

bool World::checkFallDown() {
	if (cubePos.x - 1 < 0 || cubePos.x - 1 > map.size()-1) return true;
	else if (cubePos.y - 1 < 0 || cubePos.y - 1 > map[cubePos.x - 1].size()-1) return true;
	else if (map[cubePos.x - 1][cubePos.y - 1] == -1) return true;
	else return false;
}

bool World::checkWin() {
	if (map[cubePos.x - 1][cubePos.y - 1] <= -10) return true;
	else return false;
}

glm::vec3 World::getFinish() {
	glm::vec3 finish;
	for (int x = 0; x < map.size(); x++) {
		for (int y = 0; y < map[x].size(); y++) {
			if (map[x][y] <= -10) return glm::vec3(x, y, map[x][y] + 11);
		}
	}
	return glm::vec3(-100, -100, 100);
}


Cube::Cube() {

}

Cube::Cube(std::shared_ptr <RenderingObject> _cubeObject, glm::vec3 _pos) {
	cubeObject = _cubeObject;
	pos = _pos;
	rot = vec3(0, 0, 0);
	rotPos = 0;
	rotPosOffset = 0;
	lastLevel = 1;
	currLevel = 1;
}

Cube::Cube(std::shared_ptr <RenderingObject> _cubeObject, double x, double y, double z) {
	cubeObject = _cubeObject;
	pos = vec3(x, y, z);
	rot = vec3(0, 0, 0);
	rotPos = 0;
	rotPosOffset = 0;
	lastLevel = 1;
	currLevel = 1;
}

void Cube::role(const int direction) {
	//lastLevel = currLevel;
	
	lastDirection = direction;
	lastHight = STRAIGHT;
	switch (direction)
	{
		case FORWARD:
			setRotPosOffset(FORWARD);
			pos.x += 0.02; //40
			rot.x -= 0.00157; //1.57...
			break;
		case BACKWARD:
			setRotPosOffset(BACKWARD);
			pos.x -= 0.02;
			rot.x += 0.00157;
			break;
		case LEFT:
			lastLevel = currLevel;
			pos.y -= 0.02;
			rot.y -= 0.00157;
			break;
		case RIGHT:
			lastLevel = currLevel;
			pos.y += 0.02;
			rot.y += 0.00157;
			break;
		default:
			break;
	}
	//std::cout << "POS_X: " << pos.x << "\n";
	//std::cout << "ROT_X: " << rot.x << "\n";
}

void Cube::roleUp(const int direction) {
	
	lastDirection = direction;
	lastHight = UP;
	switch (direction)
	{
	case FORWARD:
		setRotPosOffset(FORWARD);
		pos.z += 0.02;
		rot.x -= 0.00157;
		break;
	case BACKWARD:
		setRotPosOffset(BACKWARD);
		pos.z += 0.02;
		rot.x += 0.00157;
		break;
	case LEFT:
		lastLevel = currLevel;
		pos.z += 0.02;
		rot.y -= 0.00157;
		break;
	case RIGHT:
		lastLevel = currLevel;
		pos.z += 0.02;
		rot.y += 0.00157;
		break;
	default:
		break;
	}
}

void Cube::goDown(const int direction) {
	//setRotPosOffset();
	lastDirection = direction;
	lastHight = DOWN;
	pos.z -= 0.02;
}

bool Cube::roleFinish() {
	if (lastHight == STRAIGHT) {
		if (((int)pos.x) % 20 > 10) {
			pos.x += 0.02;
			rot.x -= 0.00157;
		}
		else if (((int)pos.x) % 20 <= 10 && ((int)pos.x) % 20 > 0) {
			pos.x -= 0.02;
			rot.x += 0.00157;
		}

		else if (((int)pos.y) % 20 > 10) {
			pos.y += 0.02;
			rot.y += 0.00157;
		}
		else if (((int)pos.y) % 20 <= 10 && ((int)pos.y) % 20 > 0) {
			pos.y -= 0.02;
			rot.y -= 0.00157;
		}
		else {
			rotPos = calculateRotPos();
			return true;
		}
		rotPos = calculateRotPos();

		return false;
	}
	else if (lastHight == UP) {
		if (lastDirection == FORWARD) {
			setRotPosOffset(FORWARD);
			if (((int)pos.z - 10) % 20 > 0) {
				pos.z -= 0.02;
				rot.x += 0.00157;
			}
			else return true;
		}
		else if (lastDirection == BACKWARD) {
			setRotPosOffset(BACKWARD);
			if (((int)pos.z - 10) % 20 > 0) {
				pos.z -= 0.02;
				rot.x -= 0.00157;
			}
			else return true;
		}
		else if (lastDirection == LEFT) {
			lastLevel = currLevel;
			if (((int)pos.z - 10) % 20 > 0) {
				//pos.y -= 0.02;
				pos.z -= 0.02;
				rot.y += 0.00157;
			}
			else return true;
		}
		else if (lastDirection == RIGHT) {
			lastLevel = currLevel;
			if (((int)pos.z - 10) % 20 > 0) {
				//pos.y -= 0.02;
				pos.z -= 0.02;
				rot.y -= 0.00157;
			}
			else return true;
		}
		else {
			rotPos = calculateRotPos();
			return true;
		}
		rotPos = calculateRotPos();

		return false;
	}
	else if (lastHight == DOWN) {
		if (((int)pos.z-10) % 20 > 0) {
			pos.z -= 0.02;
		}
		//else if (((int)pos.x) % 20 <= 10 && ((int)pos.x) % 20 > 0) {
			//pos.z += 0.02;
		//}

		/*else if (((int)pos.y) % 20 > 10) {
			pos.y += 0.02;
			pos.z -= 0.02;
			rot.y += 0.00314;
		}
		else if (((int)pos.y) % 20 <= 10 && ((int)pos.y) % 20 > 0) {
			pos.y -= 0.02;
			pos.z += 0.02;
			rot.y -= 0.00314;
		}*/
		else {
			rotPos = calculateRotPos();
			return true;
		}
		rotPos = calculateRotPos();

		return false;
	}
	else return true;
}

int Cube::calculateRotPos() {
	return ((int)(pos.x / 20) + rotPosOffset) % 4;
}

void Cube::setRotPosOffset(const int direction) {
	switch (direction)
	{
	case FORWARD:
		if (lastLevel < currLevel) rotPosOffset++;
		break;
	case BACKWARD:
		if (lastLevel < currLevel) rotPosOffset--;
		break;
	default:
		break;
	}
	
	lastLevel = currLevel;
}

void Cube::fall() {

	pos.z -= 0.05;
	
	//if (pos.z < -80) reset();
	//std::cout << "Fall Down" << "\n";
}

void Cube::won() {
	pos.z += 0.01;
}

void Cube::reset() {
	pos.x = 40;
	pos.y = 60;
	pos.z = 30;
	rot = vec3(0, 0, 0);
	rotPos = 0;
	rotPosOffset = 0;
	lastLevel = 1;
	currLevel = 1;
}

/*bool Cube::checkNextCubePos(const int dir, std::shared_ptr<World> world, std::shared_ptr<Cube> cube) {

	//update cube position
	world->cubePos.x = (int)(cube->pos.x / 20);
	world->cubePos.y = (int)(cube->pos.y / 20);

	//std::cout << "cubePos_X: " << world->cubePos.x<< "\n";
	//std::cout << "cubePos_Y: " << world->cubePos.y << "\n";
	//std::cout << "POS_Y%20: " << ((int)pos.y) % 20 << "\n";

	switch (dir)
	{
	case 0:
		std::cout << "cubePos_X: " << world->cubePos.x << "\n";
		std::cout << "cubePos_X: " << world->cubePos.y -1 << "\n";
		if (world->cubePos.z == world->map[world->cubePos.x][world->cubePos.y - 1]) return true;
		else return false;
		break;
	case 1:
		if (world->cubePos.z == world->map[world->cubePos.x - 2][world->cubePos.y - 1]) return true;
		else return false;
		break;
	case 2:
		if (world->cubePos.z == world->map[world->cubePos.x - 1][world->cubePos.y]) return true;
		else return false;
		break;
	case 3:
		if (world->cubePos.z == world->map[world->cubePos.x - 1][world->cubePos.y - 2]) return true;
		else return false;
		break;
	}

	return false;
}*/
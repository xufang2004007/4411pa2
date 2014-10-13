#include <vector>
#include "cubegrid.h"
#include "Vec.h"
#include "mat.h"

#ifndef _METABALL_H
#define _METABALL_H

struct Metaball {
	Vec3f position;
	float squaredRadius;

	Metaball(Vec3f newPosition, float newSquaredRadius) {
		position = newPosition;
		squaredRadius = newSquaredRadius;
	}
};

class MetaballIsNotMeatball {
private:
	enum {
		NONE,
		ALLOC,
		GRID_INIT,
		CALC,
		DRAW
	} initialized;
	std::vector<Metaball> metaballs;
	CUBE_GRID cubeGrid;
	Mat4f baseInv;
	float minX, minY, minZ, maxX, maxY, maxZ;
public:	
	MetaballIsNotMeatball(int quality);
	void draw(float threshold);
	void calc();
	void addBallAbs(Vec3f center, float radius);
	void addBallRel(float x, float y, float z, float radius);
	void setBase();
	void autoInitGrid();
	void initGrid(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);
};

#endif

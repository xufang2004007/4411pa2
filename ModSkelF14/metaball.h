#include <vector>
#include "cubegrid.h"
#include "Vec.h"

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
	std::vector<Metaball> metaballs;
	CUBE_GRID cubeGrid;
	bool initialized;
	void calc();
public:	
	MetaballIsNotMeatball(float mult = 1);
	void draw(float threshold);
	void addBallAbs(Vec3f center, float radius);
	void addBallRel(float x, float y, float z, float radius);
};

#endif

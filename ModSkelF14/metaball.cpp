#include "MetaBall.h";
#include "modelerdraw.h"
#include "vec_mat_ext.h"

MetaballIsNotMeatball::MetaballIsNotMeatball(int quality) {
	initialized = NONE;

	ModelerDrawState *mds = ModelerDrawState::Instance();
	minX = HUGE; minY = HUGE; minZ = HUGE;
	maxX = -HUGE; maxY = -HUGE; maxZ = -HUGE;

	if (mds->m_rayFile) { return; }
	_setupOpenGl();

	if (!cubeGrid.CreateMemory(quality)) {
		return;
	}

	initialized = ALLOC;
}

#include "debug.h"

void MetaballIsNotMeatball::initGrid(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax) {
	if (initialized < ALLOC) { return; }
	
	stringstream ss;
	format(ss);
	ss << xMin << " " << xMax << " " << yMin << " " << yMax << " " << zMin << " " << zMax;
	alert(ss);
	cubeGrid.Init(20, -10, 20, -10, 20, -10);
	//cubeGrid.Init((xMax - xMin) / cubeGrid.gridSize, -(xMax + xMin) / 2, (yMax - yMin) / cubeGrid.gridSize, -(yMax + yMin) / 2, (zMax - zMin) / cubeGrid.gridSize, -(zMax + zMin) / 2);
	initialized = GRID_INIT;
}

void MetaballIsNotMeatball::calc() {
	if (initialized < GRID_INIT) { return; }
	// clear the field
	for (int i = 0; i< cubeGrid.numVertices; i++) {
		cubeGrid.vertices[i].value = 0.0f;
		cubeGrid.vertices[i].normal.zeroElements();
	}
	// evaluate the scalar field at each point
	Vec3f ballToPoint;
	float squaredRadius;
	Vec3f ballPosition;
	float normalScale;
	for (std::vector<Metaball>::iterator i = metaballs.begin(); i != metaballs.end(); i++) {
		squaredRadius = i->squaredRadius;
		ballPosition = i->position;

		for (int j = 0; j < cubeGrid.numVertices; j++) {
			ballToPoint = cubeGrid.vertices[j].position - ballPosition;

			float squaredDistance = ballToPoint.length2();

			if (squaredDistance == 0.0f)
				squaredDistance = 0.0001f; // although i'm not sure

			cubeGrid.vertices[j].value += squaredRadius / squaredDistance;
			normalScale = squaredRadius / (squaredDistance*squaredDistance);

			cubeGrid.vertices[j].normal += ballToPoint*normalScale;
		}
	}
	initialized = CALC;
}

void MetaballIsNotMeatball::draw(float threshold) {
	if (initialized < CALC) { return; }
	cubeGrid.DrawSurface(threshold);
	initialized = DRAW;
}

void MetaballIsNotMeatball::addBallAbs(Vec3f center, float radius) {
	metaballs.push_back(Metaball(center, radius));
}

void MetaballIsNotMeatball::addBallRel(float x, float y, float z, float radius) { // TOTO: adjust the radius accordingly (when glScale used)
	Vec3f center(x, y, z);
	float output[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, output);
	Mat4f modelMtx = Mat4fromGLMatrix(output);
	Mat4f diff = baseInv * modelMtx;
	Vec3f abs = diff * center;
	if (abs[0] > maxX) { maxX = abs[0]; }
	if (abs[1] > maxY) { maxY = abs[1]; }
	if (abs[2] > maxZ) { maxZ = abs[2]; }
	if (abs[0] < minX) { minX = abs[0]; }
	if (abs[1] < minY) { minY = abs[1]; }
	if (abs[2] < minZ) { minZ = abs[2]; }
	addBallAbs(abs, radius);
}

void MetaballIsNotMeatball::setBase() {
	float output[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, output);
	baseInv = Mat4fromGLMatrix(output).inverse();
}

void MetaballIsNotMeatball::autoInitGrid() {
	initGrid(minX, maxX, minY, maxY, minZ, maxZ);
}
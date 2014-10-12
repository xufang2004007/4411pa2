#include "MetaBall.h";
#include "modelerdraw.h"
#include "mat.h"

MetaballIsNotMeatball::MetaballIsNotMeatball(float mult) {
	initialized = false;

	ModelerDrawState *mds = ModelerDrawState::Instance();
	int quality;

	if (mds->m_rayFile) { return; }
	_setupOpenGl();

	switch (mds->m_quality)
	{
	case HIGH:
		quality = 120 * mult; break;
	case MEDIUM:
		quality = 88 * mult; break;
	case LOW:
		quality = 56 * mult; break;
	case POOR:
		quality = 24 * mult; break;
	}

	if (!cubeGrid.CreateMemory(quality)) {
		return;
	}
	if (!cubeGrid.Init(quality)) {
		return;
	}
	
	initialized = true;
}

void MetaballIsNotMeatball::calc() {
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
}

void MetaballIsNotMeatball::draw(float threshold) {
	if (!initialized) { return; }
	calc();
	cubeGrid.DrawSurface(threshold);
}

void MetaballIsNotMeatball::addBallAbs(Vec3f center, float radius) {
	metaballs.push_back(Metaball(center, radius));
}

void MetaballIsNotMeatball::addBallRel(float x, float y, float z, float radius) {
	Vec3f center(x, y, z);
	Mat4f modelMtx;
	glGetFloatv(GL_MODELVIEW_MATRIX, modelMtx[0]);
	Vec3f abs = modelMtx * center;
	addBallAbs(abs, radius);
}

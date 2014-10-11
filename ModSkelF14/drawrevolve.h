#include "modelerdraw.h"
#include <FL/gl.h>
#include <GL/glu.h>
#include <math.h>

class DrawRevolve {
private:
	ModelerDrawState *mds;

	int sides;
	int tolerance;

	DrawRevolve();

	void makeTKnobVector(int degree, float* &output);
	void makeSKnobVector(int sides, float* &output);
	void drawRevolve(int length, double normal[], double axial[]);
	void drawTorus(double R, double r);
public:
	// static void drawRevolve();
	static void torus(double R, double r);
};
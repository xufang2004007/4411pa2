#include "modelerdraw.h"
#include <FL/gl.h>
#include <GL/glu.h>
#include <math.h>

class DrawRevolve {
private:
	ModelerDrawState *mds;

	int sides;
	int tolerance;
	int slength;
	GLUnurbs* nurb;

	DrawRevolve(ModelerDrawState* mds);
	static DrawRevolve* factory();

	void makeTKnobVector(int degree, float* &output);
	void makeSKnobVector(float* &output);
	void drawRevolve(int length, double normal[], double axial[], int repeatX, int repeatY);
	void drawTorus(double tr, double cr, int repeatX, int repeatY);

public:
	static void torus(double R, double r, int repeatX = 1, int repeatY = 1);
	static void revolve(int length, double x[], double z[], int repeatX = 1, int repeatY = 1);
};
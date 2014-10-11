#include "drawrevolve.h"

// http://www.saccade.com/writing/graphics/KnotVectors.pdf
void DrawRevolve::makeTKnobVector(int degree, float* &output) {
	output = new float[degree + 4];
	for (int i = 0; i < 3; i++) {
		output[i] = 0;
	}
	for (int i = 0; i < degree - 2; i++) {
		output[i + 3] = i;
	}
	for (int i = 1; i < 4; i++) {
		output[degree + i] = degree - 3;
	}
};

// http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/NURBS/RB-circles.html
void DrawRevolve::makeSKnobVector(int sides, float* &output) {
	output = new float[sides * 2 + 4];
	output[0] = 0;
	for (int i = 0; i <= sides; i++) {
		output[1 + i * 2] = i;
		output[2 + i * 2] = i;
	}
	output[sides * 2 + 3] = sides;
}

/*
void printV(float* f, int l) {
	std::stringstream ss;
	ss << "[";
	for (int i = 0; i < l; i++) {
		ss << f[i] << " ";
	}
	ss << "]" << std::endl;
	fl_alert(ss.str().c_str());
}
*/

DrawRevolve::DrawRevolve(ModelerDrawState* mds) {
	switch (mds->m_quality)
	{
	case HIGH:
		sides = 32; tolerance = 16; break;
	case MEDIUM:
		sides = 20; tolerance = 24; break;
	case LOW:
		sides = 12; tolerance = 40; break;
	case POOR:
		sides = 8; tolerance = 64; break;
	}
}

// revolve around z-axis only. want others? use glTranslate and glRotate by yourself! I DON'T FUCKING DO THAT FOR U!!!
void DrawRevolve::drawRevolve(int length, double normal[], double axial[]) {
	_setupOpenGl();
	if (mds->m_rayFile) { return; }

	GLUnurbs* nurb = gluNewNurbsRenderer();
	gluNurbsProperty(nurb, GLU_SAMPLING_TOLERANCE, tolerance);
	gluNurbsProperty(nurb, GLU_DISPLAY_MODE, GLU_FILL);
	gluBeginSurface(nurb);
	int slength = sides * 2 + 1;
	float* sknots;
	makeSKnobVector(sides, sknots);
	float* tknots;
	makeTKnobVector(length, tknots);
	float* cps = new float[4 * slength * length];
	#define P(d1, d2, d3) (cps[((d1 * slength) + d2) * 4 + d3])
	for (int t = 0; t < length; t++) {
		double n = normal[t];
		double a = axial[t];
		double w = sin(M_PI / 2 - M_PI / sides);
		double r = n / w;
		for (int s = 0; s < slength; s++) {
			double angle = s * M_PI / sides;
			P(t, s, 2) = a;
			if (s & 1) {
				P(t, s, 3) = w;
				P(t, s, 0) = r * cos(angle); P(t, s, 1) = r * sin(angle);
			} else {
				P(t, s, 3) = 1;
				P(t, s, 0) = n * cos(angle); P(t, s, 1) = n * sin(angle);
			}
		}
	}
	#undef P
	gluNurbsSurface(nurb, slength + 3, sknots, length + 4, tknots, 4, 4 * slength, cps, 3, 4, GL_MAP2_VERTEX_4);
	gluEndSurface(nurb);
}

void DrawRevolve::drawTorus(double R, double r) {

}

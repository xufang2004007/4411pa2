#include "drawrevolve.h"

#define P(d1, d2, d3) (cps[((d1 * slength) + d2) * 4 + d3])
#define T(d1, d2, d3) (texture[((d1 * slength) + d2) * 2 + d3])

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
void DrawRevolve::makeSKnobVector(float* &output) {
	output = new float[sides * 2 + 4];
	output[0] = 0;
	for (int i = 0; i <= sides; i++) {
		output[1 + i * 2] = i;
		output[2 + i * 2] = i;
	}
	output[sides * 2 + 3] = sides;
}

DrawRevolve::DrawRevolve(ModelerDrawState* mds) : mds(mds) {
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
	slength = sides * 2 + 1;
	nurb = gluNewNurbsRenderer();
	gluNurbsProperty(nurb, GLU_SAMPLING_TOLERANCE, tolerance);
	gluNurbsProperty(nurb, GLU_DISPLAY_MODE, GLU_FILL);
}

// revolve around z-axis only. want others? use glTranslate and glRotate by yourself! I DON'T FUCKING DO THAT FOR U!!!
void DrawRevolve::drawRevolve(int length, double normal[], double axial[], int repeatX, int repeatY) {
	gluBeginSurface(nurb);
	float* sknots;
	makeSKnobVector(sknots);
	float* tknots;
	makeTKnobVector(length, tknots);
	float* cps = new float[4 * slength * length];
	float* texture = new float[2 * slength * length];
	double w = sin(M_PI / 2 - M_PI / sides);
	for (int t = 0; t < length; t++) {
		double n = normal[t];
		double a = axial[t];
		float tt = fmod(t * repeatX / (float)length, 1);
		double r = n / w;
		for (int s = 0; s < slength; s++) {
			double angle = s * M_PI / sides;
			T(t, s, 0) = tt;
			T(t, s, 1) = fmod(s * repeatY / (float)slength, 1);
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
	gluNurbsSurface(nurb, slength + 3, sknots, length + 4, tknots, 4, 4 * slength, cps, 3, 4, GL_MAP2_VERTEX_4);
	gluNurbsSurface(nurb, slength + 3, sknots, length + 4, tknots, 2, 2 * slength, texture, 3, 4, GL_MAP2_TEXTURE_COORD_2);
	gluEndSurface(nurb);
}

void DrawRevolve::drawTorus(double tr, double cr, int repeatX, int repeatY) {
	gluBeginSurface(nurb);
	float* sknots;
	makeSKnobVector(sknots);
	float* cps = new float[4 * slength * slength];
	float* texture = new float[2 * slength * slength];
	double w = sin(M_PI / 2 - M_PI / sides);
	double or = cr / w;
	for (int t = 0; t < slength; t++) {
		double outerAngle = t * M_PI / sides;
		double a, n, o;
		if (t & 1) {
			o = w;
			n = tr + or * cos(outerAngle);
			a = or * sin(outerAngle);
		} else {
			o = 1;
			n = tr + cr * cos(outerAngle);
			a = cr * sin(outerAngle);
		}
		float tt = fmod(t * repeatX / (float) slength, 1);
		double r = n / w;
		for (int s = 0; s < slength; s++) {
			double angle = s * M_PI / sides;
			T(t, s, 0) = tt;
			T(t, s, 1) = fmod(s * repeatY / (float) slength, 1);
			P(t, s, 2) = a;
			if (s & 1) {
				P(t, s, 3) = w * o;
				P(t, s, 0) = r * cos(angle); P(t, s, 1) = r * sin(angle);
			} else {
				P(t, s, 3) = o;
				P(t, s, 0) = n * cos(angle); P(t, s, 1) = n * sin(angle);
			}
		}
	}
	gluNurbsSurface(nurb, slength + 3, sknots, slength + 3, sknots, 4, 4 * slength, cps, 3, 3, GL_MAP2_VERTEX_4);
	gluNurbsSurface(nurb, slength + 3, sknots, slength + 3, sknots, 2, 2 * slength, texture, 3, 3, GL_MAP2_TEXTURE_COORD_2);
	gluEndSurface(nurb);
}

void DrawRevolve::torus(double R, double r, int repeatX, int repeatY) {
	DrawRevolve* instance = factory();
	if (instance) {
		instance->drawTorus(R, r, repeatX, repeatY);
	}
}

void DrawRevolve::revolve(int length, double x[], double z[], int repeatX, int repeatY) {
	DrawRevolve* instance = factory();
	if (instance) {
		instance->drawRevolve(length, x, z, repeatX, repeatY);
	}
}

DrawRevolve* DrawRevolve::factory() {
	ModelerDrawState* mds = ModelerDrawState::Instance();
	if (mds->m_rayFile) { return NULL; }
	_setupOpenGl();
	return new DrawRevolve(mds);
}
#include "modelerdraw.h"

namespace HandDrawer {

	struct p {
		p(double a, double b, double c) : x(a), y(b), z(c) {};
		double x;
		double y;
		double z;
	};

	// Hand
	p lt = p(-2, 1, 0);
	p lb = p(-2, -1, 0);
	p rt = p(2, 1, 0);
	p rb = p(2, -1, 0);
	p vt[] = {
		p(-1.5, 0.5, 1),
		p(-0.9, 0.65, 1.6),
		p(-0.3, 0.5, 3),
		p(0.3, 0.5, 3),
		p(0.9, 0.5, 3),
		p(1.5, 0.5, 3)
	};
	p vb[] = {
		p(-2, -0.5, 1),
		p(-1.2, -0.5, 1.8),
		p(-0.4, -0.5, 3),
		p(0.4, -0.5, 3),
		p(1.2, -0.5, 3),
		p(2, -0.5, 3)
	};
	p h[] = {
		p(-3, 0, 3),
		p(-0.7, 0, 5),
		p(0, 0, 5.5),
		p(0.7, 0, 5),
		p(1.4, 0, 4.5)
	};

	void tri(p p1, p p2, p p3) {
		drawTriangle(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, p3.x, p3.y, p3.z);
	}

	void draw() {
		for (int i = 0; i < 5; i++) {
			tri(vt[i], vb[i], h[i]);
			tri(vt[i], vt[i + 1], h[i]);
			tri(vt[i + 1], vb[i + 1], h[i]);
			tri(vb[i], vb[i + 1], h[i]);
		}
		for (int i = 1; i < 5; i++) {
			tri(vt[i], vt[i + 1], rt);
			tri(vb[i], vb[i + 1], rb);
		}
		tri(vt[1], lt, rt);
		tri(vt[0], vt[1], lt);
		tri(vb[1], lb, rb);
		tri(vb[0], vb[1], lb);
		tri(vt[0], vb[0], lt);
		tri(lt, lb, vb[0]);
		tri(vt[5], vb[5], rt);
		tri(rt, rb, vb[5]);
	}

};
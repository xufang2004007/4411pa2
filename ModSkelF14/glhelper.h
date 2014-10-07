#include <array>
#include <stack>

// do NOT use glPushMatrix() directly, as you might forget to pop them. Also, it will cause the indentation to corrupt with some IDEs.
// use this instead
#define GLMATRIX(stmt) { glPushMatrix(); stmt; glPopMatrix(); }

std::stack<std::array<float, 3>> color_stack;

// do NOT use setDiffuseColor() directly, as you might forget to recover the original color.
// use this instead
#define GLCOLOR(colorrgb, stmt) {\
	setDiffuseColor(colorrgb);\
	color_stack.push({{colorrgb}});\
	stmt;\
	color_stack.pop();\
	if (!color_stack.empty()) {\
		setDiffuseColor(color_stack.top()[0], color_stack.top()[1], color_stack.top()[2]);\
	}\
}
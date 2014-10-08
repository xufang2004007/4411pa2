#include <array>
#include <stack>

// do NOT use glPushMatrix() directly, as you might forget to pop them. Also, it will cause the indentation to corrupt with some IDEs.
// use this instead
#define GLMATRIX(stmt) { glPushMatrix(); stmt; glPopMatrix(); }

struct ColorOrTexture {
	ColorOrTexture(float r, float g, float b) {
		isTexture = false;
		data.color = {{ r, g, b }};
	}
	ColorOrTexture(GLuint texture) {
		isTexture = true;
		data.texture = texture;
	}
	bool isTexture;
	union {
		std::array<float, 3> color;
		GLuint texture;
	} data;
};

std::stack<ColorOrTexture> color_stack;

void recoverColorOrTexture() {
	if (!color_stack.empty()) {
		if (color_stack.top().isTexture) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, color_stack.top().data.texture);
		} else {
			glDisable(GL_TEXTURE_2D);
			setDiffuseColor(color_stack.top().data.color[0], color_stack.top().data.color[1], color_stack.top().data.color[2]);
		}
	}
}

// do NOT use setDiffuseColor() directly, as you might forget to recover the original color.
// use this instead
#define GLDRAWWITH(param, stmt) {\
	color_stack.push(ColorOrTexture(param));\
	recoverColorOrTexture();\
	stmt;\
	color_stack.pop();\
	recoverColorOrTexture();\
}

// this is UGLY. I wish there will be better implementations
#define GLMAYBEDRAWWITH(param, condition, stmt) {\
	if (condition) {\
		GLDRAWWITH(param, stmt);\
	} else {\
		stmt;\
	}\
}
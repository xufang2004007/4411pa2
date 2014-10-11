#include <FL/gl.h>
#include <gl/GLU.h>
#include "bitmap.h"

#define checkImageWidth 64
#define checkImageHeight 64

namespace Texture {
	enum Textures {
		FACE,
		CHECK,
		DONUT,
		STRAW,
		TEXTURE_COUNT
	};

	GLuint texture[TEXTURE_COUNT];

	GLubyte checkImage[checkImageWidth][checkImageHeight][3];

	void makeCheckImage(){
		int i, j, c;
		for (i = 0; i < checkImageWidth; i++) {
			for (j = 0; j < checkImageHeight; j++) {
				c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;
				checkImage[i][j][0] = (GLubyte)c;
				checkImage[i][j][1] = (GLubyte)c;
				checkImage[i][j][2] = (GLubyte)c;
			}
		}
	}

	void initWithBMP(int index, char* filename) {
		glBindTexture(GL_TEXTURE_2D, texture[index]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // scale linearly when image bigger than texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // scale linearly when image smalled than texture
		int sizeX, sizeY;
		unsigned char* iData = readBMP(filename, sizeX, sizeY);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, iData);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	}

	void initWithCheck(int index) {
		glBindTexture(GL_TEXTURE_2D, texture[index]);
		makeCheckImage();
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, checkImageWidth, checkImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, &checkImage[0][0][0]);
		glShadeModel(GL_FLAT);
	}

	void init() {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(TEXTURE_COUNT, texture);

		initWithBMP(FACE, "face.bmp");
		initWithCheck(CHECK);
		initWithBMP(DONUT, "donut.bmp");
		initWithBMP(STRAW, "straw.bmp");
	}

};
// The sample model.  You should build a file
// very similar to this for when you make your model.
#define _CRT_SECURE_NO_DEPRECATE

#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include <FL/gl.h>
#include <gl/GLU.h>
#include "modelerglobals.h"
#include "glhelper.h"
#include "camera.h"
#include "handdrawer.cpp"

#define createModelerControl(index, name, minimum, maximum, stepsize, value) {\
	defaultValues[index] = value;\
	controls[index] = ModelerControl(name, minimum, maximum, stepsize, value);\
}

double defaultValues[NUMCONTROLS];

GLuint texture[2];

struct Image {

	unsigned long sizeX;

	unsigned long sizeY;

	char *data;

};

typedef struct Image Image;


#define checkImageWidth 64

#define checkImageHeight 64


GLubyte checkImage[checkImageWidth][checkImageHeight][3];

void makeCheckImage(void){

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



int ImageLoad(char *filename, Image *image) {

	FILE *file;

	unsigned long size; // size of the image in bytes.

	unsigned long i; // standard counter.

	unsigned short int planes; // number of planes in image (must be 1)

	unsigned short int bpp; // number of bits per pixel (must be 24)

	char temp; // temporary color storage for bgr-rgb conversion.

	// make sure the file is there.

	if ((file = fopen(filename, "rb")) == NULL){

		printf("File Not Found : %s\n", filename);

		return 0;

	}

	// seek through the bmp header, up to the width/height:

	fseek(file, 18, SEEK_CUR);

	// read the width

	if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {

		printf("Error reading width from %s.\n", filename);

		return 0;

	}

	//printf("Width of %s: %lu\n", filename, image->sizeX);

	// read the height

	if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {

		printf("Error reading height from %s.\n", filename);

		return 0;

	}

	//printf("Height of %s: %lu\n", filename, image->sizeY);

	// calculate the size (assuming 24 bits or 3 bytes per pixel).

	size = image->sizeX * image->sizeY * 3;

	// read the planes

	if ((fread(&planes, 2, 1, file)) != 1) {

		printf("Error reading planes from %s.\n", filename);

		return 0;

	}

	if (planes != 1) {

		printf("Planes from %s is not 1: %u\n", filename, planes);

		return 0;

	}

	// read the bitsperpixel

	if ((i = fread(&bpp, 2, 1, file)) != 1) {

		printf("Error reading bpp from %s.\n", filename);

		return 0;

	}

	if (bpp != 24) {

		printf("Bpp from %s is not 24: %u\n", filename, bpp);

		return 0;

	}

	// seek past the rest of the bitmap header.

	fseek(file, 24, SEEK_CUR);

	// read the data.

	image->data = (char *)malloc(size);

	if (image->data == NULL) {

		printf("Error allocating memory for color-corrected image data");

		return 0;

	}

	if ((i = fread(image->data, size, 1, file)) != 1) {

		printf("Error reading image data from %s.\n", filename);

		return 0;

	}

	for (i = 0; i<size; i += 3) { // reverse all of the colors. (bgr -> rgb)

		temp = image->data[i];

		image->data[i] = image->data[i + 2];

		image->data[i + 2] = temp;

	}

	// we're done.

	return 1;

}


Image * loadTexture(){

	Image *image1;

	// allocate space for texture

	image1 = (Image *)malloc(sizeof(Image));

	if (image1 == NULL) {

		printf("Error allocating space for image");

		exit(0);

	}

	if (!ImageLoad("face.bmp", image1)) {

		exit(1);

	}

	return image1;

}



// To make a SampleModel, we inherit off of ModelerView
class SampleModel : public ModelerView
{
public:
	SampleModel(int x, int y, int w, int h, char *label)
		: ModelerView(x, y, w, h, label) { }

	void draw();
	void drawGirl();
private:
	void ModelerViewDraw();
	bool isAnimating();
	double VAL(SampleModelControls index);
	int animateCounter;
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createSampleModel(int x, int y, int w, int h, char *label)
{
	return new SampleModel(x, y, w, h, label);
}

void drawBoxFromBottomCenter(double x, double y, double z) {
	GLMATRIX({
		glTranslated(-x / 2, -y / 2, 0);
		drawBox(x, y, z);
	});
}

/*
double animationLinear(double progress) {
	double p = fmod(progress, 1);
	if (p < 0.25) { return p * 4; }
	if (p < 0.75) { return 2 - p * 4; }
	return p * 4 - 4;
}
*/

double animationEase(double progress) {
	double p = fmod(progress, 1);
	return sin(progress * M_PI * 2);
}

double SampleModel::VAL(SampleModelControls index) {
	if (isAnimating() && index > CUSTOM_ANIMATION) {
		double progress = animateCounter / (double) ANIMATION_FRAMES_COUNT;
		if (VAL(NSFW)) {
			switch (index) {
			case BOY_GIRL_BIND:
				return 1;
			case BOY_GIRL_SIDE:
				return 1;
			case BOY_GIRL_ANGLE:
				return 170;
			case BOY_HEAD_PITCH:
				return animationEase(progress * 2) * 10 + 5;
			case BOY_SHOULDER_ANGLE:
				return 45 - animationEase(progress * 3) * 10;
			case BOY_ELBOW_ANGLE:
				return 130 + animationEase(progress * 3) * 5;
			case BOY_LEFT_UPPER_LEG_PITCH_ANGLE:
			case BOY_RIGHT_UPPER_LEG_PITCH_ANGLE:
			case BOY_LEFT_LOWER_LEG_PITCH_ANGLE:
			case BOY_RIGHT_LOWER_LEG_PITCH_ANGLE:
				return 75;
			case BOY_LEFT_UPPER_LEG_ROLL_ANGLE:
			case BOY_RIGHT_UPPER_LEG_ROLL_ANGLE:
				return 30;
			case BOY_RIGHT_FOOT_PITCH_ANGLE:
				return animationEase(progress * 2) * 10 + 100;
			case GIRL_HEAD_PITCH:
				return animationEase(progress * 3) * 22.5 + 2.5;
			case GIRL_LEFT_UPPER_LEG_PITCH_ANGLE:
			case GIRL_RIGHT_UPPER_LEG_PITCH_ANGLE:
				return 70 - 10 * animationEase(progress * 3);
			case GIRL_LEFT_UPPER_LEG_ROLL_ANGLE:
			case GIRL_RIGHT_UPPER_LEG_ROLL_ANGLE:
				return 40;
			}
		} else {
			switch (index) {
			case BOY_GIRL_BIND:
				return 1;
			case BOY_GIRL_SIDE:
				return 1;
			case BOY_GIRL_ANGLE:
				return animationEase(progress * 2) * 5 + 10;
			case GIRL_LEFT_UPPER_LEG_ROLL_ANGLE:
			case GIRL_RIGHT_UPPER_LEG_ROLL_ANGLE:
				return animationEase(progress) * 15 + 60;
			case GIRL_SHOULDER_LEFT_ANGLE_RAISE:
			case GIRL_SHOULDER_RIGHT_ANGLE_RAISE:
				return 7.5;
			case GIRL_SHOULDER_LEFT_ANGLE_FLAP:
			case GIRL_SHOULDER_RIGHT_ANGLE_FLAP:
				return animationEase(progress * 4) * 30 - 60;
			case GIRL_SHOULDER_LEFT_ANGLE_AXIAL:
			case GIRL_SHOULDER_RIGHT_ANGLE_AXIAL:
				return -90;
			case GIRL_ELBOW_LEFT_ANGLE:
			case GIRL_ELBOW_RIGHT_ANGLE:
				return 15;
			case BOY_HEAD_TILT:
				return animationEase(progress) * 15;
			case BOY_SHOULDER_ANGLE:
				return 75;
			case BOY_ELBOW_ANGLE:
				return 60;
			case BOY_LEFT_UPPER_LEG_PITCH_ANGLE:
				return animationEase(progress * 2) * 30;
			case BOY_RIGHT_UPPER_LEG_PITCH_ANGLE:
				return animationEase(progress * 2 + 0.5) * 30;
			case BOY_LEFT_LOWER_LEG_PITCH_ANGLE:
				return abs(animationEase(progress * 2)) * 30;
			case BOY_RIGHT_LOWER_LEG_PITCH_ANGLE:
				return abs(animationEase(progress * 2 + 0.5)) * 30;
			}
		}
		if (!VAL(CUSTOM_ANIMATION)) {
			return defaultValues[index];
		}
	}
	return ModelerApplication::Instance()->GetControlValue(index);
}

void SampleModel::ModelerViewDraw()
{
	//static GLfloat lightPosition0[] = { 4, 2, -4, 0 };
	//static GLfloat lightDiffuse0[] = { 1, 1, 1, 1 };
	GLfloat lightPosition0[] = { VAL(LIGHT_DIR_X), VAL(LIGHT_DIR_Y), VAL(LIGHT_DIR_Z), 0 };
	GLfloat lightDiffuse0[] = { 1, 1, 1, 1 };
	static GLfloat lightPosition1[] = { 5, 5, 5, 1 };
	static GLfloat lightDiffuse1[] = { 1, 1, 1, 1 };
	
	if (!valid())
	{
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_NORMALIZE);
	}

	glViewport(0, 0, w(), h());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, float(w()) / float(h()), 1.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_camera->applyViewingTransform();

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse1);

	//Add ambient light
	GLfloat ambientColor[] = { 0.8f, 0.8f, 0.8f, 1.0f }; //Color(0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	//================================
	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LESS);

	Image *image1 = loadTexture();

	if (image1 == NULL){

		printf("Image was not returned from loadTexture\n");

		exit(0);

	}

	makeCheckImage();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Create Texture

	glGenTextures(2, texture);

	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture

	glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0,

		GL_RGB, GL_UNSIGNED_BYTE, image1->data);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glBindTexture(GL_TEXTURE_2D, texture[1]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, checkImageWidth,

		checkImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, &checkImage[0][0][0]);

	//glEnable(GL_TEXTURE_2D);

	glShadeModel(GL_FLAT);
}

bool SampleModel::isAnimating()
{
	return ModelerApplication::Instance()->m_animating;
}


// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out SampleModel
void SampleModel::draw()
{
	// This call takes care of a lot of the nasty projection 
	// matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...

	animateCounter++;
	animateCounter = animateCounter % ANIMATION_FRAMES_COUNT;

	//ModelerView::draw();
	ModelerViewDraw();

	// draw the sample model
	setAmbientColor(.1f, .1f, .1f);

	double BSA = 180 - VAL(BOY_SHOULDER_ANGLE);
	double BEA = -VAL(BOY_ELBOW_ANGLE);

	GLCOLOR(COLOR_BOY_BODY, {
		GLMATRIX({
			glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));

			// body
			drawBoxFromBottomCenter(BOY_BODY_X, BOY_BODY_Y, BOY_BODY_Z);

			GLMATRIX({
				glTranslated(0, 0, BOY_BODY_Z);

				// head
				GLMATRIX({
					glRotated(VAL(BOY_HEAD_PITCH), 1, 0, 0);
					glRotated(-VAL(BOY_HEAD_TILT), 0, 1, 0);
					glTranslated(-BOY_HEAD_X / 2, -BOY_HEAD_Y / 2, 0);
					//Texture Mapping Begin
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, texture[0]);
					glBegin(GL_QUADS);

					glTexCoord2f(0.0, 0.0);
					glVertex3f(0, 0, 0.0);
					glTexCoord2f(1.0, 0.0);
					glVertex3f(BOY_HEAD_X, 0, 0.0);
					glTexCoord2f(1.0, 1.0);
					glVertex3f(BOY_HEAD_X, 0, BOY_HEAD_Z);
					glTexCoord2f(0.0, 1.0);
					glVertex3f(0, 0, BOY_HEAD_Z);
					glEnd();
					glDisable(GL_TEXTURE_2D);
					
					drawBox(BOY_HEAD_X, BOY_HEAD_Y, BOY_HEAD_Z);
					
				});

				// arms / hands
				for (int i = -1; i <= 1; i += 2) {
					GLMATRIX({
						glTranslated(i * BOY_BODY_X / 2, 0, 0);
						glRotated(BSA, 1, 0, 0);

						drawBoxFromBottomCenter(BOY_UPPER_ARM_RADIUS, BOY_UPPER_ARM_RADIUS, BOY_UPPER_ARM_LENGTH);

						GLMATRIX({
							glTranslated(0, 0, BOY_UPPER_ARM_LENGTH);
							glRotated(BEA, 1, 0, 0);
							glRotated(10, 0, -i, 0);

							drawBoxFromBottomCenter(BOY_LOWER_ARM_RADIUS, BOY_LOWER_ARM_RADIUS, BOY_LOWER_ARM_LENGTH);

							if (VAL(LVL_DETAIL) >= 1) {
								GLMATRIX({
									glTranslated(0, -0.125, BOY_LOWER_ARM_LENGTH);
									if (VAL(LVL_DETAIL) >= 2) {
										glRotated(- i * 90, 0, 0, 1);
										glScaled(i * HAND_SCALE_RATIO, HAND_SCALE_RATIO, HAND_SCALE_RATIO);
										glTranslated(-1.25, 0.25, 0);
										HandDrawer::draw();
									} else {
										drawBox(i * 0.125, 0.25, 0.375);
									}
								});
							}

						});

					});
				}

				if (VAL(BOY_GIRL_BIND)) {
					GLMATRIX({
						glRotated(BSA, 1, 0, 0);
						glTranslated(0, 0, BOY_UPPER_ARM_LENGTH);
						glRotated(BEA, 1, 0, 0);
						glTranslated(0, 0, BOY_LOWER_ARM_LENGTH);
						glRotated(VAL(BOY_GIRL_ANGLE) - BSA - BEA, 1, 0, 0);
						glRotated(VAL(BOY_GIRL_SIDE) * 180, 0, 0, 1);
						drawGirl();
					});
				}
			});

			// penis
			if (VAL(NSFW)) {
				GLMATRIX({
					glRotated(90, 1, 0, 0);
					drawBoxFromBottomCenter(BOY_PENIS_DIAMETER, BOY_PENIS_DIAMETER, BOY_PENIS_LENGTH);
				});
			}

			// left leg / feet
			GLMATRIX({
				glTranslated(BOY_BODY_X / 4, 0, 0);
				glRotated(VAL(BOY_LEFT_UPPER_LEG_PITCH_ANGLE), -1, 0, 0);
				glRotated(VAL(BOY_LEFT_UPPER_LEG_ROLL_ANGLE), 0, -1, 0);
				glRotated(VAL(BOY_LEFT_UPPER_LEG_YAW_ANGLE), 0, 0, 1);

				// upper leg
				drawBoxFromBottomCenter(BOY_UPPER_LEG_X, BOY_UPPER_LEG_Y, -BOY_UPPER_LEG_LENGTH);

				// lower leg
				GLMATRIX({
					glTranslated(0, 0, -BOY_UPPER_LEG_LENGTH);
					glRotated(180 + VAL(BOY_LEFT_LOWER_LEG_PITCH_ANGLE), 1, 0, 0);
					drawBoxFromBottomCenter(BOY_LOWER_LEG_RADIUS, BOY_LOWER_LEG_RADIUS, BOY_LOWER_LEG_LENGTH);

					// foot
					if (VAL(LVL_DETAIL) >= 1) {
						GLMATRIX({
							glTranslated(0, 0, BOY_LOWER_LEG_LENGTH);
							glRotated(180 - VAL(BOY_LEFT_FOOT_PITCH_ANGLE), 1, 0, 0);
							drawBoxFromBottomCenter(BOY_FOOT_WIDTH, BOY_FOOT_THICKNESS, -BOY_FOOT_LENGTH);
						});
					}

				});

			});

			// right leg / feet
			GLMATRIX({
				glTranslated(- BOY_BODY_X / 4, 0, 0);
				glRotated(VAL(BOY_RIGHT_UPPER_LEG_PITCH_ANGLE), -1, 0, 0);
				glRotated(VAL(BOY_RIGHT_UPPER_LEG_ROLL_ANGLE), 0, 1, 0);
				glRotated(VAL(BOY_RIGHT_UPPER_LEG_YAW_ANGLE), 0, 0, -1);

				// upper leg
				drawBoxFromBottomCenter(BOY_UPPER_LEG_X, BOY_UPPER_LEG_Y, -BOY_UPPER_LEG_LENGTH);

				// lower leg
				GLMATRIX({
					glTranslated(0, 0, -BOY_UPPER_LEG_LENGTH);
					glRotated(180 + VAL(BOY_RIGHT_LOWER_LEG_PITCH_ANGLE), 1, 0, 0);
					drawBoxFromBottomCenter(BOY_LOWER_LEG_RADIUS, BOY_LOWER_LEG_RADIUS, BOY_LOWER_LEG_LENGTH);
					
					// foot
					if (VAL(LVL_DETAIL) >= 1) {
						GLMATRIX({
							glTranslated(0, 0, BOY_LOWER_LEG_LENGTH);
							glRotated(180 - VAL(BOY_RIGHT_FOOT_PITCH_ANGLE), 1, 0, 0);
							drawBoxFromBottomCenter(BOY_FOOT_WIDTH, BOY_FOOT_THICKNESS, -BOY_FOOT_LENGTH);
						});
					}

				});

			});

		});
	});

	if (!VAL(BOY_GIRL_BIND)) {
		GLMATRIX({
			glTranslated(- VAL(XPOS), VAL(YPOS), VAL(ZPOS));
			glRotated(VAL(BOY_GIRL_SIDE) * 180, 0, 0, 1);
			drawGirl();
		});
}
}

void SampleModel::drawGirl() {
	GLCOLOR(COLOR_GIRL_BODY, {
		// body
		GLMATRIX({
			glScaled(1, 0.5, 1);
			drawCylinder(GIRL_BODY_LENGTH, GIRL_WAIST_RADIUS, VAL(GIRL_SHOULDER_LENGTH));
		});

		GLMATRIX({
			glTranslated(0, 0, GIRL_BODY_LENGTH);

			// head
			GLMATRIX({
				glRotated(- VAL(GIRL_HEAD_PITCH), 1, 0, 0);
				glRotated(VAL(GIRL_HEAD_TILT), 0, 1, 0);
				glTranslated(0, 0, GIRL_HEAD_RADIUS * GIRL_HEAD_LENGTH);
				glScaled(1, 1, GIRL_HEAD_LENGTH);	
				drawSphere(GIRL_HEAD_RADIUS);


				// eyes and mouth
				if (VAL(LVL_DETAIL) >= 2) {
					GLCOLOR(COLOR_EYES, {
						GLMATRIX({
							glTranslated(GIRL_EYES_COORD);
							drawSphere(GIRL_EYES_RADIUS);
						});
						GLMATRIX({
							glTranslated(-GIRL_EYES_COORD);
							drawSphere(GIRL_EYES_RADIUS);
						});
					});
					GLCOLOR(COLOR_MOUTH, {
						GLMATRIX({
							glTranslated(GIRL_MOUTH_COORD);
							glScaled(1, 0.85, 0.35);
							drawSphere(0.4);
						});
					});
				}

			});

			// breast
			if (VAL(NSFW)) {
				for (int i = -1; i <= 1; i += 2){
					GLMATRIX({
						glTranslated(i*GIRL_BREAST_RADIUS*0.75, GIRL_BREAST_RADIUS / 2, -GIRL_BODY_LENGTH*0.25);
						drawSphere(GIRL_BREAST_RADIUS);
					});
				}
			}

			// left arm
			GLMATRIX({
				glTranslated(-1 * VAL(GIRL_SHOULDER_LENGTH), 0, 0);
				glRotated(90 + VAL(GIRL_SHOULDER_LEFT_ANGLE_FLAP), 0, -1, 0);
				glRotated(VAL(GIRL_SHOULDER_LEFT_ANGLE_RAISE), -1, 0, 0);
				glRotated(180 + VAL(GIRL_SHOULDER_LEFT_ANGLE_AXIAL), 0, 0, 1);
				drawCylinder(GIRL_UPPER_ARM_LENGTH, GIRL_UPPER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);

				GLMATRIX({
					glTranslated(0, 0, GIRL_UPPER_ARM_LENGTH);
					glRotated(VAL(GIRL_ELBOW_LEFT_ANGLE), 1, 0, 0);
					drawCylinder(GIRL_LOWER_ARM_LENGTH, GIRL_LOWER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);

					if (VAL(LVL_DETAIL) >= 1) {
						GLMATRIX({
							glTranslated(0, 0, GIRL_LOWER_ARM_LENGTH);
							glRotated(90, 0, 1, 0);
							glScaled(1.25, 1, 1);
							drawCylinder(0.125, GIRL_LOWER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);
						});
					}

				});

			});

			// right arm
			GLMATRIX({
				glTranslated(1 * VAL(GIRL_SHOULDER_LENGTH), 0, 0);
				glRotated(90 + VAL(GIRL_SHOULDER_RIGHT_ANGLE_FLAP), 0, 1, 0);
				glRotated(VAL(GIRL_SHOULDER_RIGHT_ANGLE_RAISE), -1, 0, 0);
				glRotated(180 + VAL(GIRL_SHOULDER_RIGHT_ANGLE_AXIAL), 0, 0, -1);
				drawCylinder(GIRL_UPPER_ARM_LENGTH, GIRL_UPPER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);

				GLMATRIX({
					glTranslated(0, 0, GIRL_UPPER_ARM_LENGTH);
					glRotated(VAL(GIRL_ELBOW_RIGHT_ANGLE), 1, 0, 0);
					drawCylinder(GIRL_LOWER_ARM_LENGTH, GIRL_LOWER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);

					if (VAL(LVL_DETAIL) >= 1) {
						GLMATRIX({
							glTranslated(0, 0, GIRL_LOWER_ARM_LENGTH);
							glRotated(90, 0, -1, 0);
							glScaled(1.25, 1, 1);
							drawCylinder(0.125, GIRL_LOWER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);
						});
					}

				});

			});

		});

		// left leg / feet
		GLMATRIX({
			glTranslated(-GIRL_WAIST_RADIUS / 2, 0, 0);
			glRotated(VAL(GIRL_LEFT_UPPER_LEG_PITCH_ANGLE), 1, 0, 0);
			glRotated(VAL(GIRL_LEFT_UPPER_LEG_ROLL_ANGLE), 0, 1, 0);
			glRotated(VAL(GIRL_LEFT_UPPER_LEG_YAW_ANGLE), 0, 0, 1);
			//Texture Mapping begin
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture[1]);
			// upper leg
			glTranslated(0, 0, -GIRL_UPPER_LEG_LENGTH);
			drawCylinder(GIRL_UPPER_LEG_LENGTH, GIRL_LEG_RADIUS, GIRL_WAIST_RADIUS / 2);

			// lower leg
			GLMATRIX({
				glRotated(180 - VAL(GIRL_LEFT_LOWER_LEG_PITCH_ANGLE), 1, 0, 0);

				drawCylinder(GIRL_LOWER_LEG_LENGTH, GIRL_LEG_RADIUS, GIRL_LEG_RADIUS);
				//Texture Mapping end
				glDisable(GL_TEXTURE_2D);

				// foot
				if (VAL(LVL_DETAIL) >= 1) {
					GLMATRIX({
						glTranslated(0, 0, GIRL_LOWER_LEG_LENGTH);
						glRotated(90 + VAL(GIRL_LEFT_FOOT_PITCH_ANGLE), 1, 0, 0);
						glTranslated(0, GIRL_LEG_RADIUS, -GIRL_LEG_RADIUS / 2);
						glScaled(1, 2, 0.5);
						drawSphere(0.25);
					});
				}

			});

		});

		// right leg / feet
		GLMATRIX({
			glTranslated(GIRL_WAIST_RADIUS / 2, 0, 0);
			glRotated(VAL(GIRL_RIGHT_UPPER_LEG_PITCH_ANGLE), 1, 0, 0);
			glRotated(VAL(GIRL_RIGHT_UPPER_LEG_ROLL_ANGLE), 0, -1, 0);
			glRotated(VAL(GIRL_RIGHT_UPPER_LEG_YAW_ANGLE), 0, 0, -1);
			//Texture Mapping begin
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture[1]);
			// upper leg
			glTranslated(0, 0, -GIRL_UPPER_LEG_LENGTH);
			drawCylinder(GIRL_UPPER_LEG_LENGTH, GIRL_LEG_RADIUS, GIRL_WAIST_RADIUS / 2);

			// lower leg
			GLMATRIX({
				glRotated(180 - VAL(GIRL_RIGHT_LOWER_LEG_PITCH_ANGLE), 1, 0, 0);

				drawCylinder(GIRL_LOWER_LEG_LENGTH, GIRL_LEG_RADIUS, GIRL_LEG_RADIUS);
				//Texture Mapping end
				glDisable(GL_TEXTURE_2D);
				// foot
				if (VAL(LVL_DETAIL) >= 1) {
					GLMATRIX({
						glTranslated(0, 0, GIRL_LOWER_LEG_LENGTH);
						glRotated(90 + VAL(GIRL_RIGHT_FOOT_PITCH_ANGLE), 1, 0, 0);
						glTranslated(0, GIRL_LEG_RADIUS, -GIRL_LEG_RADIUS / 2);
						glScaled(1, 2, 0.5);
						drawSphere(0.25);
					});
				}

			});

		});
	});
}

int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
	ModelerControl controls[NUMCONTROLS];

	createModelerControl(DBG0, "Dbg 0", -1, 1, 0.01, 0);
	createModelerControl(DBG1, "Dbg 1", -1, 1, 0.01, 1);
	createModelerControl(DBG2, "Dbg 2", -1, 1, 0.01, 0);

	createModelerControl(XPOS, "X Position", -5, 5, 0.1, 2.5);
	createModelerControl(YPOS, "Y Position", -5, 5, 0.1, 0);
	createModelerControl(ZPOS, "Z Position", -5, 5, 0.1, 0);
	createModelerControl(LVL_DETAIL, "Level of detail", 0, 2, 1, 2);
	createModelerControl(NSFW, "NSFW", 0, 1, 1, 0);
	createModelerControl(LIGHT_DIR_X, "Light Direction X", -100, 100, 0.1f, 4);
	createModelerControl(LIGHT_DIR_Y, "Light Direction Y", -100, 100, 0.1f, -10);
	createModelerControl(LIGHT_DIR_Z, "Light Direction Z", -100, 100, 0.1f, 4);

	createModelerControl(CUSTOM_ANIMATION, "Customizable animation", 0, 1, 1, 0);

	createModelerControl(BOY_GIRL_BIND, "Bind characters", 0, 1, 1, 0);
	createModelerControl(BOY_GIRL_SIDE, "Character orientation", 0, 1, 1, 0);
	createModelerControl(BOY_GIRL_ANGLE, "Angle between them two", 0, 360, 0.1, 0);

	createModelerControl(BOY_HEAD_PITCH, "Boy - head pitch", -45, 90, 0.1, 0);
	createModelerControl(BOY_HEAD_TILT, "Boy - head tilt", -45, 45, 0.1, 0);
	createModelerControl(BOY_SHOULDER_ANGLE, "Boy - shoulder", 0, 90, 0.1, 45);
	createModelerControl(BOY_ELBOW_ANGLE, "Boy - elbow", 0, 150, 0.1, 30);
	createModelerControl(BOY_LEFT_UPPER_LEG_PITCH_ANGLE, "Boy - left leg (pitch)", -45, 90, 0.1, 0);
	createModelerControl(BOY_RIGHT_UPPER_LEG_PITCH_ANGLE, "Boy - right leg (pitch)", -45, 90, 0.1, 0);
	createModelerControl(BOY_LEFT_UPPER_LEG_ROLL_ANGLE, "Boy - left leg (roll)", -15, 75, 0.1, 0);
	createModelerControl(BOY_RIGHT_UPPER_LEG_ROLL_ANGLE, "Boy - right leg (roll)", -15, 75, 0.1, 0);
	createModelerControl(BOY_LEFT_UPPER_LEG_YAW_ANGLE, "Boy - left leg (yaw)", -30, 60, 0.1, 0);
	createModelerControl(BOY_RIGHT_UPPER_LEG_YAW_ANGLE, "Boy - right leg (yaw)", -30, 60, 0.1, 0);
	createModelerControl(BOY_LEFT_LOWER_LEG_PITCH_ANGLE, "Boy - left knee", 0, 120, 0.1, 0);
	createModelerControl(BOY_RIGHT_LOWER_LEG_PITCH_ANGLE, "Boy - right knee", 0, 120, 0.1, 0);
	createModelerControl(BOY_LEFT_FOOT_PITCH_ANGLE, "Boy - left ankle", 0, 105, 0.1, 90);
	createModelerControl(BOY_RIGHT_FOOT_PITCH_ANGLE, "Boy - right ankle", 0, 105, 0.1, 90);

	createModelerControl(GIRL_HEAD_PITCH, "Girl - head pitch", -45, 90, 0.1, 0);
	createModelerControl(GIRL_HEAD_TILT, "Girl - head tilt", -45, 45, 0.1, 0);
	createModelerControl(GIRL_SHOULDER_LENGTH, "Girl - shoulder width", 0.75, 1.25, 0.01, 1);
	createModelerControl(GIRL_SHOULDER_LEFT_ANGLE_FLAP, "Girl - left shoudler (flap)", -90, 90, 0.1, 60);
	createModelerControl(GIRL_SHOULDER_RIGHT_ANGLE_FLAP, "Girl - right shoudler (flap)", -90, 90, 0.1, 60);
	createModelerControl(GIRL_SHOULDER_LEFT_ANGLE_RAISE, "Girl - left shoudler (raise)", -180, 180, 0.1, 0);
	createModelerControl(GIRL_SHOULDER_RIGHT_ANGLE_RAISE, "Girl - right shoudler (raise)", -180, 180, 0.1, 0);
	createModelerControl(GIRL_SHOULDER_LEFT_ANGLE_AXIAL, "Girl - left shoudler (axial)", -90, 90, 0.1, 0);
	createModelerControl(GIRL_SHOULDER_RIGHT_ANGLE_AXIAL, "Girl - right shoudler (axial)", -90, 90, 0.1, 0);
	createModelerControl(GIRL_ELBOW_LEFT_ANGLE, "Girl - left elbow", 0, 150, 0.1, 30);
	createModelerControl(GIRL_ELBOW_RIGHT_ANGLE, "Girl - right elbow", 0, 150, 0.1, 30);
	createModelerControl(GIRL_LEFT_UPPER_LEG_PITCH_ANGLE, "Girl - left leg (pitch)", -45, 90, 0.1, 0);
	createModelerControl(GIRL_RIGHT_UPPER_LEG_PITCH_ANGLE, "Girl - right leg (pitch)", -45, 90, 0.1, 0);
	createModelerControl(GIRL_LEFT_UPPER_LEG_ROLL_ANGLE, "Girl - left leg (roll)", -15, 75, 0.1, 0);
	createModelerControl(GIRL_RIGHT_UPPER_LEG_ROLL_ANGLE, "Girl - right leg (roll)", -15, 75, 0.1, 0);
	createModelerControl(GIRL_LEFT_UPPER_LEG_YAW_ANGLE, "Girl - left leg (yaw)", -30, 60, 0.1, 0);
	createModelerControl(GIRL_RIGHT_UPPER_LEG_YAW_ANGLE, "Girl - right leg (yaw)", -30, 60, 0.1, 0);
	createModelerControl(GIRL_LEFT_LOWER_LEG_PITCH_ANGLE, "Girl - left knee", 0, 120, 0.1, 0);
	createModelerControl(GIRL_RIGHT_LOWER_LEG_PITCH_ANGLE, "Girl - right knee", 0, 120, 0.1, 0);
	createModelerControl(GIRL_LEFT_FOOT_PITCH_ANGLE, "Girl - left ankle", 0, 105, 0.1, 90);
	createModelerControl(GIRL_RIGHT_FOOT_PITCH_ANGLE, "Girl - right ankle", 0, 105, 0.1, 90);

	srand(25486745);
	ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);
	return ModelerApplication::Instance()->Run();
}

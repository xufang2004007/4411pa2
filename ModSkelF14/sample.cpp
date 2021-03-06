#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include <FL/gl.h>
#include "modelerglobals.h"
#include "camera.h"
#include "drawrevolve.h"
#include "metaball.h"
#include "time.h"

typedef MetaballIsNotMeatball Meta;

// those three special sources files are treated as headers
#include "glhelper.cpp"
#include "texture.cpp"
#include "handdrawer.cpp"

#define createModelerControl(index, name, minimum, maximum, stepsize, value) {\
	defaultValues[index] = value;\
	controls[index] = ModelerControl(name, minimum, maximum, stepsize, value);\
}

double defaultValues[NUMCONTROLS];

// To make a SampleModel, we inherit off of ModelerView
class SampleModel : public ModelerView
{
public:
	SampleModel(int x, int y, int w, int h, char *label)
		: ModelerView(x, y, w, h, label) {}

	void draw();
	void drawGirl();
	void drawHat();
private:
	void ModelerViewDraw();
	bool isAnimating();
	double VAL(SampleModelControls index);
	int animateCounter;
	double random();
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

double animationEase(double progress) {
	double p = fmod(progress, 1);
	return sin(progress * M_PI * 2); // FIXME: use cubic curve, a lot faster
}

double SampleModel::random() {
	return rand() / (double) RAND_MAX;
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
			case THOUSERS:
				return 0;
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

	// Add ambient light
	GLfloat ambientColor[] = { 0.75f, 0.75f, 0.75f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	Texture::init();
}

bool SampleModel::isAnimating()
{
	return ModelerApplication::Instance()->m_animating;
}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out SampleModel
void SampleModel::draw()
{
	//ModelerView::draw();
	ModelerViewDraw();

	animateCounter++;
	animateCounter = animateCounter % ANIMATION_FRAMES_COUNT;

	// draw the sample model
	setAmbientColor(.15f, .15f, .15f);

	double BSA = 180 - VAL(BOY_SHOULDER_ANGLE);
	double BEA = -VAL(BOY_ELBOW_ANGLE);

	GLDRAWWITH(COLOR_BOY_BODY, {
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

					if (VAL(FACE_TEXTURE)) {
						GLDRAWWITH(Texture::texture[Texture::FACE], {
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
						});
					}
					
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

void SampleModel::drawHat() {
	double height = VAL(HAT_HEIGHT);
	double axial[] = { 0, 0.25, 0.75, (height * 3 + 0.75) / 4, height, height };
	double normal[] = { VAL(HAT_RADIUS), 1, 1, 1, 0.5, 0 };
	
	GLDRAWWITH(Texture::texture[Texture::STRAW], { DrawRevolve::revolve(6, normal, axial); });
}

void SampleModel::drawGirl() {
	Meta* skin = NULL;
	double MBS = VAL(METABALL_ARM_BALL_SPACING);
	double MBR = VAL(METABALL_ARM_BALL_RADIUS);
	double GSL = VAL(GIRL_SHOULDER_LENGTH);
	double MBSB = VAL(METABALL_BODY_BALL_SPACING);
	double MBRB = VAL(METABALL_BODY_BALL_RADIUS);

	GLDRAWWITH(COLOR_GIRL_BODY, {
		if (VAL(GIRL_METABALL_SKIN)) {
			skin = new Meta(VAL(NUM_MARCHING_CUBES));
			skin->setBase();
			srand(19930423); // my birthday. re-srand each time, to make sure the dimensions stay the same
		}

		// body
		if (skin) {
			// bottom surface
			{
				double a = GIRL_WAIST_RADIUS - MBRB / 2 - MBSB;
				double b = a / 2;
				int jStep = ceil(M_PI * (1.5 * (GIRL_WAIST_RADIUS - MBRB / 2)) * 1.05 / MBSB) + 0.5;
				double jStepSize = 2 * M_PI / jStep;
				double offset = jStepSize / 2;
				for (int j = 0; j < jStep; j++) {
					double angle = j * jStepSize + offset;
					skin->addBallRel(a * cos(angle), b * sin(angle), MBRB / 2, MBRB);
				}
			}
			// side surface
			{
				double offset = 0;
				int step = ceil(GIRL_BODY_LENGTH / MBSB) + 0.5;
				double stepSize = GIRL_BODY_LENGTH / step;
				for (int i = 0; i <= step; i++) {
					double a = (GIRL_WAIST_RADIUS * (step - i) * (step - i) / step + GSL * i) / step - MBRB / 2;
					double b = a / 2;
					double cir = M_PI * (a + b) * 1.05; // as it's ellipse of h = 9, instead of circle
					int jStep = ceil(cir / MBSB) + 0.5;
					double jStepSize = 2 * M_PI / jStep;
					float height = stepSize * i;
					for (int j = 0; j < jStep; j++) {
						double angle = j * jStepSize + offset;
						skin->addBallRel(a * cos(angle), b * sin(angle), height, MBRB);
					}
					if (i != step) { offset = M_PI * rand(); }
				}
			}
			// top surface
			{
				double actual = GSL - MBRB / 2;
				int step = ceil(actual / MBSB) + 0.5;
				double stepSize = actual / step;
				for (int i = 0; i < step; i++) {
					double a = actual * i / step;
					double b = a / 2;
					double cir = M_PI * (a + b) * 1.05; // as it's ellipse of h = 9, instead of circle
					int jStep = ceil(cir / MBSB) + 0.5;
					double jStepSize = 2 * M_PI / jStep;
					double offset = M_PI * rand();
					for (int j = 0; j < jStep; j++) {
						double angle = j * jStepSize + offset;
						skin->addBallRel(a * cos(angle), b * sin(angle), GIRL_BODY_LENGTH - MBRB / 2, MBRB);
					}
				}
			}
		} else {
			GLMATRIX({
				glScaled(1, 0.5, 1);
				drawCylinder(GIRL_BODY_LENGTH, GIRL_WAIST_RADIUS, GSL);
			});
		}

		GLMATRIX({
			glTranslated(0, 0, GIRL_BODY_LENGTH);

			// head
			GLMATRIX({
				glRotated(- VAL(GIRL_HEAD_PITCH), 1, 0, 0);
				glRotated(VAL(GIRL_HEAD_TILT), 0, 1, 0);

				GLMATRIX({
					glTranslated(0, 0, GIRL_HEAD_RADIUS * GIRL_HEAD_LENGTH);
					glScaled(1, 1, GIRL_HEAD_LENGTH);
					drawSphere(GIRL_HEAD_RADIUS);

					// eyes and mouth
					if (VAL(LVL_DETAIL) >= 2) {
						GLDRAWWITH(COLOR_EYES, {
							GLMATRIX({
								glTranslated(GIRL_EYES_COORD);
								drawSphere(GIRL_EYES_RADIUS);
							});
							GLMATRIX({
								glTranslated(-GIRL_EYES_COORD);
								drawSphere(GIRL_EYES_RADIUS);
							});
						});
						GLDRAWWITH(COLOR_MOUTH, {
							GLMATRIX({
								glTranslated(GIRL_MOUTH_COORD);
								glScaled(1, 0.85, 0.35);
								drawSphere(0.4);
							});
						});
					}
				});

				if (VAL(HAT_SHOW)) {
					double scale = 0.75 * GIRL_HEAD_RADIUS;
					GLMATRIX({
						glTranslated(0, 0, GIRL_HEAD_RADIUS * GIRL_HEAD_LENGTH * 1.35);
						glScaled(scale, scale, scale);
						drawHat();
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

				if (skin) {
					int step = ceil(GIRL_UPPER_ARM_LENGTH / MBS) + 0.5;
					double stepSize = GIRL_UPPER_ARM_LENGTH / step;
					for (int i = 0; i <= step; i++) {
						skin->addBallRel(0, 0, i * stepSize, (GIRL_UPPER_ARM_RADIUS * (step - i) + GIRL_LOWER_ARM_RADIUS * i) / step * MBR);
					}
				} else {
					drawCylinder(GIRL_UPPER_ARM_LENGTH, GIRL_UPPER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);
				}

				GLMATRIX({
					glTranslated(0, 0, GIRL_UPPER_ARM_LENGTH);
					glRotated(VAL(GIRL_ELBOW_LEFT_ANGLE), 1, 0, 0);

					if (skin) {
						int step = ceil(GIRL_LOWER_ARM_LENGTH / MBS) + 0.5;
						double stepSize = GIRL_LOWER_ARM_LENGTH / step;
						for (int i = 0; i <= step; i++) {
							skin->addBallRel(0, 0, i * stepSize, GIRL_LOWER_ARM_RADIUS * MBR);
						}
					} else {
						drawCylinder(GIRL_LOWER_ARM_LENGTH, GIRL_LOWER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);
					}

					if (VAL(LVL_DETAIL) >= 1) {
						GLMATRIX({
							glTranslated(0, 0, GIRL_LOWER_ARM_LENGTH);
							glRotated(90, 0, 1, 0);
							glScaled(1.25, 1, 1);
							drawCylinder(GIRL_PALM_THICKNESS, GIRL_LOWER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);
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

				if (skin) {
					int step = ceil(GIRL_UPPER_ARM_LENGTH / MBS) + 0.5;
					double stepSize = GIRL_UPPER_ARM_LENGTH / step;
					for (int i = 0; i <= step; i++) {
						skin->addBallRel(0, 0, i * stepSize, (GIRL_UPPER_ARM_RADIUS * (step - i) + GIRL_LOWER_ARM_RADIUS * i) / step * MBR);
					}
				} else {
					drawCylinder(GIRL_UPPER_ARM_LENGTH, GIRL_UPPER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);
				}

				GLMATRIX({
					glTranslated(0, 0, GIRL_UPPER_ARM_LENGTH);
					glRotated(VAL(GIRL_ELBOW_RIGHT_ANGLE), 1, 0, 0);
					
					if (skin) {
						int step = ceil(GIRL_LOWER_ARM_LENGTH / MBS) + 0.5;
						double stepSize = GIRL_LOWER_ARM_LENGTH / step;
						for (int i = 0; i <= step; i++) {
							skin->addBallRel(0, 0, i * stepSize, GIRL_LOWER_ARM_RADIUS * MBR);
						}
					} else {
						drawCylinder(GIRL_LOWER_ARM_LENGTH, GIRL_LOWER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);
					}

					GLMATRIX({
						glTranslated(0, 0, GIRL_LOWER_ARM_LENGTH);
						glRotated(90, 0, -1, 0);

						if (VAL(LVL_DETAIL) >= 1) {
							GLMATRIX({
								glScaled(1.25, 1, 1);
								drawCylinder(GIRL_PALM_THICKNESS, GIRL_LOWER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);
							});
						}

						if (VAL(DONUT_SHOW)) {
							double donut_r = VAL(DONUT_MAJOR_R);
							double donut_xsec_r = donut_r * VAL(DONUT_R_RATIO);
							GLMATRIX({
								glTranslated(donut_r + donut_xsec_r, 0, -donut_xsec_r);
								GLDRAWWITH(Texture::texture[Texture::DONUT], {
									DrawRevolve::torus(donut_r, donut_xsec_r, VAL(DONUT_X_REPEAT), VAL(DONUT_Y_REPEAT));
								});
							});
						}
					});

				});

			});
		});

		// left leg / feet
		GLMATRIX({
			glTranslated(-GIRL_WAIST_RADIUS / 2, 0, 0);
			glRotated(VAL(GIRL_LEFT_UPPER_LEG_PITCH_ANGLE), 1, 0, 0);
			glRotated(VAL(GIRL_LEFT_UPPER_LEG_ROLL_ANGLE), 0, 1, 0);
			glRotated(VAL(GIRL_LEFT_UPPER_LEG_YAW_ANGLE), 0, 0, 1);

			// upper leg
			glTranslated(0, 0, -GIRL_UPPER_LEG_LENGTH);

			GLMAYBEDRAWWITH(Texture::texture[Texture::CHECK], VAL(THOUSERS) >= 1, {
				drawCylinder(GIRL_UPPER_LEG_LENGTH, GIRL_LEG_RADIUS, GIRL_WAIST_RADIUS / 2);
			});

			// lower leg
			GLMATRIX({
				glRotated(180 - VAL(GIRL_LEFT_LOWER_LEG_PITCH_ANGLE), 1, 0, 0);

				GLMAYBEDRAWWITH(Texture::texture[Texture::CHECK], VAL(THOUSERS) >= 2, {
					drawCylinder(GIRL_LOWER_LEG_LENGTH, GIRL_LEG_RADIUS, GIRL_LEG_RADIUS);
				});

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

			glTranslated(0, 0, -GIRL_UPPER_LEG_LENGTH);

			GLMAYBEDRAWWITH(Texture::texture[Texture::CHECK], VAL(THOUSERS) >= 1, {
				drawCylinder(GIRL_UPPER_LEG_LENGTH, GIRL_LEG_RADIUS, GIRL_WAIST_RADIUS / 2);
			});

			// lower leg
			GLMATRIX({
				glRotated(180 - VAL(GIRL_RIGHT_LOWER_LEG_PITCH_ANGLE), 1, 0, 0);

				GLMAYBEDRAWWITH(Texture::texture[Texture::CHECK], VAL(THOUSERS) >= 2, {
					drawCylinder(GIRL_LOWER_LEG_LENGTH, GIRL_LEG_RADIUS, GIRL_LEG_RADIUS);
				});

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

		if (skin) {
			skin->autoInitGrid();
			skin->calc();
			skin->draw(VAL(METABALL_SURFACE_THRESHOLD));
			delete skin;
		}

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
	createModelerControl(LIGHT_DIR_X, "Light Direction X", -100, 100, 0.1, 4);
	createModelerControl(LIGHT_DIR_Y, "Light Direction Y", -100, 100, 0.1, -10);
	createModelerControl(LIGHT_DIR_Z, "Light Direction Z", -100, 100, 0.1, 4);

	createModelerControl(FACE_TEXTURE, "Texture on boy's face", 0, 1, 1, 0);

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
	createModelerControl(GIRL_SHOULDER_LENGTH, "Girl - shoulder width", 0.75, 1.25, 0.01, 0.9);
	createModelerControl(GIRL_SHOULDER_LEFT_ANGLE_FLAP, "Girl - left shoudler (flap)", -90, 90, 0.1, 60);
	createModelerControl(GIRL_SHOULDER_RIGHT_ANGLE_FLAP, "Girl - right shoudler (flap)", -90, 90, 0.1, 60);
	createModelerControl(GIRL_SHOULDER_LEFT_ANGLE_RAISE, "Girl - left shoudler (raise)", -180, 180, 0.1, 0);
	createModelerControl(GIRL_SHOULDER_RIGHT_ANGLE_RAISE, "Girl - right shoudler (raise)", -180, 180, 0.1, 0);
	createModelerControl(GIRL_SHOULDER_LEFT_ANGLE_AXIAL, "Girl - left shoudler (axial)", -90, 90, 0.1, 0);
	createModelerControl(GIRL_SHOULDER_RIGHT_ANGLE_AXIAL, "Girl - right shoudler (axial)", -90, 90, 0.1, 0);
	createModelerControl(GIRL_ELBOW_LEFT_ANGLE, "Girl - left elbow", 0, 150, 0.1, 30);
	createModelerControl(GIRL_ELBOW_RIGHT_ANGLE, "Girl - right elbow", 0, 150, 0.1, 30);
	createModelerControl(THOUSERS, "Girl - thousers", 0, 2, 1, 1);
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

	createModelerControl(GIRL_METABALL_SKIN, "Metaball skinning for girl", 0, 1, 1, 0);
	createModelerControl(METABALL_SURFACE_THRESHOLD, "Metaball - surfacing threshold", 1, 100, 0.2, 65);
	createModelerControl(METABALL_BODY_BALL_RADIUS, "Metaball (body) - ball radius", 0.1, 1, 0.02, 0.3);
	createModelerControl(METABALL_BODY_BALL_SPACING, "Metaball (body) - ball spacing", 0.1, 1, 0.02, 0.3);
	createModelerControl(METABALL_ARM_BALL_RADIUS, "Metaball (arm) - ball radius magnification ratio", 0.5, 2, 0.05, 1.2);
	createModelerControl(METABALL_ARM_BALL_SPACING, "Metaball (arm) - ball spacing", 0.02, 1, 0.01, 0.05);
	createModelerControl(NUM_MARCHING_CUBES, "Metaball - marching cube grid size", 10, 200, 1, 50);

	createModelerControl(DONUT_SHOW, "Donut - visible", 0, 1, 1, 0);
	createModelerControl(DONUT_MAJOR_R, "Donut - radius", 0.05, 2, 0.05, 0.25);
	createModelerControl(DONUT_R_RATIO, "Donut - r / R", 0.1, 1, 0.05, 0.5);
	createModelerControl(DONUT_X_REPEAT, "Donut - texture repeat (vertical)", 1, 5, 1, 1);
	createModelerControl(DONUT_Y_REPEAT, "Donut - texture repeat (horizontal)", 1, 5, 1, 1);

	createModelerControl(HAT_SHOW, "Hat - visible", 0, 1, 1, 0);
	createModelerControl(HAT_RADIUS, "Hat - disc radius", 1.75, 5, 0.05, 2.5);
	createModelerControl(HAT_HEIGHT, "Hat - height", 1.25, 3.5, 0.05, 2.25);

	ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);
	return ModelerApplication::Instance()->Run();
}

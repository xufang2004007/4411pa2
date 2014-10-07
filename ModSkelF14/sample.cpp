// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include <FL/gl.h>

#include "modelerglobals.h"
#include "glhelper.h"

// To make a SampleModel, we inherit off of ModelerView
class SampleModel : public ModelerView
{
public:
	SampleModel(int x, int y, int w, int h, char *label)
		: ModelerView(x, y, w, h, label) { }

	virtual void draw();
	virtual void drawGirl();
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

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out SampleModel
void SampleModel::draw()
{
	// This call takes care of a lot of the nasty projection 
	// matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
	ModelerView::draw();

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

							GLMATRIX({
								glTranslated(0, -0.125, BOY_LOWER_ARM_LENGTH);
								drawBox(i * 0.125, 0.25, 0.375);
							});

						});

					});
				}

				GLMATRIX({
					glRotated(BSA, 1, 0, 0);
					glTranslated(0, 0, BOY_UPPER_ARM_LENGTH);
					glRotated(BEA, 1, 0, 0);
					glTranslated(0, 0, BOY_LOWER_ARM_LENGTH);
					glRotated(VAL(BOY_GIRL_ANGLE) - BSA - BEA, 1, 0, 0);
					glRotated(VAL(BOY_GIRL_SIDE) * 180, 0, 0, 1);
					drawGirl();
				});
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
					GLMATRIX({
						glTranslated(0, 0, BOY_LOWER_LEG_LENGTH);
						glRotated(180 - VAL(BOY_LEFT_FOOT_PITCH_ANGLE), 1, 0, 0);
						drawBoxFromBottomCenter(BOY_FOOT_WIDTH, BOY_FOOT_THICKNESS, -BOY_FOOT_LENGTH);
					});

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
					GLMATRIX({
						glTranslated(0, 0, BOY_LOWER_LEG_LENGTH);
						glRotated(180 - VAL(BOY_RIGHT_FOOT_PITCH_ANGLE), 1, 0, 0);
						drawBoxFromBottomCenter(BOY_FOOT_WIDTH, BOY_FOOT_THICKNESS, -BOY_FOOT_LENGTH);
					});

				});

			});

		});
	});
}

void SampleModel::drawGirl() {
	GLCOLOR(COLOR_GIRL_BODY, {
		// body
		GLMATRIX({
			glScaled(1, 0.5, 1);
			drawCylinder(GIRL_BODY_LENGTH, GIRL_WAIST_RADIUS, GIRL_SHOULDER_LENGTH);
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

				// eyes
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
				glTranslated(-1 * GIRL_SHOULDER_LENGTH, 0, 0);
				glRotated(90 + VAL(GIRL_SHOULDER_LEFT_ANGLE_FLAP), 0, -1, 0);
				glRotated(VAL(GIRL_SHOULDER_LEFT_ANGLE_RAISE), -1, 0, 0);
				glRotated(180 + VAL(GIRL_SHOULDER_LEFT_ANGLE_AXIAL), 0, 0, 1);
				drawCylinder(GIRL_UPPER_ARM_LENGTH, GIRL_UPPER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);

				GLMATRIX({
					glTranslated(0, 0, GIRL_UPPER_ARM_LENGTH);
					glRotated(VAL(GIRL_ELBOW_LEFT_ANGLE), 1, 0, 0);
					drawCylinder(GIRL_LOWER_ARM_LENGTH, GIRL_LOWER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);

					GLMATRIX({
						glTranslated(0, 0, GIRL_LOWER_ARM_LENGTH);
						glRotated(90, 0, 1, 0);
						glScaled(1.25, 1, 1);
						drawCylinder(0.125, GIRL_LOWER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);
					});

				});

			});

			// right arm
			GLMATRIX({
				glTranslated(1 * GIRL_SHOULDER_LENGTH, 0, 0);
				glRotated(90 + VAL(GIRL_SHOULDER_RIGHT_ANGLE_FLAP), 0, 1, 0);
				glRotated(VAL(GIRL_SHOULDER_RIGHT_ANGLE_RAISE), -1, 0, 0);
				glRotated(180 + VAL(GIRL_SHOULDER_RIGHT_ANGLE_AXIAL), 0, 0, -1);
				drawCylinder(GIRL_UPPER_ARM_LENGTH, GIRL_UPPER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);

				GLMATRIX({
					glTranslated(0, 0, GIRL_UPPER_ARM_LENGTH);
					glRotated(VAL(GIRL_ELBOW_RIGHT_ANGLE), 1, 0, 0);
					drawCylinder(GIRL_LOWER_ARM_LENGTH, GIRL_LOWER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);

					GLMATRIX({
						glTranslated(0, 0, GIRL_LOWER_ARM_LENGTH);
						glRotated(90, 0, -1, 0);
						glScaled(1.25, 1, 1);
						drawCylinder(0.125, GIRL_LOWER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);
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
			drawCylinder(GIRL_UPPER_LEG_LENGTH, GIRL_LEG_RADIUS, GIRL_WAIST_RADIUS / 2);

			// lower leg
			GLMATRIX({
				glRotated(180 - VAL(GIRL_LEFT_LOWER_LEG_PITCH_ANGLE), 1, 0, 0);

				drawCylinder(GIRL_LOWER_LEG_LENGTH, GIRL_LEG_RADIUS, GIRL_LEG_RADIUS);

				// foot
				GLMATRIX({
					glTranslated(0, 0, GIRL_LOWER_LEG_LENGTH);
					glRotated(90 + VAL(GIRL_LEFT_FOOT_PITCH_ANGLE), 1, 0, 0);
					glTranslated(0, GIRL_LEG_RADIUS, -GIRL_LEG_RADIUS / 2);
					glScaled(1, 2, 0.5);
					drawSphere(0.25);
				});

			});

		});

		// right leg / feet
		GLMATRIX({
			glTranslated(GIRL_WAIST_RADIUS / 2, 0, 0);
			glRotated(VAL(GIRL_RIGHT_UPPER_LEG_PITCH_ANGLE), 1, 0, 0);
			glRotated(VAL(GIRL_RIGHT_UPPER_LEG_ROLL_ANGLE), 0, -1, 0);
			glRotated(VAL(GIRL_RIGHT_UPPER_LEG_YAW_ANGLE), 0, 0, -1);

			// upper leg
			glTranslated(0, 0, -GIRL_UPPER_LEG_LENGTH);
			drawCylinder(GIRL_UPPER_LEG_LENGTH, GIRL_LEG_RADIUS, GIRL_WAIST_RADIUS / 2);

			// lower leg
			GLMATRIX({
				glRotated(180 - VAL(GIRL_RIGHT_LOWER_LEG_PITCH_ANGLE), 1, 0, 0);

				drawCylinder(GIRL_LOWER_LEG_LENGTH, GIRL_LEG_RADIUS, GIRL_LEG_RADIUS);

				// foot
				GLMATRIX({
					glTranslated(0, 0, GIRL_LOWER_LEG_LENGTH);
					glRotated(90 + VAL(GIRL_RIGHT_FOOT_PITCH_ANGLE), 1, 0, 0);
					glTranslated(0, GIRL_LEG_RADIUS, -GIRL_LEG_RADIUS / 2);
					glScaled(1, 2, 0.5);
					drawSphere(0.25);
				});

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
	controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
	controls[YPOS] = ModelerControl("Y Position", -5, 5, 0.1f, 0);
	controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
	controls[BOY_GIRL_SIDE] = ModelerControl("Character orientation", 0, 1, 1, 0);
	controls[BOY_GIRL_ANGLE] = ModelerControl("Angle between them two", 0, 360, 0.1f, 180);
	controls[NSFW] = ModelerControl("NSFW", 0, 1, 1, 0);

	controls[BOY_HEAD_PITCH] = ModelerControl("Boy - head pitch", -45, 90, 0.1f, 0);
	controls[BOY_HEAD_TILT] = ModelerControl("Boy - head tilt", -45, 45, 0.1f, 0);
	controls[BOY_SHOULDER_ANGLE] = ModelerControl("Boy - shoulder", 0, 90, 0.1f, 45);
	controls[BOY_ELBOW_ANGLE] = ModelerControl("Boy - elbow", 0, 150, 0.1f, 30);
	controls[BOY_LEFT_UPPER_LEG_PITCH_ANGLE] = ModelerControl("Boy - left leg (pitch)", -45, 90, 0.1f, 0);
	controls[BOY_RIGHT_UPPER_LEG_PITCH_ANGLE] = ModelerControl("Boy - right leg (pitch)", -45, 90, 0.1f, 0);
	controls[BOY_LEFT_UPPER_LEG_ROLL_ANGLE] = ModelerControl("Boy - left leg (roll)", -15, 45, 0.1f, 0);
	controls[BOY_RIGHT_UPPER_LEG_ROLL_ANGLE] = ModelerControl("Boy - right leg (roll)", -15, 45, 0.1f, 0);
	controls[BOY_LEFT_UPPER_LEG_YAW_ANGLE] = ModelerControl("Boy - left leg (yaw)", -30, 60, 0.1f, 0);
	controls[BOY_RIGHT_UPPER_LEG_YAW_ANGLE] = ModelerControl("Boy - right leg (yaw)", -30, 60, 0.1f, 0);
	controls[BOY_LEFT_LOWER_LEG_PITCH_ANGLE] = ModelerControl("Boy - left knee", 0, 120, 0.1f, 0);
	controls[BOY_RIGHT_LOWER_LEG_PITCH_ANGLE] = ModelerControl("Boy - right knee", 0, 120, 0.1f, 0);
	controls[BOY_LEFT_FOOT_PITCH_ANGLE] = ModelerControl("Boy - left ankle", 0, 105, 0.1f, 90);
	controls[BOY_RIGHT_FOOT_PITCH_ANGLE] = ModelerControl("Boy - right ankle", 0, 105, 0.1f, 90);

	controls[GIRL_HEAD_PITCH] = ModelerControl("Girl - head pitch", -45, 90, 0.1f, 0);
	controls[GIRL_HEAD_TILT] = ModelerControl("Girl - head tilt", -45, 45, 0.1f, 0);
	controls[GIRL_SHOULDER_LEFT_ANGLE_FLAP] = ModelerControl("Girl - left shoudler (flap)", -90, 90, 0.1f, 60);
	controls[GIRL_SHOULDER_RIGHT_ANGLE_FLAP] = ModelerControl("Girl - right shoudler (flap)", -90, 90, 0.1f, 60);
	controls[GIRL_SHOULDER_LEFT_ANGLE_RAISE] = ModelerControl("Girl - left shoudler (raise)", -180, 180, 0.1f, 0);
	controls[GIRL_SHOULDER_RIGHT_ANGLE_RAISE] = ModelerControl("Girl - right shoudler (raise)", -180, 180, 0.1f, 0);
	controls[GIRL_SHOULDER_LEFT_ANGLE_AXIAL] = ModelerControl("Girl - left shoudler (axial)", -90, 90, 0.1f, 0);
	controls[GIRL_SHOULDER_RIGHT_ANGLE_AXIAL] = ModelerControl("Girl - right shoudler (axial)", -90, 90, 0.1f, 0);
	controls[GIRL_ELBOW_LEFT_ANGLE] = ModelerControl("Girl - left elbow", 0, 150, 0.1f, 30);
	controls[GIRL_ELBOW_RIGHT_ANGLE] = ModelerControl("Girl - right elbow", 0, 150, 0.1f, 30);
	controls[GIRL_LEFT_UPPER_LEG_PITCH_ANGLE] = ModelerControl("Girl - left leg (pitch)", -45, 90, 0.1f, 0);
	controls[GIRL_RIGHT_UPPER_LEG_PITCH_ANGLE] = ModelerControl("Girl - right leg (pitch)", -45, 90, 0.1f, 0);
	controls[GIRL_LEFT_UPPER_LEG_ROLL_ANGLE] = ModelerControl("Girl - left leg (roll)", -15, 45, 0.1f, 0);
	controls[GIRL_RIGHT_UPPER_LEG_ROLL_ANGLE] = ModelerControl("Girl - right leg (roll)", -15, 45, 0.1f, 0);
	controls[GIRL_LEFT_UPPER_LEG_YAW_ANGLE] = ModelerControl("Girl - left leg (yaw)", -30, 60, 0.1f, 0);
	controls[GIRL_RIGHT_UPPER_LEG_YAW_ANGLE] = ModelerControl("Girl - right leg (yaw)", -30, 60, 0.1f, 0);
	controls[GIRL_LEFT_LOWER_LEG_PITCH_ANGLE] = ModelerControl("Girl - left knee", 0, 120, 0.1f, 0);
	controls[GIRL_RIGHT_LOWER_LEG_PITCH_ANGLE] = ModelerControl("Girl - right knee", 0, 120, 0.1f, 0);
	controls[GIRL_LEFT_FOOT_PITCH_ANGLE] = ModelerControl("Girl - left ankle", 0, 105, 0.1f, 90);
	controls[GIRL_RIGHT_FOOT_PITCH_ANGLE] = ModelerControl("Girl - right ankle", 0, 105, 0.1f, 90);

	ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);
	return ModelerApplication::Instance()->Run();
}

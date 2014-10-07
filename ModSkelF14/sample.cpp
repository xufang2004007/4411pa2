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
			GLMATRIX({
				glTranslated(-BOY_BODY_X / 2, -BOY_BODY_Y / 2, 0);
				drawBox(BOY_BODY_X, BOY_BODY_Y, BOY_BODY_Z);
			});

			GLMATRIX({
				glTranslated(0, 0, BOY_BODY_Z);

				// head
				GLMATRIX({
					glRotated(VAL(BOY_HEAD_PITCH), 1, 0, 0);
					glRotated(VAL(BOY_HEAD_TILT), 0, 1, 0);
					drawBox(BOY_HEAD_X / 2, BOY_HEAD_Y / 2, BOY_HEAD_Z);
					drawBox(-BOY_HEAD_X / 2, BOY_HEAD_Y / 2, BOY_HEAD_Z);
					drawBox(BOY_HEAD_X / 2, -BOY_HEAD_Y / 2, BOY_HEAD_Z);
					drawBox(-BOY_HEAD_X / 2, -BOY_HEAD_Y / 2, BOY_HEAD_Z);
				});

				// arms / hands
				for (int i = -1; i <= 1; i += 2) {
					GLMATRIX({
						glTranslated(i * BOY_BODY_X / 2, 0, 0);
						glRotated(BSA, 1, 0, 0);

						GLMATRIX({
							glTranslated(-BOY_UPPER_ARM_RADIUS / 2, -BOY_UPPER_ARM_RADIUS / 2, 0);
							drawBox(BOY_UPPER_ARM_RADIUS, BOY_UPPER_ARM_RADIUS, BOY_UPPER_ARM_LENGTH);
						});

						GLMATRIX({
							glTranslated(0, 0, BOY_UPPER_ARM_LENGTH);
							glRotated(BEA, 1, 0, 0);
							glRotated(10, 0, -i, 0);

							GLMATRIX({
								glTranslated(-BOY_LOWER_ARM_RADIUS / 2, -BOY_LOWER_ARM_RADIUS / 2, 0);
								drawBox(BOY_LOWER_ARM_RADIUS, BOY_LOWER_ARM_RADIUS, BOY_LOWER_ARM_LENGTH);
							});

							GLMATRIX({
								glTranslated(0, -0.125, BOY_LOWER_ARM_LENGTH);
								drawBox(i * 0.125, 0.25, 0.375);
							});

						});

					});
				}

				// left leg / feet
				GLMATRIX({
					glTranslated(-BOY_BODY_X / 2, 0, (-BOY_LEG_LENGTH - BOY_BODY_Z) / 2);
					glRotated(BOY_LEFT_UPPER_LEG_PITCH_ANGLE, 1, 0, 0);
					glRotated(BOY_LEFT_UPPER_LEG_RAW_ANGLE, 0, 1, 0);
					glRotated(BOY_LEFT_UPPER_LEG_YAW_ANGLE, 0, 0, 1);
					glTranslated(0, 0, (-BOY_LEG_LENGTH - BOY_BODY_Z) / 2);

					// upper leg
					GLMATRIX({
						drawBox(BOY_BODY_X / 2, BOY_BODY_Y, BOY_LEG_LENGTH);
					});

					// lower leg
					GLMATRIX({
						glRotated(BOY_LEFT_LOWER_LEG_PITCH_ANGLE, 1, 0, 0);
						glTranslated(0, 0, -BOY_LEG_LENGTH);

						drawBox(BOY_BODY_X / 2, BOY_BODY_Y, BOY_LEG_LENGTH);

						// foot
						GLMATRIX({
							glRotated(BOY_LEFT_FOOT_PITCH_ANGLE, 1, 0, 0);
							glTranslated(0, BOY_BODY_X / 2, -BOY_BODY_X / 4);
							glScaled(1, 2, 0.5);
							drawBox(BOY_BODY_X / 2, BOY_BODY_X / 2, BOY_BODY_X / 2);
						});

					});

				});

				// right leg / feet
				GLMATRIX({
					glTranslated(0, 0, (-BOY_LEG_LENGTH - BOY_BODY_Z) / 2);
					glRotated(BOY_RIGHT_UPPER_LEG_PITCH_ANGLE, 1, 0, 0);
					glRotated(BOY_RIGHT_UPPER_LEG_RAW_ANGLE, 0, 1, 0);
					glRotated(BOY_RIGHT_UPPER_LEG_YAW_ANGLE, 0, 0, 1);
					glTranslated(0, 0, (-BOY_LEG_LENGTH - BOY_BODY_Z) / 2);

					// upper leg
					GLMATRIX({
						drawBox(BOY_BODY_X / 2, BOY_BODY_Y, BOY_LEG_LENGTH);
					});

					// lower leg
					GLMATRIX({
						glRotated(BOY_RIGHT_LOWER_LEG_PITCH_ANGLE, 1, 0, 0);
						glTranslated(0, 0, -BOY_LEG_LENGTH);

						drawBox(BOY_BODY_X / 2, BOY_BODY_Y, BOY_LEG_LENGTH);

						// foot
						GLMATRIX({
							glRotated(BOY_RIGHT_FOOT_PITCH_ANGLE, 1, 0, 0);
							glTranslated(0, BOY_BODY_X / 2, -BOY_BODY_X / 4);
							glScaled(1, 2, 0.5);
							drawBox(BOY_BODY_X / 2, BOY_BODY_X / 2, BOY_BODY_X / 2);
						});

					});

				});

				GLMATRIX({
					glRotated(BSA, 1, 0, 0);
					glTranslated(0, 0, BOY_UPPER_ARM_LENGTH);
					glRotated(BEA, 1, 0, 0);
					glTranslated(0, 0, BOY_LOWER_ARM_LENGTH);
					glRotated(VAL(BOY_GIRL_ANGLE) - BSA - BEA, 1, 0, 0);
					drawGirl();
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
				glRotated(GIRL_HEAD_PITCH_ANGLE, 1, 0, 0);
				glRotated(GIRL_HEAD_TILT_ANGLE, 0, 1, 0);
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
			for (int i = -1; i <= 1; i += 2){
				GLMATRIX({
					glTranslated(i*GIRL_BREAST_RADIUS*0.75, GIRL_BREAST_RADIUS / 2, -GIRL_BODY_LENGTH*0.25);
					drawSphere(GIRL_BREAST_RADIUS);
				});
			}

			// left arm
			GLMATRIX({
				glTranslated(-1 * GIRL_SHOULDER_LENGTH, 0, 0);
				glRotated(GIRL_SHOULDER_LEFT_ANGLE_FLAP, 0, 1, 0);
				glRotated(GIRL_SHOULDER_LEFT_ANGLE_RAISE, 1, 0, 0);
				glRotated(GIRL_SHOULDER_LEFT_ANGLE_AXIAL, 0, 0, 1);
				drawCylinder(GIRL_UPPER_ARM_LENGTH, GIRL_UPPER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);

				GLMATRIX({
					glTranslated(0, 0, GIRL_UPPER_ARM_LENGTH);
					glRotated(GIRL_ELBOW_LEFT_ANGLE, 1, 0, 0);
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
				glRotated(GIRL_SHOULDER_RIGHT_ANGLE_FLAP, 0, 1, 0);
				glRotated(GIRL_SHOULDER_RIGHT_ANGLE_RAISE, 1, 0, 0);
				glRotated(GIRL_SHOULDER_RIGHT_ANGLE_AXIAL, 0, 0, 1);
				drawCylinder(GIRL_UPPER_ARM_LENGTH, GIRL_UPPER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);

				GLMATRIX({
					glTranslated(0, 0, GIRL_UPPER_ARM_LENGTH);
					glRotated(GIRL_ELBOW_RIGHT_ANGLE, 1, 0, 0);
					drawCylinder(GIRL_LOWER_ARM_LENGTH, GIRL_LOWER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);

					GLMATRIX({
						glTranslated(0, 0, GIRL_LOWER_ARM_LENGTH);
						glRotated(90, 0, -1, 0);
						glScaled(1.25, 1, 1);
						drawCylinder(0.125, GIRL_LOWER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);
					});

				});

			});

			// left leg / feet
			GLMATRIX({
				glTranslated(-GIRL_WAIST_RADIUS / 2, 0, (-GIRL_LEG_LENGTH - GIRL_BODY_LENGTH) / 2);
				glRotated(GIRL_LEFT_UPPER_LEG_PITCH_ANGLE, 1, 0, 0);
				glRotated(GIRL_LEFT_UPPER_LEG_RAW_ANGLE, 0, 1, 0);
				glRotated(GIRL_LEFT_UPPER_LEG_YAW_ANGLE, 0, 0, 1);
				glTranslated(0, 0, (-GIRL_LEG_LENGTH - GIRL_BODY_LENGTH) / 2);

				// upper leg
				GLMATRIX({
					drawCylinder(GIRL_LEG_LENGTH, GIRL_LEG_RADIUS, GIRL_WAIST_RADIUS / 2);
				});

				// lower leg
				GLMATRIX({
					glRotated(GIRL_LEFT_LOWER_LEG_PITCH_ANGLE, 1, 0, 0);
					glTranslated(0, 0, -GIRL_LEG_LENGTH);

					GLMATRIX({
						drawCylinder(GIRL_LEG_LENGTH, GIRL_LEG_RADIUS, GIRL_LEG_RADIUS);
					});

					// foot
					GLMATRIX({
						glRotated(GIRL_LEFT_FOOT_PITCH_ANGLE, 1, 0, 0);
						glTranslated(0, GIRL_LEG_RADIUS, -GIRL_LEG_RADIUS / 2);
						glScaled(1, 2, 0.5);
						drawSphere(0.25);
					});

				});

			});

			// right leg / feet
			GLMATRIX({
				glTranslated(GIRL_WAIST_RADIUS / 2, 0, (-GIRL_LEG_LENGTH - GIRL_BODY_LENGTH) / 2);
				glRotated(GIRL_RIGHT_UPPER_LEG_PITCH_ANGLE, 1, 0, 0);
				glRotated(GIRL_RIGHT_UPPER_LEG_RAW_ANGLE, 0, 1, 0);
				glRotated(GIRL_RIGHT_UPPER_LEG_YAW_ANGLE, 0, 0, 1);
				glTranslated(0, 0, (-GIRL_LEG_LENGTH - GIRL_BODY_LENGTH) / 2);

				// upper leg
				GLMATRIX({
					drawCylinder(GIRL_LEG_LENGTH, GIRL_LEG_RADIUS, GIRL_WAIST_RADIUS / 2);
				});

				// lower leg
				GLMATRIX({
					glRotated(GIRL_RIGHT_LOWER_LEG_PITCH_ANGLE, 1, 0, 0);
					glTranslated(0, 0, -GIRL_LEG_LENGTH);

					GLMATRIX({
						drawCylinder(GIRL_LEG_LENGTH, GIRL_LEG_RADIUS, GIRL_LEG_RADIUS);
					});

					// foot
					GLMATRIX({
						glRotated(GIRL_RIGHT_FOOT_PITCH_ANGLE, 1, 0, 0);
						glTranslated(0, GIRL_LEG_RADIUS, -GIRL_LEG_RADIUS / 2);
						glScaled(1, 2, 0.5);
						drawSphere(0.25);
					});

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
	controls[BOY_GIRL_BIND] = ModelerControl("Bind characters", 0, 1, 1, 1);
	controls[BOY_GIRL_SIDE] = ModelerControl("Characters on same orientation", 0, 1, 1, 0);
	controls[BOY_GIRL_ANGLE] = ModelerControl("Angle between them two", 0, 360, 0.1f, 180);
	controls[BOY_HEAD_PITCH] = ModelerControl("Boy - head pitch", -45, 90, 0.1f, 0);
	controls[BOY_HEAD_TILT] = ModelerControl("Boy - head tilt", -45, 45, 0.1f, 0);
	controls[BOY_SHOULDER_ANGLE] = ModelerControl("Boy - shoulder", 0, 90, 0.1f, 45);
	controls[BOY_ELBOW_ANGLE] = ModelerControl("Boy - elbow", 0, 150, 0.1f, 30);

	ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);
	return ModelerApplication::Instance()->Run();
}

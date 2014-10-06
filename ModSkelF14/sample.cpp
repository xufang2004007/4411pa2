// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include <FL/gl.h>

#include "modelerglobals.h"

// To make a SampleModel, we inherit off of ModelerView
class SampleModel : public ModelerView 
{
public:
    SampleModel(int x, int y, int w, int h, char *label) 
        : ModelerView(x,y,w,h,label) { }

    virtual void draw();
	virtual void drawGirl();
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createSampleModel(int x, int y, int w, int h, char *label)
{ 
    return new SampleModel(x,y,w,h,label); 
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
	setAmbientColor(.1f,.1f,.1f);

	// boy base

	setDiffuseColor(COLOR_BOY_BODY);
	glPushMatrix();
	glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
	
		// body
		glPushMatrix();
		glTranslated(-BOY_BODY_X / 2, -BOY_BODY_Y / 2, 0);
		drawBox(BOY_BODY_X, BOY_BODY_Y, BOY_BODY_Z);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, 0, BOY_BODY_Z);

			// head
			glPushMatrix();
			glTranslated(-BOY_HEAD_X / 2, -BOY_HEAD_Y / 2, 0);
			glRotated(BOY_HEAD_PITCH_ANGLE, 1, 0, 0);
			glRotated(BOY_HEAD_TILT_ANGLE, 0, 1, 0);
			drawBox(BOY_HEAD_X, BOY_HEAD_Y, BOY_HEAD_Z);
			glPopMatrix();

			// arms / hands
			for (int i = -1; i <= 1; i += 2) {
				glPushMatrix();
				glTranslated(i * BOY_BODY_X / 2, 0, 0);
				glRotated(BOY_SHOULDER_ANGLE, 1, 0, 0);

					glPushMatrix();
					glTranslated(- BOY_UPPER_ARM_RADIUS / 2, - BOY_UPPER_ARM_RADIUS / 2, 0);
					drawBox(BOY_UPPER_ARM_RADIUS, BOY_UPPER_ARM_RADIUS, BOY_UPPER_ARM_LENGTH);
					glPopMatrix();

					glPushMatrix();
					glTranslated(0, 0, BOY_UPPER_ARM_LENGTH);
					glRotated(BOY_ELBOW_ANGLE, 1, 0, 0);
					glTranslated(- BOY_LOWER_ARM_RADIUS / 2, - BOY_LOWER_ARM_RADIUS / 2, 0);
					drawBox(BOY_LOWER_ARM_RADIUS, BOY_LOWER_ARM_RADIUS, BOY_LOWER_ARM_LENGTH);
					glPopMatrix();

				glPopMatrix();
			}

			// left leg / feet
			glPushMatrix();
			glTranslated(-BOY_BODY_X / 2, 0, (-BOY_LEG_LENGTH - BOY_BODY_Z) / 2);
			glRotated(BOY_LEFT_UPPER_LEG_PITCH_ANGLE, 1, 0, 0);
			glRotated(BOY_LEFT_UPPER_LEG_RAW_ANGLE, 0, 1, 0);
			glRotated(BOY_LEFT_UPPER_LEG_YAW_ANGLE, 0, 0, 1);
			glTranslated(0, 0, (-BOY_LEG_LENGTH - BOY_BODY_Z) / 2);

				// upper leg
				glPushMatrix();
				drawBox(BOY_BODY_X / 2, BOY_BODY_Y, BOY_LEG_LENGTH);
				glPopMatrix();

				// lower leg
				glPushMatrix();
				glRotated(BOY_LEFT_LOWER_LEG_PITCH_ANGLE, 1, 0, 0);
				glTranslated(0, 0, -BOY_LEG_LENGTH);
					
					glPushMatrix();
					drawBox(BOY_BODY_X / 2, BOY_BODY_Y, BOY_LEG_LENGTH);
					glPopMatrix();

					//foot
					glPushMatrix();
					glRotated(BOY_LEFT_FOOT_PITCH_ANGLE, 1, 0, 0);
					glTranslated(0, BOY_BODY_X/2, - BOY_BODY_X / 4);
					glScaled(1, 2, 0.5);
					drawBox(BOY_BODY_X / 2, BOY_BODY_X / 2, BOY_BODY_X / 2);
					glPopMatrix();

				glPopMatrix();

			glPopMatrix();

			// right leg / feet
			glPushMatrix();
			glTranslated(0, 0, (-BOY_LEG_LENGTH - BOY_BODY_Z) / 2);
			glRotated(BOY_RIGHT_UPPER_LEG_PITCH_ANGLE, 1, 0, 0);
			glRotated(BOY_RIGHT_UPPER_LEG_RAW_ANGLE, 0, 1, 0);
			glRotated(BOY_RIGHT_UPPER_LEG_YAW_ANGLE, 0, 0, 1);
			glTranslated(0, 0, (-BOY_LEG_LENGTH - BOY_BODY_Z) / 2);

				// upper leg
				glPushMatrix();
				drawBox(BOY_BODY_X / 2, BOY_BODY_Y, BOY_LEG_LENGTH);
				glPopMatrix();

				// lower leg
				glPushMatrix();
				glRotated(BOY_RIGHT_LOWER_LEG_PITCH_ANGLE, 1, 0, 0);
				glTranslated(0, 0, -BOY_LEG_LENGTH);
					
					glPushMatrix();
					drawBox(BOY_BODY_X / 2, BOY_BODY_Y, BOY_LEG_LENGTH);
					glPopMatrix();

					//foot
					glPushMatrix();
					glRotated(BOY_RIGHT_FOOT_PITCH_ANGLE, 1, 0, 0);
					glTranslated(0, BOY_BODY_X/2, - BOY_BODY_X / 4);
					glScaled(1, 2, 0.5);
					drawBox(BOY_BODY_X / 2, BOY_BODY_X / 2, BOY_BODY_X / 2);
					glPopMatrix();

				glPopMatrix();

			glPopMatrix();

		glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	glTranslated(3, 0, 0);
	drawGirl();
	glPopMatrix();
}

void SampleModel::drawGirl() {
	setDiffuseColor(COLOR_GIRL_BODY);

		// body
		glPushMatrix();
		glScaled(1, 0.5, 1);
		drawCylinder(GIRL_BODY_LENGTH, GIRL_WAIST_RADIUS, GIRL_SHOULDER_LENGTH);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, 0, GIRL_BODY_LENGTH);

			// head
			glPushMatrix();
			glRotated(GIRL_HEAD_PITCH_ANGLE, 1, 0, 0);
			glRotated(GIRL_HEAD_TILT_ANGLE, 0, 1, 0);
			glTranslated(0, 0, HEAD_RADIUS * HEAD_LENGTH);
			glScaled(1, 1, HEAD_LENGTH);
			drawSphere(HEAD_RADIUS);

			// eyes
			setDiffuseColor(COLOR_EYES);

				glPushMatrix();
				glTranslated(EYES_COORD);
				drawSphere(EYES_RADIUS);
				glPopMatrix();

				glPushMatrix();
				glTranslated(-EYES_COORD);
				drawSphere(EYES_RADIUS);
				glPopMatrix();

			setDiffuseColor(COLOR_GIRL_BODY);

			glPopMatrix();
			
			// breast
			for (int i = -1; i <= 1; i += 2){
				glPushMatrix();
				glTranslated(i*GIRL_BREAST_RADIUS*0.75, GIRL_BREAST_RADIUS / 2, -GIRL_BODY_LENGTH*0.25);
				drawSphere(GIRL_BREAST_RADIUS);
				glPopMatrix();
			}

			// arms / hands
			for (int i = -1; i <= 1; i += 2) {
				glPushMatrix();
				glTranslated(i * GIRL_SHOULDER_LENGTH, 0, 0);
				glRotated(GIRL_SHOULDER_ANGLE, 1, 0, 0);
				drawCylinder(GIRL_UPPER_ARM_LENGTH, GIRL_UPPER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);
					
					glPushMatrix();
					glTranslated(0, 0, GIRL_UPPER_ARM_LENGTH);
					glRotated(GIRL_ELBOW_ANGLE, 1, 0, 0);
					drawCylinder(GIRL_LOWER_ARM_LENGTH, GIRL_LOWER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);

						glPushMatrix();
						glTranslated(0, 0, GIRL_LOWER_ARM_LENGTH);
						glRotated(90, 0, -i, 0);
						glScaled(1.25, 1, 1);
						drawCylinder(0.125, GIRL_LOWER_ARM_RADIUS, GIRL_LOWER_ARM_RADIUS);
						glPopMatrix();

					glPopMatrix();

				glPopMatrix();
			}

			// left leg / feet
			glPushMatrix();
			glTranslated(-GIRL_WAIST_RADIUS / 2, 0, (-GIRL_LEG_LENGTH - GIRL_BODY_LENGTH)/2);
			glRotated(GIRL_LEFT_UPPER_LEG_PITCH_ANGLE, 1, 0, 0);
			glRotated(GIRL_LEFT_UPPER_LEG_RAW_ANGLE, 0, 1, 0);
			glRotated(GIRL_LEFT_UPPER_LEG_YAW_ANGLE, 0, 0, 1);
			glTranslated(0, 0, (-GIRL_LEG_LENGTH - GIRL_BODY_LENGTH) / 2);

				// upper leg
				glPushMatrix();
				drawCylinder(GIRL_LEG_LENGTH, GIRL_LEG_RADIUS, GIRL_WAIST_RADIUS / 2);
				glPopMatrix();

				// lower leg
				glPushMatrix();
				glRotated(GIRL_LEFT_LOWER_LEG_PITCH_ANGLE, 1, 0, 0);
				glTranslated(0, 0, -GIRL_LEG_LENGTH);
					
					glPushMatrix();
					drawCylinder(GIRL_LEG_LENGTH, GIRL_LEG_RADIUS, GIRL_LEG_RADIUS);
					glPopMatrix();

					//foot
					glPushMatrix();
					glRotated(GIRL_LEFT_FOOT_PITCH_ANGLE, 1, 0, 0);
					glTranslated(0, GIRL_LEG_RADIUS, - GIRL_LEG_RADIUS / 2);
					glScaled(1, 2, 0.5);
					drawSphere(0.25);
					glPopMatrix();

				glPopMatrix();

			glPopMatrix();

			// right leg / feet
			glPushMatrix();
			glTranslated(GIRL_WAIST_RADIUS / 2, 0, (-GIRL_LEG_LENGTH - GIRL_BODY_LENGTH) / 2);
			glRotated(GIRL_RIGHT_UPPER_LEG_PITCH_ANGLE, 1, 0, 0);
			glRotated(GIRL_RIGHT_UPPER_LEG_RAW_ANGLE, 0, 1, 0);
			glRotated(GIRL_RIGHT_UPPER_LEG_YAW_ANGLE, 0, 0, 1);
			glTranslated(0, 0, (-GIRL_LEG_LENGTH - GIRL_BODY_LENGTH) / 2);

				// upper leg
				glPushMatrix();
				drawCylinder(GIRL_LEG_LENGTH, GIRL_LEG_RADIUS, GIRL_WAIST_RADIUS / 2);
				glPopMatrix();

				// lower leg
				glPushMatrix();
				glRotated(GIRL_RIGHT_LOWER_LEG_PITCH_ANGLE, 1, 0, 0);
				glTranslated(0, 0, -GIRL_LEG_LENGTH);

					glPushMatrix();
					drawCylinder(GIRL_LEG_LENGTH, GIRL_LEG_RADIUS, GIRL_LEG_RADIUS);
					glPopMatrix();

					// foot
					glPushMatrix();
					glRotated(GIRL_RIGHT_FOOT_PITCH_ANGLE, 1, 0, 0);
					glTranslated(0, GIRL_LEG_RADIUS, -GIRL_LEG_RADIUS / 2);
					glScaled(1, 2, 0.5);
					drawSphere(0.25);
					glPopMatrix();

				glPopMatrix();

			glPopMatrix();

		glPopMatrix();
}

int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
    ModelerControl controls[NUMCONTROLS];
    controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
    controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
    controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);

    ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);
    return ModelerApplication::Instance()->Run();
}

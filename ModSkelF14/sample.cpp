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
	setDiffuseColor(COLOR_BODY);
	glPushMatrix();
	glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));

		// body
		glPushMatrix();
		glScaled(1, 0.5, 1);
		drawCylinder(BODY_LENGTH, 0.75, 1);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, 0, BODY_LENGTH);

			// head
			glPushMatrix();
			glRotated(HEAD_PITCH_ANGLE, 1, 0, 0);
			glRotated(HEAD_TILT_ANGLE, 0, 1, 0);
			glTranslated(0, 0, HEAD_RADIUS * HEAD_LENGTH);
			glScaled(1, 1, HEAD_LENGTH);
			drawSphere(HEAD_RADIUS);

				// eyes
				glPushMatrix();
				glTranslated(EYES_COORD);
				drawSphere(EYES_RADIUS);
				glPopMatrix();

			glPopMatrix();
		
			// arms / hands
			for (int i = -1; i <= 1; i += 2) {
				glPushMatrix();
				glTranslated(i, 0, 0);
				glRotated(135, 1, 0, 0);
				drawCylinder(UPPER_ARM_LENGTH, 0.25, 0.25);
					
					glPushMatrix();
					glTranslated(0, 0, UPPER_ARM_LENGTH);
					glRotated(ELBOW_ANGLE, 1, 0, 0);
					drawCylinder(LOWER_ARM_LENGTH, 0.2, 0.2);
					glPopMatrix();

				glPopMatrix();
			}

			// legs / feet

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

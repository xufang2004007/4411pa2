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
	setDiffuseColor(COLOR_GREEN);
	glPushMatrix();
	glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));

		// body
		glPushMatrix();
		glScaled(1, 0.5, 1);
		drawCylinder(BODY_LENGTH, WAIST_RADIUS, SHOULDER_LENGTH);
		glPopMatrix();

		
		glPushMatrix();
		glTranslated(0, 0, BODY_LENGTH);

			// head
			glPushMatrix();
			glTranslated(0, 0, HEAD_RADIUS * HEAD_LENGTH);
			glScaled(1, 1, HEAD_LENGTH);
			drawSphere(HEAD_RADIUS);
			glPopMatrix();
		
			// arms / hands
			glPushMatrix();
			glTranslated(1, 0, 0);
			glRotated(135, 0, 1, 0);
			drawCylinder(1.25, 0.25, 0.25);
			glPopMatrix();

			// leg
			glPushMatrix();
			glTranslated(0, 0, -LEG_LENGTH-BODY_LENGTH);
			    
				// right upper leg
				glPushMatrix();
				glTranslated(-WAIST_RADIUS/2, 0, 0);
				drawCylinder(LEG_LENGTH, LEG_RADIUS, WAIST_RADIUS / 2);
				glPopMatrix();

				// right upper leg
				glPushMatrix();
				glTranslated(WAIST_RADIUS/2, 0, 0);
				drawCylinder(LEG_LENGTH, LEG_RADIUS, WAIST_RADIUS / 2);
				glPopMatrix();

				// right lower leg
				glPushMatrix();
				glTranslated(-WAIST_RADIUS/2, 0, -LEG_LENGTH);
				drawCylinder(LEG_LENGTH, LEG_RADIUS, LEG_RADIUS);
				glPopMatrix();

				// right lower leg
				glPushMatrix();
				glTranslated(WAIST_RADIUS/2, 0, -LEG_LENGTH);
				drawCylinder(LEG_LENGTH, LEG_RADIUS, LEG_RADIUS);
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

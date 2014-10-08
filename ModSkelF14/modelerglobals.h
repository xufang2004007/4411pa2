#ifndef _MODELER_GLOBALS_H
#define _MODELER_GLOBALS_H

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502
#endif

// Controls

enum SampleModelControls { 
	DBG0, DBG1, DBG2,
	XPOS, YPOS, ZPOS, LVL_DETAIL, NSFW,
	LIGHT_DIR_X, LIGHT_DIR_Y, LIGHT_DIR_Z, FACE_TEXTURE,
	CUSTOM_ANIMATION, // special marker, do NOT move. all controls of angles must be below, and anything else must be above
	BOY_GIRL_BIND, BOY_GIRL_SIDE, BOY_GIRL_ANGLE,
	BOY_HEAD_PITCH, BOY_HEAD_TILT, BOY_SHOULDER_ANGLE, BOY_ELBOW_ANGLE,
	BOY_LEFT_UPPER_LEG_PITCH_ANGLE, BOY_RIGHT_UPPER_LEG_PITCH_ANGLE, BOY_LEFT_UPPER_LEG_ROLL_ANGLE, BOY_RIGHT_UPPER_LEG_ROLL_ANGLE, BOY_LEFT_UPPER_LEG_YAW_ANGLE, BOY_RIGHT_UPPER_LEG_YAW_ANGLE,
	BOY_LEFT_LOWER_LEG_PITCH_ANGLE, BOY_RIGHT_LOWER_LEG_PITCH_ANGLE, BOY_LEFT_FOOT_PITCH_ANGLE, BOY_RIGHT_FOOT_PITCH_ANGLE, 
	GIRL_HEAD_PITCH, GIRL_HEAD_TILT, GIRL_SHOULDER_LENGTH,
	GIRL_SHOULDER_LEFT_ANGLE_FLAP, GIRL_SHOULDER_RIGHT_ANGLE_FLAP, GIRL_SHOULDER_LEFT_ANGLE_RAISE, GIRL_SHOULDER_RIGHT_ANGLE_RAISE, GIRL_SHOULDER_LEFT_ANGLE_AXIAL, GIRL_SHOULDER_RIGHT_ANGLE_AXIAL,
	GIRL_ELBOW_LEFT_ANGLE, GIRL_ELBOW_RIGHT_ANGLE,
	THOUSERS,
	GIRL_LEFT_UPPER_LEG_PITCH_ANGLE, GIRL_RIGHT_UPPER_LEG_PITCH_ANGLE, GIRL_LEFT_UPPER_LEG_ROLL_ANGLE, GIRL_RIGHT_UPPER_LEG_ROLL_ANGLE, GIRL_LEFT_UPPER_LEG_YAW_ANGLE, GIRL_RIGHT_UPPER_LEG_YAW_ANGLE,
	GIRL_LEFT_LOWER_LEG_PITCH_ANGLE, GIRL_RIGHT_LOWER_LEG_PITCH_ANGLE, GIRL_LEFT_FOOT_PITCH_ANGLE, GIRL_RIGHT_FOOT_PITCH_ANGLE,
	NUMCONTROLS // special marker, do NOT move. all controls must be above
};

// The animation will repeat itself after such many frames 

#define ANIMATION_FRAMES_COUNT	120

// Colors

#define COLOR_EYES	0.0f, 0.0f, 0.0f
#define COLOR_BOY_BODY  0.5f, 0.6f, 1.0f
#define COLOR_GIRL_BODY  1.0f, 0.6f, 0.5f
#define COLOR_MOUTH	1.0f, 0.4f, 0.4f

// Dimensions (please, trail each integer with `.0` to make sure it is treated as a double)
	// Common
		// BOY_GIRL_ANGLE							variable
	// Boy
		// Body
		#define BOY_BODY_X							2.0
		#define BOY_BODY_Y							0.75
		#define BOY_BODY_Z							2.25
		// Arm
		#define BOY_UPPER_ARM_LENGTH				1.375
		#define BOY_UPPER_ARM_RADIUS				2 * 0.2
			 // BOY_ELBOW_ANGLE						variable
		#define BOY_LOWER_ARM_LENGTH				1.25
		#define BOY_LOWER_ARM_RADIUS				2 * 0.15
			 // BOY_SHOULDER_ANGLE					variable
		#define HAND_SCALE_RATIO					0.1
		// Head
		#define BOY_HEAD_X							1.125
		#define BOY_HEAD_Y							0.875
		#define BOY_HEAD_Z							1.25
			 // BOY_HEAD_PITCH						variable
			 // BOY_HEAD_TILT						variable
		// Leg
		#define BOY_UPPER_LEG_LENGTH				1.25
		#define BOY_UPPER_LEG_X						0.625
		#define BOY_UPPER_LEG_Y						0.5
		#define BOY_LOWER_LEG_LENGTH				1.25
		#define BOY_LOWER_LEG_RADIUS				0.5
			 // BOY_LEFT_UPPER_LEG_PITCH_ANGLE		variable
			 // BOY_RIGHT_UPPER_LEG_PITCH_ANGLE		variable
			 // BOY_LEFT_UPPER_LEG_ROLL_ANGLE		variable
			 // BOY_RIGHT_UPPER_LEG_ROLL_ANGLE		variable
			 // BOY_LEFT_UPPER_LEG_YAW_ANGLE		variable
			 // BOY_RIGHT_UPPER_LEG_YAW_ANGLE		variable
			 // BOY_LEFT_LOWER_LEG_PITCH_ANGLE		variable		 
			 // BOY_RIGHT_LOWER_LEG_PITCH_ANGLE		variable
		// Penis
		#define BOY_PENIS_DIAMETER					0.25
		#define BOY_PENIS_LENGTH					1.25 
		// Foot
		#define BOY_FOOT_LENGTH						0.875
		#define BOY_FOOT_WIDTH						0.5
		#define BOY_FOOT_THICKNESS					0.175 
			 // BOY_LEFT_FOOT_PITCH_ANGLE			variable
			 // BOY_RIGHT_FOOT_PITCH_ANGLE			variable
	// Girl
		// Body
		#define GIRL_BODY_LENGTH					2.0
		#define GIRL_WAIST_RADIUS					0.75
			 // GIRL_SHOULDER_LENGTH				variable
		#define GIRL_BREAST_RADIUS					0.5
		// Arm
		#define GIRL_UPPER_ARM_LENGTH				1.35
			 // GIRL_ELBOW_LEFT_ANGLE				variable
			 // GIRL_ELBOW_RIGHT_ANGLE				variable
		#define GIRL_LOWER_ARM_LENGTH				1.225
			 // GIRL_SHOULDER_LEFT_ANGLE_FLAP		variable
			 // GIRL_SHOULDER_RIGHT_ANGLE_FLAP		variable
			 // GIRL_SHOULDER_LEFT_ANGLE_RAISE		variable
			 // GIRL_SHOULDER_RIGHT_ANGLE_RAISE		variable
			 // GIRL_SHOULDER_LEFT_ANGLE_AXIAL		variable
			 // GIRL_SHOULDER_RIGHT_ANGLE_AXIAL		variable
		#define GIRL_UPPER_ARM_RADIUS				0.2
		#define GIRL_LOWER_ARM_RADIUS				0.15
		// Head
			 // GIRL_HEAD_PITCH						variable
			 // GIRL_HEAD_TILT						variable
		#define GIRL_HEAD_LENGTH					1.25
		#define GIRL_HEAD_RADIUS					0.625
		#define GIRL_EYES_COORD						0.25, 0.5, 0.125
		#define GIRL_EYES_RADIUS					0.1
		#define GIRL_MOUTH_COORD					0, 0.3, -0.2
		// Leg
		#define GIRL_LEG_RADIUS						0.2
		#define GIRL_UPPER_LEG_LENGTH				1.25
		#define GIRL_LOWER_LEG_LENGTH				1.25
			 // GIRL_LEFT_UPPER_LEG_PITCH_ANGLE		variable
		 	 // GIRL_RIGHT_UPPER_LEG_PITCH_ANGLE	variable
			 // GIRL_LEFT_UPPER_LEG_ROLL_ANGLE		variable
			 // GIRL_RIGHT_UPPER_LEG_ROLL_ANGLE		variable
			 // GIRL_LEFT_UPPER_LEG_YAW_ANGLE		variable
			 // GIRL_RIGHT_UPPER_LEG_YAW_ANGLE		variable
		 	 // GIRL_LEFT_LOWER_LEG_PITCH_ANGLE		variable		 
			 // GIRL_RIGHT_LOWER_LEG_PITCH_ANGLE	variable
		// Foot
			 // GIRL_LEFT_FOOT_PITCH_ANGLE			variable
			 // GIRL_RIGHT_FOOT_PITCH_ANGLE			variable

#endif
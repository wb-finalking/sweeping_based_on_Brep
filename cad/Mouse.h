#ifndef __Mouse__
#define __Mouse__

#define  GLUT_WHEEL_UP 3           //¶¨Òå¹öÂÖ²Ù×÷  
#define  GLUT_WHEEL_DOWN 4 
class Mouse
{
public:
	static double mouseLX, mouseLY, cameraAngleX, cameraAngleY, cameraDistance;
	static double  mouseRX, mouseRY, cameraTransX, cameraTransY;
	static bool mouseLeftDown, mouseLeftUp, mouseRightDown, mouseRightUp;
	static double times;


	static void mouseCB(int button, int state, int x, int y)
	{
		mouseLX = x;
		mouseLY = y;
		mouseRX = x;
		mouseRY = y;

		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				mouseLeftDown = true;
			}
			else if (state == GLUT_UP)
				mouseLeftDown = false;
		}

		else if (button == GLUT_RIGHT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				mouseRightDown = true;
			}
			else if (state == GLUT_UP)
				mouseRightDown = false;
		}
		else if (button == GLUT_WHEEL_UP)
		{
			times+=0.008f;
		}
		else if (button == GLUT_WHEEL_DOWN)
			times -= 0.008f;

	}

	static void mouseMotionCB(int x, int y)
	{
		if (mouseLeftDown)
		{
			cameraAngleY += (x - mouseLX)/5;
			cameraAngleX += (y - mouseLY)/5;
			mouseLX = x;
			mouseLY = y;
		}
		if (mouseRightDown)
		{
			cameraTransX += (x - mouseRX) / 10;
			cameraTransY += (y - mouseRY) / 10;
			mouseRX = x;
			mouseRY = y;
			/*cameraDistance += (y - mouseY) * 0.2f;
			mouseY = y;*/
		}

		glutPostRedisplay();
	}

};

double Mouse::mouseLX = 0;
double Mouse::mouseLY = 0;
double Mouse::cameraAngleX = 0;
double Mouse::cameraAngleY = 0;
double Mouse::cameraDistance = 0;
double Mouse::mouseRX = 0;
double Mouse::mouseRY = 0;
double Mouse::cameraTransX = 0;
double Mouse::cameraTransY = 0;
double Mouse::times = 1;
bool Mouse::mouseLeftDown = false;
bool Mouse::mouseLeftUp = false;
bool Mouse::mouseRightDown = false;
bool Mouse::mouseRightUp = false;

#endif
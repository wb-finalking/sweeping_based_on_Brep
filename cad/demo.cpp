#include <iostream>
#include <gl\glew.h>
#include <gl\freeglut.h>
#include "structure.h"
#include "elar_operator.h"
#include "Mouse.h"
#include "Construct.h"
#include "GluTess.h"

#pragma comment (lib,"glew32.lib")
using namespace std;
ElarOperator op;
Solid* any_obj;


void  init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 100.0);
	//gluPerspective(75, 1, 1, 2000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	double point[4][3] = { { 25, 25, -10 }, { 75, 25, -10 }, { 75, 75, -10 }, { 25, 75, -10 } };
	construct_out(op, any_obj, point,4);
	double point2[4][3] = { { 30, 30, -10 }, { 35, 30, -10 }, { 35, 35, -10 }, { 30, 35, -10 } };
	construct_inner(op, any_obj, point2, 4);
	double point3[4][3] = { { 40, 40, -10 }, { 60, 40, -10 }, { 60, 60, -10 }, { 40, 65, -10 } };
	construct_inner(op, any_obj, point3, 4);

	double dir[3] = { 0, 0, -1 };
	op.sweep(any_obj, dir, 10);
}



void  draw() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	HalfEdge* he = any_obj->faces->out_lp->halfedges,*org;
	org = he;

	glLoadIdentity();
	glTranslatef(Mouse::cameraTransX, 0, 0);
	glTranslatef(0, -Mouse::cameraTransY, 0);


	glRotatef(Mouse::cameraAngleX, 1, 0, 0);
	glRotatef(Mouse::cameraAngleY, 0, 1, 0);

	glScalef(Mouse::times, Mouse::times, Mouse::times);
	//gluLookAt(0.0, 0.0, 80.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glColor3f(1, 0, 0);

	Face* f = any_obj->faces;
	while (f!=NULL)
	{
		face_show(f);
		f = f->next;
	}

	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

}


int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_RGB);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(800, 600);
	/*glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);*/
	glutCreateWindow("demo");

	bool glewExperimental = GLU_TRUE;
	glewInit();
	glGetError();

	init();
	glutDisplayFunc(draw);
	glutReshapeFunc(reshape);
	//glutIdleFunc(renderScene);  
	glutMouseFunc(Mouse::mouseCB);
	glutMotionFunc(Mouse::mouseMotionCB);
	glutMainLoop();
	return 0;
}
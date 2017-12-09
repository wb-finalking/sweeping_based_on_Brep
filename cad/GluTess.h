#ifndef __GluTess__
#define __GluTess__


	void CALLBACK vertexCallback(GLvoid *vertex)
{
	const GLdouble *pointer;
	pointer = (GLdouble *)vertex;
	double color[4] = {1,0,0,0};
	glColor3dv(color);//在此设置颜色 
	//glColor3dv(pointer + 3);//在此设置颜色  
	glVertex3dv(pointer);
}

void CALLBACK beginCallback(GLenum which){
	glBegin(which);
}

void CALLBACK endCallback(){
	glEnd();
}

void CALLBACK errorCallback(GLenum errorCode){
	const GLubyte *estring;
	estring = gluErrorString(errorCode);
	fprintf(stderr, "Tessellation Error: %s\n", estring);
	exit(0);
}
void CALLBACK combineCallback(GLdouble coords[3],
	GLdouble *vertex_data[4],
	GLfloat weight[4], GLdouble **dataOut)
{
	GLdouble *vertex;
	int i;
	vertex = (GLdouble *)malloc(6 * sizeof(GLdouble));
	vertex[0] = coords[0];
	vertex[1] = coords[1];
	vertex[2] = coords[2];
	for (i = 3; i < 7; i++)
		vertex[i] = weight[0] * vertex_data[0][i]
		+ weight[1] * vertex_data[1][i]
		+ weight[2] * vertex_data[2][i]
		+ weight[3] * vertex_data[3][i];
	*dataOut = vertex;
}


#endif
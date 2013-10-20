/*
This program creates a porsche, cow, dinopet, or octopus on the screen. It can be rotated by left clicking on the mouse.
All images are centered and scaled. You can refresh the image back to where it started by pressing R or quit the program with Q or Esc.
Keith McGill
11/8/12
*/
#include "Angel.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;
int numVerts, numTris, numEdges;
vec3 *verts;
vec3 *verts2;
int **tris;
const float degToRad = M_PI / 180.0;
float angle = 0.1 * degToRad;

int scale = 0;
double scaleHigh = 0, scaleLow = 0, scaleD = 0;
double vertTempLow[3];
double vertTempHigh[3];
int vertTempCenter[3];

void readOFFMesh(string filename)
{
	string format;
	
	double centX = 0, centY = 0, centZ = 0;

	ifstream in(filename);
	if (in.bad())
	{
		cerr << "File " << filename << " not found. Exiting." << endl;
		system("PAUSE");
	}
	getline(in, format);
	if (format != "OFF")
	{
		cerr << "File " << filename << " not OFF file. Exiting." << endl;
		in.close();
		system("PAUSE");
	}
	in >> numVerts >> numTris >> numEdges;

	verts = new vec3[numVerts];
	verts2 = new vec3[6*numTris];

	for (int i = 0; i < numVerts; i++)
	{
		in >> verts[i][0] >> verts[i][1] >> verts [i][2];
		if (i == 0)
		{
			vertTempLow[0] = verts[i][0];
			vertTempLow[1] = verts[i][1];
			vertTempLow[2] = verts[i][2];
			vertTempHigh[0] = verts[i][0];
			vertTempHigh[1] = verts[i][1];
			vertTempHigh[2] = verts[i][2];
		}
		if (vertTempLow[0] > verts[i][0])
			vertTempLow[0] = verts[i][0];
		if (vertTempLow[1] > verts[i][1])
			vertTempLow[1] = verts[i][1];
		if (vertTempLow[2] > verts[i][2])
			vertTempLow[2] = verts[i][2];

		if (vertTempHigh[0] < verts[i][0])
			vertTempHigh[0] = verts[i][0];
		if (vertTempHigh[1] < verts[i][1])
			vertTempHigh[1] = verts[i][1];
		if (vertTempHigh[2] < verts[i][2])
			vertTempHigh[2] = verts[i][2];
	}

	vertTempCenter[0] = ((int)vertTempHigh[0] + (int)vertTempLow[0]) / 2;
	vertTempCenter[1] = ((int)vertTempHigh[1] + (int)vertTempLow[1]) / 2;
	vertTempCenter[2] = ((int)vertTempHigh[2] + (int)vertTempLow[2]) / 2;
	
	for (int i = 0; i < numVerts; i++)
	{
		verts[i][0] = verts[i][0] - vertTempCenter[0];
		verts[i][1] = verts[i][1] - vertTempCenter[1];
		verts[i][2] = verts[i][2] - vertTempCenter[2];
	}

	for (int i = 0; i < 3; i++)
	{
		if (abs(vertTempHigh[i]) > abs(scaleHigh))
		{
			scaleHigh = abs(vertTempHigh[i]);
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (abs(vertTempLow[i]) < abs(scaleLow))
		{
			scaleLow = abs(vertTempLow[i]);
		}
	}

	if (scaleHigh > 1 || scaleLow > 1)
	{
		if (scaleHigh > scaleLow)
		{
			scale = (int)scaleHigh;
			for (int i = 0; i < numVerts; i++)
			{
			verts[i][0] = (verts[i][0] * 0.9) / scale;
			verts[i][1] = (verts[i][1] * 0.9) / scale;
			verts[i][2] = (verts[i][2] * 0.9) / scale;
			}
		}
		else
		{
			scale = (int)scaleLow;
			for (int i = 0; i < numVerts; i++)
			{
			verts[i][0] = (verts[i][0] * 0.9) / scale;
			verts[i][1] = (verts[i][1] * 0.9) / scale;
			verts[i][2] = (verts[i][2] * 0.9) / scale;
			}
		}
	}
	
	else
	{
		if (scaleHigh > scaleLow)
		{
			scaleD = 0.9 / scaleHigh;
			for (int i = 0; i < numVerts; i++)
			{
			verts[i][0] = (verts[i][0] * scaleD);
			verts[i][1] = (verts[i][1] * scaleD);
			verts[i][2] = (verts[i][2] * scaleD);
			}
		}
		else
		{
			scaleD = 0.9 / scaleLow;
			for (int i = 0; i < numVerts; i++)
			{
			verts[i][0] = (verts[i][0] * scaleD);
			verts[i][1] = (verts[i][1] * scaleD);
			verts[i][2] = (verts[i][2] * scaleD);
			}
		}
	}

	tris = new int*[numTris];
	for (int i = 0; i < numTris; ++i)
		tris[i] = new int[3];
	for (int i = 0; i < numTris; ++i)
	{
		int temp;
		in >> temp;
		in >> tris[i][0] >> tris[i][1] >> tris[i][2];
	}
	for (int i = 0; i < numTris; ++i)
	{
		verts2[6*i] = verts[tris[i][0]];
		verts2[6*i + 1] = verts[tris[i][1]];
		verts2[6*i + 2] = verts[tris[i][1]];
		verts2[6*i + 3] = verts[tris[i][2]];
		verts2[6*i + 4] = verts[tris[i][2]];
		verts2[6*i + 5] = verts[tris[i][0]];
	}
	in.close();
}

void idleRotateZ()
{
	for (int i = 0; i < (6 * numTris); i++)
	{
		float x = cos(angle)*verts2[i].x - sin(angle)*verts2[i].y;
		float y = sin(angle)*verts2[i].x + cos(angle)*verts2[i].y;
		verts2[i].x = x;
		verts2[i].y = y;
	}
	glBufferData(GL_ARRAY_BUFFER, 6 * numTris*sizeof(vec3), verts2, GL_STATIC_DRAW);
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		glutIdleFunc(NULL);
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		glutIdleFunc(idleRotateZ);
		
	}
}

void keyboard(unsigned char key, int width, int height)
{
	switch( key ) {
	case 033:  // Escape key
	case 'q': case 'Q':
	    exit( EXIT_SUCCESS );
	    break;
	case 'r': case 'R':
	    
		for (int i = 0; i < numTris; ++i)
		{
			verts2[6*i] = verts[tris[i][0]];
			verts2[6*i + 1] = verts[tris[i][1]];
			verts2[6*i + 2] = verts[tris[i][1]];
			verts2[6*i + 3] = verts[tris[i][2]];
			verts2[6*i + 4] = verts[tris[i][2]];
			verts2[6*i + 5] = verts[tris[i][0]];
		}
		glBufferData(GL_ARRAY_BUFFER, 6 * numTris*sizeof(vec3), verts2, GL_STATIC_DRAW);
		glutPostRedisplay();
	    break;
    }
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_LINES, 0, 6 * numTris);
    glFlush();
}

void init()
{   
    // Vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Buffer object
    GLuint buffer;
    glGenBuffers(1, &buffer );
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // Empty buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * numTris*sizeof(vec3), verts2, GL_STATIC_DRAW);

    // Load shaders and use the resulting shader program
    GLuint program = InitShader("vshader_lab2.glsl", "fshader_lab2.glsl");
    glUseProgram(program);

    // Initialize the vertex position attribute from the vertex shader    
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0);
	glPointSize(2);
}

int main(int argc, char **argv)
{  
	string filename = "Cow5804.off";
	//string filename = "Dinopet.off";
	//string filename = "82porsche.off";
	//string filename = "Octopus.off";
	//string filename = "chicken.off";
	//string filename = "elephant.off";
	//string filename = "horse.off";
	//string filename = "pitbull.off";
    readOFFMesh(filename);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("CSC 2078 - Lab #2 - Keith McGill");
    
	glewExperimental = GL_TRUE;
	glewInit();
	init();	
	
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
	glutIdleFunc(NULL);
      
    glutMainLoop();
	return 0;
}
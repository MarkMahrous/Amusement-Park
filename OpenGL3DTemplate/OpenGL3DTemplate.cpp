#include<iostream>
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <MMSystem.h>
#include <mciapi.h>
#include <stdlib.h>
#include <glut.h>
#include<algorithm>
#include <math.h>
#include <random>
#include <Mmsystem.h>
#include <mciapi.h>
#include <playsoundapi.h>
#include <chrono>
#include <thread>

#pragma comment(lib,"winmm.lib")
#define getMin(a,b) (((a) > (b)) ? (b) : (a))
#define getMax(a,b) (((a) > (b)) ? (a) : (b))

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

float playerPos[3] = { 1, 0.3, 1 };
float wallCol = 0.0;
float goalPos[3][2] = { {0.1,0.5},{0.5,0.9},{0.8,0.7} };
bool goalBool[3];

float gateScale = 1.0;
float playerRot = 0;
float objectRot = 0;
float goalRot = 0;
int collect = 0;
bool animate = false;
int winner = 0;
int loser = 0;
int timer = 100;
int inc = 0;

class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Cam {
public:
	Vector3f eye, center, top;

	Cam(float X = 1.0f, float Y = 1.0f, float Z = 1.0f, float cenX = 0.0f, float cenY = 0.0f, float cenZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(X, Y, Z);
		center = Vector3f(cenX, cenY, cenZ);
		top = Vector3f(upX, upY, upZ);
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			top.x, top.y, top.z
		);
	}
	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = top.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + top * sin(DEG2RAD(a));
		top = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = top.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(top);
		center = eye + view;
	}
	void moveX(float d) {
		Vector3f right = top.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + top.unit() * d;
		center = center + top.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}
};

Cam camera;

//Print Text
//the first two are the coordinates where the text is display,
//the third coordinate is the string containing the text to display
void print(int x, int y, int z, char* string) {
	int len, i;
	glRasterPos3f(x, y, z);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
}

bool checkIntersect(float playerPositions[], float goal[]) {
	float x1 = playerPositions[0] / 2 - 0.1, x2 = playerPositions[0] / 2 + 0.1;
	float z1 = playerPositions[2] / 2 - 0.1, z2 = playerPositions[2] / 2 + 0.1;
	return (goal[0] >= x1 && goal[0] <= x2 && goal[1] >= z1 && goal[1] <= z2);
}

void handleCollisions() {
	for (int i = 0; i < 3; i++) {
		if (goalBool[i] == false && checkIntersect(playerPos, goalPos[i])) {
			goalBool[i] = true;
			collect++;
			printf("Collect %d\n", collect);
		}
	}
}

void Keyboard(unsigned char key, int x, int y) {
	float offSet = 0.01;

	switch (key) {
	case 'z':
		animate = true;
		break;
	case 'x':
		animate = false;
		break;
	case '`':
		printf("%f %f %f\n", camera.eye.x, camera.eye.y, camera.eye.z);
		printf("%f %f %f\n", camera.center.x, camera.center.y, camera.center.z);
		printf("%f %f %f\n", camera.top.x, camera.top.y, camera.top.z);
		break;
	case '+':
		camera.moveZ(offSet);
		break;
	case '-':
		camera.moveZ(-offSet);
		break;

	case 'w':
		camera.moveY(offSet);
		break;
	case 'a':
		camera.moveX(offSet);
		break;
	case 's':
		camera.moveY(-offSet);
		break;
	case 'd':
		camera.moveX(-offSet);
		break;


	case '0':
		camera.eye = Vector3f(1.509249, 1.509249, 1.509249);
		camera.center = Vector3f(-0.490748, -0.490748, -0.490748);
		camera.top = Vector3f(0, 1, 0);
		break;
	case '7':
		camera.eye = Vector3f(0.546511, 0.368942, 1.943849);
		camera.center = Vector3f(0.505704, 0.334042, 0.945292);
		camera.top = Vector3f(-0.001425, 0.999391, -0.034870);
		break;
	case '8':
		camera.eye = Vector3f(0.5, 1.520059, 0.503368);
		camera.center = Vector3f(0.5, 0.520209, 0.485916);
		camera.top = Vector3f(0, 0.017452, -0.999848);
		break;
	case '9':
		camera.eye = Vector3f(1.963135, 0.349816, 0.431126);
		camera.center = Vector3f(0.964986, 0.367269, 0.489381);
		camera.top = Vector3f(0.017423, 0.999848, -0.001017);
		break;

	case '5':
		playerRot = 180;
		playerPos[2] = getMax(0.0, playerPos[2] - 0.01);
		break;
	case '1':
		playerRot = -90;
		playerPos[0] = getMax(0.0, playerPos[0] - 0.01);
		break;
	case '2':
		playerRot = 0;
		playerPos[2] = getMin(2.0, playerPos[2] + 0.01);
		break;
	case '3':
		playerRot = 90;
		playerPos[0] = getMin(2.0, playerPos[0] + 0.01);
		break;

	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	handleCollisions();
	glutPostRedisplay();
}

void View(int key, int x, int y) {
	float a = 1.0;

	switch (key) {
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	}

	glutPostRedisplay();
}

void lights() {
	GLfloat amb[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diff[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shin[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMaterialfv(GL_FRONT, GL_SHININESS, shin);

	GLfloat Intensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPos[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, Intensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Intensity);
}

void cam() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

void wallPart(double z) {
	glPushMatrix();
	glTranslated(0, 0.5 / 2, z);
	glScaled(0.01, 0.5, 0.05);
	glutSolidCube(1.0);
	glPopMatrix();
}

void wall() {
	glPushMatrix();
	glColor3f(0.5, 0.5, wallCol);

	for (int i = 0; i < 11; i++) {
		wallPart(i * 0.1);
	}

	glPushMatrix();
	glRotated(90, 0, 1, 0);
	for (int i = 0; i < 11; i++) {
		wallPart(i * 0.1);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslated(1.0, 0, 0);
	glRotated(-90, 0, 1, 0);
	glRotated(90, 0, 1, 0);
	for (int i = 0; i < 11; i++) {
		wallPart(i * 0.1);
	}
	glPopMatrix();

}

// 1 primitive for the ground
void ground() {
	glColor3f(0.2, 1.0, 1.0);
	glPushMatrix();
	glTranslated(0.5, 0.01, 0.5);
	glScaled(1.0, 0.02, 1.0);
	glutSolidCube(1);
	glPopMatrix();
}

// 6 primitives for the gate
void gate() {
	glPushMatrix();
	glTranslated(0.15, 0, 0);
	glScaled(gateScale, gateScale, gateScale);
	glRotated(-90, 0, 1, 0);

	// Gate top using spheres
	glColor3f(0.8, 0.8, 0.8);
	glPushMatrix();
	glTranslated(0.35, 0.58, 0.04);
	glutSolidSphere(0.05, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(1 - 0.35, 0.58, 0.04);
	glutSolidSphere(0.05, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.5, 0.58, 0.04);
	glutSolidSphere(0.05, 20, 20);
	glPopMatrix();

	// Columns using cylinders
	glColor3f(0.8, 0.8, 0.8);

	glPushMatrix();
	glTranslated(0.35, 0.03, 0.05);
	glRotated(-90, 1, 0, 0);
	GLUquadricObj* quadratic;
	quadratic = gluNewQuadric();
	gluCylinder(quadratic, 0.05, 0.05, 0.5, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(1 - 0.35, 0.012, 0.05);
	glRotated(-90, 1, 0, 0);
	gluCylinder(quadratic, 0.05, 0.05, 0.5, 20, 20);
	glPopMatrix();

	// Horizontal bar
	glColor3f(0.2, 0.2, 0.2);
	glPushMatrix();
	glTranslated(0.5, 0.5, 0.05);
	glScaled(0.5, 0.07, 0.07);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();
}

// 10 primitives for the tree
void tree() {
	glPushMatrix();
	glTranslated(0.15, 0, 0.1);
	// Trunk
	glColor3f(0.376, 0.192, 0.082); // Brown for the trunk
	glPushMatrix();
	glTranslated(-0.15, 0, -0.1);
	glRotated(-90, 1, 0, 0);
	GLUquadricObj* quadratic;
	quadratic = gluNewQuadric();
	gluCylinder(quadratic, 0.08, 0.05, 0.5, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.15, 0.3, -0.1);
	glRotated(40, 0, 0, 1);
	glRotated(-90, 1, 0, 0);
	gluCylinder(quadratic, 0.05, 0.02, 0.3, 8, 8);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.15, 0.3, -0.1);
	glRotated(-40, 0, 0, 1);
	glRotated(-90, 1, 0, 0);
	gluCylinder(quadratic, 0.05, 0.02, 0.25, 8, 8);
	glPopMatrix();

	// Branches
	glPushMatrix();
	glColor3f(0.133, 0.545, 0.133); // Dark Green
	glScaled(0.9, 0.5, 0.9);
	glTranslated(-0.15, 1, -0.15);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.15, 0.65, 30, 5);
	glPopMatrix();

	//6 red Apples
	glColor3f(1.0, 0.1, 0.1);
	glPushMatrix();
	glScaled(0.6, 0.6, 0.6);
	glTranslated(-0.15, 1.2, -0.18);
	glutSolidSphere(0.03, 20, 20);
	glPopMatrix();

	glColor3f(1.0, 0.1, 0.1);
	glPushMatrix();
	glScaled(0.6, 0.6, 0.6);
	glTranslated(-0.25, 1.15, -0.14);
	glutSolidSphere(0.03, 20, 20);
	glPopMatrix();

	glColor3f(1.0, 0.1, 0.1);
	glPushMatrix();
	glScaled(0.6, 0.6, 0.6);
	glTranslated(-0.15, 1, -0.09);
	glutSolidSphere(0.03, 20, 20);
	glPopMatrix();

	glColor3f(1.0, 0.1, 0.1);
	glPushMatrix();
	glScaled(0.6, 0.6, 0.6);
	glTranslated(-0.1, 1.05, -0.3);
	glutSolidSphere(0.03, 20, 20);
	glPopMatrix();

	glColor3f(1.0, 0.1, 0.1);
	glPushMatrix();
	glScaled(0.6, 0.6, 0.6);
	glTranslated(-0.05, 1.05, -0.12);
	glutSolidSphere(0.03, 20, 20);
	glPopMatrix();

	glColor3f(1.0, 0.1, 0.1);
	glPushMatrix();
	glScaled(0.6, 0.6, 0.6);
	glTranslated(-0.33, 1.05, -0.33);
	glutSolidSphere(0.03, 20, 20);
	glPopMatrix();

	glPopMatrix();
}

// 3 trees
void trees() {
	glPushMatrix();
	glScaled(0.5, 0.5, 0.5);
	glTranslated(0.25, 0, 0.25);
	glRotated(objectRot, 0, 1, 0);
	tree();
	glPopMatrix();

	glPushMatrix();
	glScaled(0.5, 0.5, 0.5);
	glTranslated(0.65, 0, 0.25);
	glRotated(objectRot, 0, 1, 0);
	tree();
	glPopMatrix();

	glPushMatrix();
	glScaled(0.5, 0.5, 0.5);
	glTranslated(1.0, 0, 0.25);
	glRotated(objectRot, 0, 1, 0);
	tree();
	glPopMatrix();

}

// 11 primitives for the zombie
void zombie() {
	glPushMatrix();
	glTranslated(0.85, 0.25, 0.15);
	glScaled(0.5, 0.5, 0.5);
	glRotated(objectRot, 0, 1, 0);
	//printf("%d\n", objectRot);

	// Head using a larger sphere with red eyes
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslated(0, 0.1, 0);
	glutSolidSphere(0.15, 15, 15);

	// Red eyes using smaller spheres
	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslated(0.06, 0.08, 0.13);
	glutSolidSphere(0.03, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.06, 0.08, 0.13);
	glutSolidSphere(0.03, 15, 15);
	glPopMatrix();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslated(0.06, 0.08, 0.16);
	glScaled(0.3, 0.3, 0.3);
	glutSolidSphere(0.03, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.06, 0.08, 0.16);
	glScaled(0.3, 0.3, 0.3);
	glutSolidSphere(0.03, 15, 15);
	glPopMatrix();

	// Simple smile using a curve
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0, 0.0, 0.15);
	glRotated(90, 1, 0, 0);
	glutWireTorus(0.02, 0.04, 50, 50); // You can use other primitives for a different smile
	glPopMatrix();

	glPopMatrix();

	// Body using a scaled cube
	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslated(0, -0.1, 0);
	glScaled(0.5, 0.8, 0.3);
	glutSolidCube(0.3);
	glPopMatrix();

	// Arms using elongated cubes
	glColor3f(1, 0.6, 0);
	glPushMatrix();
	glTranslated(0.2, 0, 0);
	//glRotated(rotateIce, 0, 1, 0);
	glScaled(0.6, 0.15, 0.15);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.2, 0, 0);
	//glRotated(rotateIce, 0, 1, 0);
	glScaled(0.6, 0.15, 0.15);
	glutSolidCube(0.3);
	glPopMatrix();

	// Legs using scaled cylinders
	glColor3f(0, 0, 0);
	GLUquadricObj* quadratic;
	quadratic = gluNewQuadric();

	glPushMatrix();
	glTranslated(-0.1, -0.5, 0);
	glRotated(-90, 1, 0, 0);
	gluCylinder(quadratic, 0.1f, 0.1f, 0.4f, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.1, -0.5, 0);
	glRotated(-90, 1, 0, 0);
	gluCylinder(quadratic, 0.1f, 0.1f, 0.4f, 20, 20);
	glPopMatrix();

	glPopMatrix();
}

// 10 primitives for the player
void Player() {
	glPushMatrix();
	glScaled(0.5, 0.5, 0.5);
	glTranslated(playerPos[0], playerPos[1], playerPos[2]);
	glRotated(playerRot, 0, 1, 0);

	// Head using a sphere
	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslated(0, 0.16, 0);
	glScaled(0.65, 0.65, 0.65);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();

	// white eyes using smaller spheres
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslated(0.028, 0.17, 0.06);
	glScaled(0.5, 0.5, 0.5);
	glutSolidSphere(0.03, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.028, 0.17, 0.06);
	glScaled(0.5, 0.5, 0.5);
	glutSolidSphere(0.03, 15, 15);
	glPopMatrix();

	glColor3f(0.35, 0.05, 0);
	glPushMatrix();
	glTranslated(0.028, 0.17, 0.07);
	glScaled(0.2, 0.2, 0.2);
	glutSolidSphere(0.03, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.028, 0.17, 0.07);
	glScaled(0.2, 0.2, 0.2);
	glutSolidSphere(0.03, 15, 15);
	glPopMatrix();

	// Body using a cube
	glColor3f(0, 0, 1);
	glPushMatrix();
	glTranslated(0, 0, 0);
	glScaled(0.3, 0.4, 0.2);
	glutSolidCube(0.5);
	glPopMatrix();

	// Legs using cylinders
	glColor3f(1, 1, 0);
	GLUquadricObj* quadratic;
	quadratic = gluNewQuadric();

	glPushMatrix();
	glTranslated(-0.08, -0.3, 0);
	glScaled(0.65, 1, 0.65);
	glRotated(-90, 1, 0, 0);
	gluCylinder(quadratic, 0.05f, 0.05f, 0.25f, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.08, -0.3, 0);
	glScaled(0.65, 1, 0.65);
	glRotated(-90, 1, 0, 0);
	gluCylinder(quadratic, 0.05f, 0.05f, 0.25f, 20, 20);
	glPopMatrix();

	// Arms using scaled cubes
	glColor3f(0, 1, 0);
	glPushMatrix();
	glTranslated(-0.15, 0.1, 0);
	glScaled(0.26, 0.1, 0.1);
	glutSolidCube(0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.15, 0.1, 0);
	glScaled(0.26, 0.1, 0.1);
	glutSolidCube(0.5);
	glPopMatrix();

	glPopMatrix();
}

void ParkGame() {
	glPushMatrix();
	glScaled(0.65, 0.65, 0.65);
	glTranslated(1.2, 0, 1.2);
	glRotated(objectRot, 0, 1, 0);

	glColor3f(0.85, 0.7, 0.0);
	glPushMatrix();
	glTranslated(0, 0.03, 0.0);
	glRotated(-90, 1, 0, 0);
	GLUquadricObj* quadratic;
	quadratic = gluNewQuadric();
	gluCylinder(quadratic, 0.05, 0.05, 0.5, 20, 20);
	glPopMatrix();

	// Add a sphere at the top
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();
	glTranslated(0, 0.55, 0.0);
	glutSolidSphere(0.05, 20, 20);
	glPopMatrix();

	// Add the torus at the top
	glColor3f(0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslated(0, 0.5, 0.0);
	glRotated(90, 1, 0, 0);
	glutSolidTorus(0.05, 0.15, 20, 20);
	glPopMatrix();

	// Add the torus at the middle
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glTranslated(0, 0.3, 0.0);
	glScaled(0.8, 0.8, 0.8);
	glRotated(90, 1, 0, 0);
	glutSolidTorus(0.05, 0.15, 20, 20);
	glPopMatrix();

	// Add the torus at the bottom
	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();
	glTranslated(0, 0.1, 0.0);
	glScaled(0.6, 0.6, 0.6);
	glRotated(90, 1, 0, 0);
	glutSolidTorus(0.05, 0.15, 20, 20);
	glPopMatrix();

	// Add four cubes as wires
	glColor3f(0.9, 0.1, 1.0);
	glPushMatrix();
	glTranslated(0.23, 0.35, 0.0);
	glRotated(-70, 0, 0, 1);
	glPushMatrix();
	glScaled(2.5, 0.25, 0.25);
	glutSolidCube(0.1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.23, 0.35, 0.0);
	glRotated(70, 0, 0, 1);
	glPushMatrix();
	glScaled(2.5, 0.25, 0.25);
	glutSolidCube(0.1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 0.35, 0.23);
	glRotated(-20, 1, 0, 0);
	glRotated(90, 0, 0, 1);
	glPushMatrix();
	glScaled(2.5, 0.25, 0.25);
	glutSolidCube(0.1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 0.35, -0.23);
	glRotated(20, 1, 0, 0);
	glRotated(90, 0, 0, 1);
	glPushMatrix();
	glScaled(2.5, 0.25, 0.25);
	glutSolidCube(0.1);
	glPopMatrix();
	glPopMatrix();


	glPopMatrix();
}

void smallCar() {
	glPushMatrix();
	glScaled(0.4, 0.4, 0.4);
	glTranslated(2, 0, 1.15);
	glRotated(objectRot + 90, 0, 1, 0);

	// Car body
	glColor3f(1.0, 0.0, 0.0); // Red color for the car body
	glPushMatrix();
	glTranslated(0, 0.15, 0); // Adjust position
	glScaled(1.5, 0.5, 0.5); // Scale to form a rectangular shape
	glutSolidCube(0.5); // Adjust size as needed
	glPopMatrix();

	// Car top
	glColor3f(0.0, 0.0, 1.0); // Blue color for the top of the car
	glPushMatrix();
	glTranslated(0, 0.3, 0); // Raise the top part
	glScaled(1, 0.5, 0.5); // Scale to form a smaller rectangular shape
	glutSolidCube(0.35); // Adjust size as needed
	glPopMatrix();

	// Wheels
	glColor3f(0.0, 0.0, 0.0); // Black color for the wheels
	glPushMatrix();
	glTranslated(-0.2, 0.1, 0.15); // Front left wheel position
	glutSolidTorus(0.03, 0.05, 10, 10); // Adjust dimensions
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.2, 0.1, 0.15); // Front right wheel position
	glutSolidTorus(0.03, 0.05, 10, 10); // Adjust dimensions
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.2, 0.1, -0.15); // Back left wheel position
	glutSolidTorus(0.03, 0.05, 10, 10); // Adjust dimensions
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.2, 0.1, -0.15); // Back right wheel position
	glutSolidTorus(0.03, 0.05, 10, 10); // Adjust dimensions
	glPopMatrix();

	glPopMatrix();
}

void goal() {
	glPushMatrix();
	glScaled(0.5, 0.5, 0.5);
	glTranslated(0.0, 0.08, 0.0);

	glColor3f(1.0, 0.0, 0.0); // Red color
	glPushMatrix();
	glutSolidCube(0.1);
	glPopMatrix();

	glColor3f(0, 0, 1.0); // Blue color
	glPushMatrix();
	glScaled(0.3, 0.3, 0.3);
	glTranslated(0, 0.15, 0);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();

	glColor3f(0.8, 0.8, 0.0); // Yellow color
	glPushMatrix();
	glScaled(0.2, 0.2, 0.2);
	glTranslated(0, 0.35, 0);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();

	glPopMatrix();
}

void TheGoal() {

	if (!goalBool[0]) {
		glPushMatrix();
		glTranslated(goalPos[0][0], 0, goalPos[0][1]);
		glScaled(1, 1, 1);
		glRotated(goalRot, 0, 1, 0);
		glPushMatrix();
		goal();
		glPopMatrix();
		glPopMatrix();
	}

	if (!goalBool[1]) {
		glPushMatrix();
		glTranslated(goalPos[1][0], 0, goalPos[1][1]);
		glPushMatrix();
		glScaled(1, 1, 1);
		glRotated(goalRot, 0, 1, 0);
		goal();
		glPopMatrix();
		glPopMatrix();
	}

	if (!goalBool[2]) {
		glPushMatrix();
		glTranslated(goalPos[2][0], 0, goalPos[2][1]);
		glPushMatrix();
		glScaled(1, 1, 1);
		glRotated(goalRot, 0, 1, 0);
		goal();
		glPopMatrix();
		glPopMatrix();
	}
}

void Display() {
	cam();
	lights();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (winner == 1) {
		sndPlaySound(TEXT("game.wav"), SND_ASYNC | SND_LOOP | SND_FILENAME);
		char* p0s[20];
		sprintf((char*)p0s, "You Win");
		print(0.5, 0.5, 0.5, (char*)p0s);
	}
	else if (loser == 1) {
		char* p0s[20];
		sprintf((char*)p0s, "You Lose");
		print(0.5, 0.5, 0.5, (char*)p0s);
	}
	else {
		ground();

		wall();

		gate();

		Player();

		TheGoal();

		zombie();

		smallCar();

		trees();

		ParkGame();
	}

	glFlush();
}

void AnimateObjects(int val)
{
	if (animate) {
		objectRot += 10;
		if (gateScale == 1.0) {
			gateScale = 1.01;
		}
		else {
			gateScale = 1.0;
		}
	}
	if (inc == 1) {
		wallCol += 0.1;
	}
	else {
		wallCol -= 0.1;
	}
	if (wallCol <= 0.0) {
		inc = 1;
	}
	if (wallCol >= 1.0) {
		inc = 0;
	}
	goalRot += 10;
	glutPostRedisplay();
	glutTimerFunc(100, AnimateObjects, 0);
}

void timeFuncAnim(int val)
{
	timer--;
	if (timer <= 0) {
		if (collect == 3) {
			winner = 1;
			printf("%d winner\n", winner);
		}
		else {
			loser = 1;
			printf("%d loser\n", loser);
			if (loser) {
				printf("loser\n");
			}
		}
	}

	printf("%d\n", timer);
	glutPostRedisplay();
	glutTimerFunc(1000, timeFuncAnim, 0);
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(800, 600);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Player Collecting goals");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(View);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

	glClearColor(0.7f, 0.7f, 0.7f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glutTimerFunc(0, timeFuncAnim, 0);
	glutTimerFunc(0, AnimateObjects, 0);
	glShadeModel(GL_SMOOTH);

	sndPlaySound(TEXT("game22.wav"), SND_ASYNC | SND_LOOP | SND_FILENAME);
	glutMainLoop();
}


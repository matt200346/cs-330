// Matthew Eaton
// CS 330 3D scene final project
// 08/14/22
// This is the main file that contains main
// main initializes the window creates the 3d objects, shades them, and then checks input for camera control that is passed to the mainCam object
// this program creates a plane and 3d objects on the plane for a full low poly 3d scene with camera movement

#include <iostream>         // cout, cerr etc.
#include <cstdlib>          // EXIT_FAILURE
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library

#include "Shader.h"
#include "Camera.h"
//#include "Plane.h"
//#include "Icosahedron.h"
//#include "Sphere.h"

using namespace std;


namespace
{
	const char* const WINDOW_TITLE = "3D scene Matt Eaton";

	// Window width and height
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;
	const double PI = 3.14159265359;

	int sizeVerts;
	int sizeVerts2;
	int planeSizeVerts;
	int icoSizeVerts;
	int cubeSizeVerts;

	float moveSpeed = 1.5f;
	// Main window
	GLFWwindow* gWindow = nullptr;
	// Main camera
	Camera mainCam(WINDOW_HEIGHT, WINDOW_WIDTH);
	double gLastX = WINDOW_WIDTH / 2.0f;
	double gLastY = WINDOW_HEIGHT / 2.0f;
	bool gFirstMouse = true;

	// delta time related variables
	// double may be overkill
	double deltaTime = 0.0f;
	double lastFrame = 0.0f;

	// textureId variable
	GLuint texture1Id;
	GLuint texture2Id;
	GLuint zeroNumTextureId;
	GLuint greenPlasticTextureId;
	GLuint blueTextureId;
	GLuint lemonTextureId;
}


bool UInitialize(int, char* [], GLFWwindow** window);
void UProcessInput(GLFWwindow* window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void singleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
float* CrossProduct(float a[3], float b[3], float c[3]);
//##########################################################################################################################
// Object Verts

// pyramid 1 verts
GLfloat verts[] =
	{
		// Pyramid Coordinates and colors
		// sides are one solid color

	// vertex pos			// colors			  //Texture coord     // normal coord
	 0.0f, 0.7f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f,   0.5f, 1.0f,      0.0f,  0.5f,  0.7f,   // Top of pyramid (point 0) - Yellow
	-0.5f, 0.0f, 0.5f,   1.0f, 1.0f, 0.0f, 1.0f,   0.0f, 0.0f,      0.0f,  0.5f,  0.7f,   // Bottom-left of front triangle (point 1) - Yellow
	 0.5f, 0.0f, 0.5f,   1.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,      0.0f,  0.5f,  0.7f,   // Bottom-right of front triangle (point 2) - Yellow
	 //--------end of front facing tri --------------------------------------------------------------
	 0.0f, 0.7f,  0.0f,  1.0f, 0.0f, 1.0f, 1.0f,   0.5f, 1.0f,     -0.7f,  0.5f,  0.0f,  // Top of pyramid (point 3) - Pink
	-0.5f, 0.0f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f,   0.0f, 0.0f,     -0.7f,  0.5f,  0.0f,  // Back-left of square base (point 4) - Pink
	-0.5f, 0.0f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f,   1.0f, 0.0f,     -0.7f,  0.5f,  0.0f,  // Bottom-left of front triangle (point 5) - Pink
	//---------end of left side tri -----------------------------------------------------------------
	 0.0f, 0.7f,  0.0f,  0.0f, 0.0f, 1.0f, 1.0f,   0.5f, 1.0f,      0.7f,  0.5f,  0.0f,   // Top of pyramid (point 6) - Blue
	 0.5f, 0.0f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 0.0f,      0.7f,  0.5f,  0.0f,   // Bottom-right of front triangle (point 7) - Blue
	 0.5f, 0.0f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f,   1.0f, 0.0f,      0.7f,  0.5f,  0.0f,   // Back-right of square base (point 8) - Blue
	//---------end of right side tri ----------------------------------------------------------------
	 0.0f, 0.7f,  0.0f,  1.0f, 0.5f, 0.0f, 1.0f,   0.5f, 1.0f,      0.0f,  0.5f, -0.7f,   // Top of pyramid (point 9) - Orange
	 0.5f, 0.0f, -0.5f,  1.0f, 0.5f, 0.0f, 1.0f,   0.0f, 0.0f,      0.0f,  0.5f, -0.7f,   // Back-right of square base (point 10) - Orange
	-0.5f, 0.0f, -0.5f,  1.0f, 0.5f, 0.0f, 1.0f,   1.0f, 0.0f,      0.0f,  0.5f, -0.7f,   // Back-left of square base (point 11) - Orange
	//---------end of back side tri -----------------------------------------------------------------
	-0.5f, 0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f,   0.0f, 1.0f,      0.0f, -1.0f,  0.0f,   // Front-left of square base (point 12) - Green 
	-0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,   0.0f, 0.0f,      0.0f, -1.0f,  0.0f,   // Back-left of square base (point 13) - Green
	 0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,      0.0f, -1.0f,  0.0f,   // Back-right of square base (point 14) - Green
	 0.5f, 0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 1.0f,      0.0f, -1.0f,  0.0f    // Front-right of square base(point 15) - Green
	 //--------end of bottom square -------------------------------------------------------------

	};

GLfloat verts2[] =
{
	// Pyramid Coordinates and colors
	// sides are one solid color

	// vertex pos			// colors			  //Texture coord     // normal coord
	 0.0f, -0.7f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f,   0.5f, 1.0f,      0.0f,  -0.5f,  0.7f,   // Top of pyramid (point 0) - Yellow
	-0.5f,  0.0f, 0.5f,   1.0f, 1.0f, 0.0f, 1.0f,   0.0f, 0.0f,      0.0f,  -0.5f,  0.7f,   // Bottom-left of front triangle (point 1) - Yellow
	 0.5f,  0.0f, 0.5f,   1.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,      0.0f,  -0.5f,  0.7f,   // Bottom-right of front triangle (point 2) - Yellow
	 //--------end of front facing tri --------------------------------------------------------------
	 0.0f, -0.7f,  0.0f,  1.0f, 0.0f, 1.0f, 1.0f,   0.5f, 1.0f,     -0.7f,  -0.5f,  0.0f,  // Top of pyramid (point 3) - Pink
	-0.5f,  0.0f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f,   0.0f, 0.0f,     -0.7f,  -0.5f,  0.0f,  // Back-left of square base (point 4) - Pink
	-0.5f,  0.0f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f,   1.0f, 0.0f,     -0.7f,  -0.5f,  0.0f,  // Bottom-left of front triangle (point 5) - Pink
	//---------end of left side tri -----------------------------------------------------------------
	 0.0f, -0.7f,  0.0f,  0.0f, 0.0f, 1.0f, 1.0f,   0.5f, 1.0f,      0.7f,  -0.5f,  0.0f,   // Top of pyramid (point 6) - Blue
	 0.5f,  0.0f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 0.0f,      0.7f,  -0.5f,  0.0f,   // Bottom-right of front triangle (point 7) - Blue
	 0.5f,  0.0f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f,   1.0f, 0.0f,      0.7f,  -0.5f,  0.0f,   // Back-right of square base (point 8) - Blue
	//---------end of right side tri ----------------------------------------------------------------
	 0.0f, -0.7f,  0.0f,  1.0f, 0.5f, 0.0f, 1.0f,   0.5f, 1.0f,      0.0f,  -0.5f, -0.7f,   // Top of pyramid (point 9) - Orange
	 0.5f,  0.0f, -0.5f,  1.0f, 0.5f, 0.0f, 1.0f,   0.0f, 0.0f,      0.0f,  -0.5f, -0.7f,   // Back-right of square base (point 10) - Orange
	-0.5f,  0.0f, -0.5f,  1.0f, 0.5f, 0.0f, 1.0f,   1.0f, 0.0f,      0.0f,  -0.5f, -0.7f,   // Back-left of square base (point 11) - Orange
	//---------end of back side tri -----------------------------------------------------------------
	-0.5f,  0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f,   0.0f, 1.0f,      0.0f,   1.0f,  0.0f,   // Front-left of square base (point 12) - Green 
	-0.5f,  0.0f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,   0.0f, 0.0f,      0.0f,   1.0f,  0.0f,   // Back-left of square base (point 13) - Green
	 0.5f,  0.0f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,      0.0f,   1.0f,  0.0f,   // Back-right of square base (point 14) - Green
	 0.5f,  0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 1.0f,      0.0f,   1.0f,  0.0f    // Front-right of square base(point 15) - Green
	 //--------end of bottom square -------------------------------------------------------------

};

GLfloat planeVerts[] =
{
	// Plane Coordinates and colors

	// vertex pos			// colors		       // texture coord   // Normal Coord
	-5.0f, 0.0f,  5.0f,   0.0f, 0.2f, 0.2f, 1.0f,   0.0f, 0.0f,      0.0f,   1.0f,  0.0f, // Front-left corner (point 0) - Dark Cyan
	-5.0f, 0.0f, -5.0f,   0.0f, 0.2f, 0.2f, 1.0f,   0.0f, 1.0f,      0.0f,   1.0f,  0.0f, // Back-left corner (point 1) - Dark Cyan
	 5.0f, 0.0f, -5.0f,   0.0f, 0.2f, 0.2f, 1.0f,   1.0f, 1.0f,      0.0f,   1.0f,  0.0f, // Back-right corner (point 2) - Dark Cyan
	 5.0f, 0.0f,  5.0f,   0.0f, 0.2f, 0.2f, 1.0f,   1.0f, 0.0f,      0.0f,   1.0f,  0.0f // Front-right corner (point 3) - Dark Cyan
};

// coordinate variables for the icosahedron
const float x = 0.6180339887498948483063079070103f;
const float y = 1.0f;

GLfloat icosahedronVerts[] =
{

	// Icosahedron Coordinates and colors
	
	// vertex pos			// colors		        // texture coord        // Normal Coord
	 //-------- Tri 1 -------------------------------------------------------------------------------------------------------
	   -x, 0.0f,    y,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 0.0f,         0.0f,  0.47f,  1.24f, //  0
	 0.0f,    y,    x,    0.0f, 0.2f, 0.2f, 1.0f,    0.5f, 1.0f,         0.0f,  0.47f,  1.24f, //  4
	    x, 0.0f,    y,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 0.0f,         0.0f,  0.47f,  1.24f, //  1
	 //-------- Tri 2 -------------------------------------------------------------------------------------------------------
	   -x, 0.0f,    y,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 0.0f,       -0.76f,  0.76f,  0.76f, //  0
	   -y,    x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 0.0f,       -0.76f,  0.76f,  0.76f, //  9
	 0.0f,    y,    x,    0.0f, 0.2f, 0.2f, 1.0f,    0.5f, 1.0f,       -0.76f,  0.76f,  0.76f, //  4
	 //-------- Tri 3 -------------------------------------------------------------------------------------------------------
	   -y,    x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 0.0f,       -0.47f,  1.24f,   0.0f, //  9
	 0.0f,    y,   -x,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 0.0f,       -0.47f,  1.24f,   0.0f, //  5
	 0.0f,    y,    x,    0.0f, 0.2f, 0.2f, 1.0f,    0.5f, 1.0f,       -0.47f,  1.24f,   0.0f, //  4
	 //-------- Tri 4 -------------------------------------------------------------------------------------------------------
	 0.0f,    y,    x,    0.0f, 0.2f, 0.2f, 1.0f,    0.5f, 1.0f,        0.47f,  1.24f,   0.0f, //  4
	 0.0f,    y,   -x,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 0.0f,        0.47f,  1.24f,   0.0f, //  5
	    y,    x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 0.0f,        0.47f,  1.24f,   0.0f, //  8
	 //-------- Tri 5 -------------------------------------------------------------------------------------------------------
	 0.0f,    y,    x,    0.0f, 0.2f, 0.2f, 1.0f,    0.5f, 1.0f,        0.76f,  0.76f,  0.76f, //  4
		y,    x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 0.0f,        0.76f,  0.76f,  0.76f, //  8
	    x, 0.0f,    y,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 0.0f,        0.76f,  0.76f,  0.76f, //  1
	 //-------- Tri 6 -------------------------------------------------------------------------------------------------------
		y,    x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 1.0f,        1.24f,   0.0f,  0.47f, //  8
		y,   -x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    0.5f, 0.0f,        1.24f,   0.0f,  0.47f, // 10
	    x, 0.0f,    y,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 1.0f,        1.24f,   0.0f,  0.47f, //  1
	 //-------- Tri 7 -------------------------------------------------------------------------------------------------------
	    y,    x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    0.5f, 1.0f,        1.24f,   0.0f, -0.47f, //  8
		x, 0.0f,   -y,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 0.0f,        1.24f,   0.0f, -0.47f, //  3
	    y,   -x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 0.0f,        1.24f,   0.0f, -0.47f, // 10
	 //-------- Tri 8 -------------------------------------------------------------------------------------------------------
	 0.0f,    y,   -x,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 1.0f,        0.76f,  0.76f, -0.76f, //  5
	    x, 0.0f,   -y,    0.0f, 0.2f, 0.2f, 1.0f,    0.5f, 0.0f,        0.76f,  0.76f, -0.76f, //  3
	    y,    x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 1.0f,        0.76f,  0.76f, -0.76f, //  8
	 //-------- Tri 9 -------------------------------------------------------------------------------------------------------
	 0.0f,    y,   -x,    0.0f, 0.2f, 0.2f, 1.0f,    0.5f, 1.0f,         0.0f,  0.47f, -1.24f, //  5
	   -x, 0.0f,   -y,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 0.0f,         0.0f,  0.47f, -1.24f, //  2
	    x, 0.0f,   -y,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 0.0f,         0.0f,  0.47f, -1.24f, //  3
	 //-------- Tri 10 ------------------------------------------------------------------------------------------------------
	   -x, 0.0f,   -y,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 1.0f,         0.0f, -0.47f, -1.24f, //  2
	 0.0f,   -y,   -x,    0.0f, 0.2f, 0.2f, 1.0f,    0.5f, 0.0f,         0.0f, -0.47f, -1.24f, //  7
	    x, 0.0f,   -y,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 1.0f,         0.0f, -0.47f, -1.24f, //  3
	 //-------- Tri 11 ------------------------------------------------------------------------------------------------------
	 0.0f,   -y,   -x,    0.0f, 0.2f, 0.2f, 1.0f,    0.5f, 0.0f,        0.76f, -0.76f, -0.76f, //  7
	    y,   -x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 1.0f,        0.76f, -0.76f, -0.76f, // 10
	    x, 0.0f,   -y,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 1.0f,        0.76f, -0.76f, -0.76f, //  3
	 //-------- Tri 12 ------------------------------------------------------------------------------------------------------
	 0.0f,   -y,   -x,    0.0f, 0.2f, 0.2f, 1.0f,    0.5f, 0.0f,        0.47f, -1.24f,   0.0f, //  7
	 0.0f,   -y,    x,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 1.0f,        0.47f, -1.24f,   0.0f, //  6
	    y,   -x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 1.0f,        0.47f, -1.24f,   0.0f, // 10
	 //-------- Tri 13 ------------------------------------------------------------------------------------------------------
	 0.0f,   -y,   -x,    0.0f, 0.2f, 0.2f, 1.0f,    0.5f, 0.0f,       -0.47f, -1.24f,   0.0f, //  7
	   -y,   -x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 1.0f,       -0.47f, -1.24f,   0.0f, // 11
	 0.0f,   -y,    x,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 1.0f,       -0.47f, -1.24f,   0.0f, //  6
	 //-------- Tri 14 ------------------------------------------------------------------------------------------------------
	   -y,   -x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 0.0f,       -0.76f, -0.76f,  0.76f, // 11
	   -x, 0.0f,    y,    0.0f, 0.2f, 0.2f, 1.0f,    0.5f, 1.0f,       -0.76f, -0.76f,  0.76f, //  0
	 0.0f,   -y,    x,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 0.0f,       -0.76f, -0.76f,  0.76f, //  6
	 //-------- Tri 15 ------------------------------------------------------------------------------------------------------
	   -x, 0.0f,    y,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 1.0f,         0.0f, -0.47f,  1.24f, //  0
	    x, 0.0f,    y,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 1.0f,         0.0f, -0.47f,  1.24f, //  1
	 0.0f,   -y,    x,    0.0f, 0.2f, 0.2f, 1.0f,    0.5f, 0.0f,         0.0f, -0.47f,  1.24f, //  6
	 //-------- Tri 16 ------------------------------------------------------------------------------------------------------
	 0.0f,   -y,    x,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 0.0f,        0.76f, -0.76f,  0.76f, //  6
	    x, 0.0f,    y,    0.0f, 0.2f, 0.2f, 1.0f,    0.5f, 1.0f,        0.76f, -0.76f,  0.76f, //  1
		y,   -x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 0.0f,        0.76f, -0.76f,  0.76f, // 10
	 //-------- Tri 17 ------------------------------------------------------------------------------------------------------
	   -y,    x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 1.0f,       -1.24f,   0.0f,  0.47f, //  9
	   -x, 0.0f,    y,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 1.0f,       -1.24f,   0.0f,  0.47f, //  0
	   -y,   -x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    0.5f, 0.0f,       -1.24f,   0.0f,  0.47f, // 11
	 //-------- Tri 18 ------------------------------------------------------------------------------------------------------
	   -y,    x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    0.5f, 1.0f,       -1.24f,   0.0f, -0.47f, //  9
	   -y,   -x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 0.0f,       -1.24f,   0.0f, -0.47f, // 11
	   -x, 0.0f,   -y,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 0.0f,       -1.24f,   0.0f, -0.47f, //  2
	 //-------- Tri 19 ------------------------------------------------------------------------------------------------------
	   -y,    x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 1.0f,       -0.76f,  0.76f, -0.76f, //  9
	   -x, 0.0f,   -y,    0.0f, 0.2f, 0.2f, 1.0f,    0.5f, 0.0f,       -0.76f,  0.76f, -0.76f, //  2
	 0.0f,    y,   -x,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 1.0f,       -0.76f,  0.76f, -0.76f, //  5
	 //-------- Tri 20 ------------------------------------------------------------------------------------------------------
	 0.0f,   -y,   -x,    0.0f, 0.2f, 0.2f, 1.0f,    0.5f, 0.0f,       -0.76f, -0.76f, -0.76f, //  7
	   -x, 0.0f,   -y,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 1.0f,       -0.76f, -0.76f, -0.76f, //  2
	   -y,   -x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 1.0f,       -0.76f, -0.76f, -0.76f, // 11
	 //-------- End of Tris -------------------------------------------------------------------------------------------------


	//-x, 0.0f,  y,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 0.0f,       0.0f, 1.0f, 0.0f, //  0
	// x, 0.0f,  y,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 1.0f,       0.0f, 1.0f, 0.0f, //  1
	//-x, 0.0f, -y,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 1.0f,       0.0f, 1.0f, 0.0f, //  2
	// x, 0.0f, -y,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 0.0f,       0.0f, 1.0f, 0.0f, //  3

	//0.0f,  y,  x,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 0.0f,       0.0f, 1.0f, 0.0f, //  4
	//0.0f,  y, -x,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 1.0f,       0.0f, 1.0f, 0.0f, //  5
	//0.0f, -y,  x,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 1.0f,       0.0f, 1.0f, 0.0f, //  6
	//0.0f, -y, -x,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 0.0f,       0.0f, 1.0f, 0.0f, //  7

	// y,  x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 0.0f,       0.0f, 1.0f, 0.0f, //  8
	//-y,  x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 1.0f,       0.0f, 1.0f, 0.0f, //  9
	// y, -x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 1.0f,       0.0f, 1.0f, 0.0f, // 10
	//-y, -x, 0.0f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 0.0f,       0.0f, 1.0f, 0.0f  // 11

};

GLfloat cubeVerts[] =
{
	// Cube Coordinates and colors

	// vertex pos			// colors		        // texture coord        // Normal Coord
	 //-------- Tri 1 -------------------------------------------------------------------------------------------------------
	-0.5f,  0.5f,  0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 1.0f,         0.0f,  0.0f,  0.0f, //  0 Front face
	-0.5f, -0.5f,  0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 0.0f,         0.0f,  0.0f,  0.0f, //  1 Front face
	 0.5f, -0.5f,  0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 0.0f,         0.0f,  0.0f,  0.0f, //  2 Front face
	 0.5f,  0.5f,  0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 1.0f,         0.0f,  0.0f,  0.0f, //  3 Front face
	 //-------- Tri 2 -------------------------------------------------------------------------------------------------------
	-0.5f,  0.5f, -0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 1.0f,         0.0f,  0.0f,  0.0f, //  4 Left face
	-0.5f, -0.5f, -0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 0.0f,         0.0f,  0.0f,  0.0f, //  5 Left face
	-0.5f, -0.5f,  0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 0.0f,         0.0f,  0.0f,  0.0f, //  6 Left face
    -0.5f,  0.5f,  0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 1.0f,         0.0f,  0.0f,  0.0f, //  7 Left face
	 //-------- Tri 2 -------------------------------------------------------------------------------------------------------
	 0.5f,  0.5f, -0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 1.0f,         0.0f,  0.47f,  1.24f, //  8 Back face
	 0.5f, -0.5f, -0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 0.0f,         0.0f,  0.47f,  1.24f, //  9 Back face
	-0.5f, -0.5f, -0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 0.0f,         0.0f,  0.47f,  1.24f, //  10 Back face
	-0.5f,  0.5f, -0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 1.0f,         0.0f,  0.47f,  1.24f, //  11 Back face
	 //-------- Tri 2 -------------------------------------------------------------------------------------------------------
	 0.5f,  0.5f,  0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 1.0f,         0.0f,  0.47f,  1.24f, //  12 Right face
	 0.5f, -0.5f,  0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 0.0f,         0.0f,  0.47f,  1.24f, //  13 Right face
	 0.5f, -0.5f, -0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 0.0f,         0.0f,  0.47f,  1.24f, //  14 Right face
     0.5f,  0.5f, -0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 1.0f,         0.0f,  0.47f,  1.24f, //  15 Right face
	 //-------- Tri 2 -------------------------------------------------------------------------------------------------------
	-0.5f,  0.5f,  0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 0.0f,         0.0f,  0.47f,  1.24f, //  16 Top face
	-0.5f,  0.5f, -0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 1.0f,         0.0f,  0.47f,  1.24f, //  17 Top face
	 0.5f,  0.5f, -0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 1.0f,         0.0f,  0.47f,  1.24f, //  18 Top face
	 0.5f,  0.5f,  0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 0.0f,         0.0f,  0.47f,  1.24f, //  19 Top face
	 //-------- Tri 2 -------------------------------------------------------------------------------------------------------
	 0.5f, -0.5f, -0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 0.0f,         0.0f,  0.47f,  1.24f, //  20 Bottom face
	 0.5f, -0.5f,  0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    1.0f, 1.0f,         0.0f,  0.47f,  1.24f, //  21 Bottom face
	-0.5f, -0.5f,  0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 1.0f,         0.0f,  0.47f,  1.24f, //  22 Bottom face
	-0.5f, -0.5f, -0.5f,    0.0f, 0.2f, 0.2f, 1.0f,    0.0f, 0.0f,         0.0f,  0.47f,  1.24f  //  23 Bottom face
};
//##########################################################################################################################

// main loop or the game loop
int main(int argc, char* argv[])
{
	//rotation degrees variables in radians
	float alphaD8 = 55.0f * (PI / 180.0);
	float alphaD20 = 21.25f * (PI / 180);
	float alphaCube = 0.0f;
	float alphaSphere = 1.0f * (PI / 180);
	//cout << alpha << endl;

	//// code for calculating normals and printing it on screen
	//// used to get normals and to test the CrossProduct function
	//float* norm;
	//float a[3] = { 0.0f,   -y,   -x };
	//float b[3] = { -x, 0.0f,   -y };
	//float c[3] = { -y,   -x, 0.0f };
	//norm = CrossProduct(a, b, c);
	//for (int i = 0; i < 3; i++)
	//{
	//	printf("%lf\n", norm[i]);
	//}


	if (!UInitialize(argc, argv, &gWindow))
	{
		return EXIT_FAILURE;
	}

	
	sizeVerts = sizeof(verts);
	sizeVerts2 = sizeof(verts2);
	planeSizeVerts = sizeof(planeVerts);
	icoSizeVerts = sizeof(icosahedronVerts);
	cubeSizeVerts = sizeof(cubeVerts);


	//create the pyramid objects
	Pyramid pyramid1(gWindow, verts, sizeVerts);
	Pyramid pyramid2(gWindow, verts2, sizeVerts2);
	Pyramid pyramid3(gWindow, verts, sizeVerts);

	// create the plane objects
	Plane plane(gWindow, planeVerts, planeSizeVerts);

	// create the icosahedron object
	Icosahedron ico(gWindow, icosahedronVerts, icoSizeVerts);

	//create cube object
	Cube cube(gWindow, cubeVerts, cubeSizeVerts);

	// sphere object
	Sphere sphere(15);

	// start shader
	if (!createShader())
	{
		return EXIT_FAILURE;
	}

	// set the background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// texture data location
	//-----------------------------------------------------------------------------------------------------
	// Textures located here
	// test.png is made by me as a test teture for the plane,
	// whereas the granite and marble textures were found at cgbookcase.com run by Dorian Zgraggen
	// the site says all textures are free to use
	//-----------------------------------------------------------------------------------------------------
	const char* texFile = "images/plane.png";
	const char* texFile2 = "images/marble.png";
	const char* texFile3 = "images/zero_texture.png";
	const char* texFile4 = "images/GreenPlastic.png";
	const char* texFile5 = "images/RandomBlue.png";
	const char* texFile6 = "images/Lemon.png";

	//######################################################################################################
	// load in the textures
	//######################################################################################################
	// attempt to load texture 1 if it fails return an error
	if (!loadTexture(texFile, texture1Id))
	{
		cout << "Failed to load texture!" << texFile << endl;
		return EXIT_FAILURE;
	}

	// attempt to load texture 2 if it fails return an error
	if (!loadTexture(texFile2, texture2Id))
	{
		cout << "Failed to load texture!" << texFile2 << endl;
		return EXIT_FAILURE;
	}

	// attempt to load texture 3 if it fails return an error
	if (!loadTexture(texFile3, zeroNumTextureId))
	{
		cout << "Failed to load texture!" << texFile3 << endl;
		return EXIT_FAILURE;
	}

	// attempt to load texture 4 if it fails return an error
	if (!loadTexture(texFile4, greenPlasticTextureId))
	{
		cout << "Failed to load texture!" << texFile4 << endl;
		return EXIT_FAILURE;
	}

	// attempt to load texture 5 if it fails return an error
	if (!loadTexture(texFile5, blueTextureId))
	{
		cout << "Failed to load texture!" << texFile5 << endl;
		return EXIT_FAILURE;
	}

	// attempt to load texture 5 if it fails return an error
	if (!loadTexture(texFile6, lemonTextureId))
	{
		cout << "Failed to load texture!" << texFile6 << endl;
		return EXIT_FAILURE;
	}
	//######################################################################################################
	// end loading in the textures
	//######################################################################################################


	// tell it what shader to use
	glUseProgram(getProgramId());
	// Set the texture units
	glUniform1i(glGetUniformLocation(getProgramId(), "Texture1"), 0);
	glUniform1i(glGetUniformLocation(getProgramId(), "uTextureBase"), 1);
	glUniform1i(glGetUniformLocation(getProgramId(), "uTextureExtra"), 2);
	

	// ----------------------------------------------------------------------------------------------------
	//end texture section
	// ----------------------------------------------------------------------------------------------------

	// loop that renders the screen
	while (!glfwWindowShouldClose(gWindow))
	{
		// find delta time
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// process input
		UProcessInput(gWindow);

		// Enable z-depth
		glEnable(GL_DEPTH_TEST);

		// clear the screen might have to remove when objects are added
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//------------------------------------------------------------------------------------
		// update the camera
		mainCam.updateCamera();


		// test light with rotations
		//alphaD8 += 0.01;
		//alphaD20 += 0.01;
		//alphaSphere += 0.01;

		// render the pyramid & plane
		render(pyramid1, alphaD8, mainCam, texture2Id, zeroNumTextureId);
		render(pyramid2, alphaD8, mainCam, texture2Id, zeroNumTextureId);
		render(pyramid3, 0.0f, mainCam, greenPlasticTextureId);
		render(plane, 0.0f, mainCam, texture1Id);
		render(ico, alphaD20, mainCam, greenPlasticTextureId);
		render(cube, alphaCube, mainCam, blueTextureId);
		render(sphere, alphaSphere, mainCam, lemonTextureId);
		//------------------------------------------------------------------------------------

		// swap buffers after drawing objects
		glfwSwapBuffers(gWindow);

		glfwPollEvents();

	}

	// delete textures
	deleteTexture(texture1Id);
	deleteTexture(texture2Id);
	deleteTexture(zeroNumTextureId);
	deleteTexture(greenPlasticTextureId);
	deleteTexture(blueTextureId);
	deleteTexture(lemonTextureId);

	// exits program
	exit(EXIT_SUCCESS);
}

//Make a new window and initialize GLFW & GLEW
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
	// GLFW: init and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// for apple computers
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// create window
	* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (*window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(*window);
	glfwSetFramebufferSizeCallback(*window, UResizeWindow);
	glfwSetScrollCallback(*window, UMouseScrollCallback);
	glfwSetMouseButtonCallback(*window, UMouseButtonCallback);
	glfwSetCursorPosCallback(*window, UMousePositionCallback);
	glfwSetKeyCallback(gWindow, singleKeyCallback);

	// initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum GlewInitResult = glewInit();

	// if GLEW errors return false and display error
	if (GLEW_OK != GlewInitResult)
	{
		cerr << glewGetErrorString(GlewInitResult) << endl;
		return false;
	}

	// display OpenGl version that the GPU uses
	cout << "OpenGL Version GPU Uses: " << glGetString(GL_VERSION) << endl;
	return true;
}

// process keyboard input from the user
void UProcessInput(GLFWwindow* window)
{
	// local variable that stores if a key has been pressed
	bool keypress = false;

	// stores the movement with respect to deltatime
	// slight loss of data converting over to float, but the accuracy is plenty at float level
	float cameraOffset = moveSpeed * deltaTime;

	// if the user presses esc then close the window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	// user pressed w
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		//cout << "You pressed W! ";
		//keypress = true;
		mainCam.translateCamera(cameraOffset, 'w');
	}

	//user pressed s
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		//cout << "You pressed S! ";
		//keypress = true;
		mainCam.translateCamera(cameraOffset, 's');
	}

	// user pressed a
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		//cout << "You pressed A! ";
		//keypress = true;
		mainCam.translateCamera(cameraOffset, 'a');
	}

	// user pressed d
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		//cout << "You pressed D! ";
		//keypress = true;
		mainCam.translateCamera(cameraOffset, 'd');
	}

	// user pressed q
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		//cout << "You pressed Q! ";
		//keypress = true;
		mainCam.translateCamera(cameraOffset, 'q');
	}

	// user pressed e
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		//cout << "You pressed E! ";
		//keypress = true;
		mainCam.translateCamera(cameraOffset, 'e');
	}

	// if user pressed a key get mouse position
	if (keypress)
	{
		//double x, y;
		//glfwGetCursorPos(window, &x, &y);
		//cout << "Cursor at position (" << x << ", " << y << ")" << endl;
	}
}

// only reports a callback once so it should stop the key from being spammed on one press
// is only used for the p key at the moment movement keys remain in the other check, because
// we want to check for them and keep moving if they remain pressed
void singleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_P) && action == GLFW_PRESS)
	{
		cout << "You pressed P! " << endl;
		mainCam.setOrthoCheck();
	}
}

// call this function when the window size is altered
void UResizeWindow(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// call this function when the mouse moves
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	// if this is the first time the mouse is onscreen set variables
	if (gFirstMouse)
	{
		gLastX = xpos;
		gLastY = ypos;
		gFirstMouse = false;
	}

	// find the offset from the last position of the mouse
	double xoffset = xpos - gLastX;
	double yoffset = gLastY - ypos;

	// set the last position to the current position
	gLastX = xpos;
	gLastY = ypos;

	// pass the offsets the camera object
	mainCam.mouseMovement(xoffset, yoffset);
}

// call this function when the mouse wheel is used
// minimum speed is currently 1.5
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	// cout for debugging
	//cout << "Mouse wheel at (" << xoffset << ", " << yoffset << ")" << endl;

	// if scroll wheel is moved up increase move speed
	if (yoffset > 0)
	{
		moveSpeed += 0.5f;
	}

	// else if scroll wheel is moved down decrease move speed
	else if (yoffset < 0)
	{
		// check if move speed is greater than 1.5
		if (moveSpeed > 1.5)
		{
			moveSpeed -= 0.5f;
		}
	}

	// float math is not 100% accurate so check if the float is less than 1.5 and set it to 1.5 if it is
	if (moveSpeed < 1.5f)
	{
		moveSpeed = 1.5f;
	}
	//debug print statement that shows move speed
	cout << moveSpeed << endl;
}

// call this function when the mouse buttons are used
// currently it is only useful for debugging, but in the future it will be useful
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	switch (button)
	{
		case GLFW_MOUSE_BUTTON_LEFT:
		{
			if (action == GLFW_PRESS)
			{
				cout << "Left mouse button pressed" << endl;
				
			}

			else
			{
				cout << "Left mouse button released" << endl;
			}
		}
		break;

		case GLFW_MOUSE_BUTTON_MIDDLE:
		{
			if (action == GLFW_PRESS)
			{
				cout << "Middle mouse button pressed" << endl;
				
			}
			else
			{
				cout << "Middle mouse button released" << endl;
			}
		}
		break;

		case GLFW_MOUSE_BUTTON_RIGHT:
		{
			if (action == GLFW_PRESS)
			{
				cout << "Right mouse button pressed" << endl;
				
			}
			else
			{
				cout << "Right mouse button released" << endl;
			}
		}
		break;

		default:
			cout << "Unhandled mouse button event" << endl;
			break;
	}
}

// calculate the cross product to find the normal vector for the tri given
// I used this to find the normal vector and hardcoded it into vertexes
float* CrossProduct(float a[3], float b[3], float c[3])
{
	// find u and v, which are the two vectors for the tri
	float u[3] =
	{ 
		-(b[0] - a[0]),
		-(b[1] - a[1]),
		-(b[2] - a[2])
	};
	float v[3] =
	{
		(c[0] - a[0]),
		(c[1] - a[1]),
		(c[2] - a[2])
	};

	// calculate the cross product and return it
	float result[3] =
	{
		((u[1] * v[2]) - (u[2] * v[1])),
		((u[2] * v[0]) - (u[0] * v[2])),
		((u[0] * v[1]) - (u[1] * v[0]))
	};
	return result;
}
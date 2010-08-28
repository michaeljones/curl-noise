
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <unistd.h>

#include <iostream>

#include <cmath>

#include <curlnoise/curlnoise.h>

#define ESCAPE 27
#define KEY_Q 938

/* The number of our GLUT window */
int window; 

const int particleCount = 10000;
curlnoise::Float2 positions[ particleCount ];

void InitGL(int Width, int Height)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// This Will Clear The Background Color To Black
	glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);				// The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
	glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();				// Reset The Projection Matrix
	
	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window
	
	glMatrixMode(GL_MODELVIEW);
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height)
{
	if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
		Height=1;

	glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */
void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View

	srand48( 0 );

	perlin::NoiseFactory noiseFactory;
	curlnoise::CurlFactory curlFactory( noiseFactory );

	std::vector< curlnoise::Line* > boundaries;

	boundaries.push_back( new curlnoise::Line( curlnoise::Float2( 0.0f, 3.0f ), curlnoise::Float2( 0.0f, -5.0f ) ) );
	boundaries.push_back( new curlnoise::Line( curlnoise::Float2( -10.0f, 10.0f ), curlnoise::Float2( 0.0f, -20.0f ) ) );
	boundaries.push_back( new curlnoise::Line( curlnoise::Float2( -10.0f, 3.0f ), curlnoise::Float2( 10.0f, 0.0f ) ) );

	curlnoise::Noise2D* curlnoise = curlFactory.create2D( boundaries );

	static float z=0;

	z += 0.01f;

	glBegin( GL_LINES );

	glVertex3f( 0.0f, 3.0f, -20.0f );
	glVertex3f( 0.0f, -2.0f, -20.0f );

	glVertex3f( -10.0f, 10.0f, -20.0f );
	glVertex3f( -10.0f, -10.0f, -20.0f );

	glVertex3f( -10.0f, 3.0f, -20.0f );
	glVertex3f( 0.0f, 3.0f, -20.0f );

	/*
	for ( int i=-50; i<50; ++i )
	{
		for ( int j=-50; j<50; ++j )
		{
			float posX = i/10.0f;
			float posY = j/10.0f;

			curlnoise::Float2 vel = curlnoise->generate( posX, posY );

			glColor3f( 0.0f, 0.0f, 0.0f );

			glVertex3f( posX, posY, -20.0f );

			glColor3f( 1.0f, 1.0f, 1.0f );

			float vm = 10.0f;
			glVertex3f( posX + vel.x * vm, posY + vel.y * vm, -20.0f );
		}
	}
	*/

	glEnd();

	glBegin( GL_LINES );

	for ( int i=0; i<particleCount; ++i )
	{
		glVertex3f( positions[ i ].x, positions[ i ].y, -20.0f );

		curlnoise::Float2 vel = curlnoise->generate( positions[i].x, positions[i].y );

		positions[ i ].x += vel.x;
		positions[ i ].y += vel.y;

		glVertex3f( positions[ i ].x, positions[ i ].y, -20.0f );
	}

	glEnd();

	glBegin( GL_POINTS );

	for ( int i=0; i<particleCount; ++i )
	{
		glVertex3f( positions[ i ].x, positions[ i ].y, -20.0f );
	}

	glEnd();

	delete curlnoise;


	glutSwapBuffers();
}

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y) 
{
	/* avoid thrashing this procedure */
	usleep(10);
	
	/* If escape is pressed, kill everything. */
	if ( key == ESCAPE || key == KEY_Q ) 
	{ 
		/* shut down our window */
		glutDestroyWindow(window); 
		
		/* exit the program...normal termination. */
		exit(0);									 
	}
}

int main(int argc, char **argv) 
{	

	for ( int i=0; i<particleCount; ++i )
	{
		positions[ i ].x = ( drand48() - 0.5f ) * 20.0f;
		positions[ i ].y = ( drand48() - 0.5f ) * 20.0f;
	}


	/* Initialize GLUT state - glut will take any command line arguments that pertain to it or 
		 X Windows - look at its documentation at http://reality.sgi.com/mjk/spec3/spec3.html */	
	glutInit(&argc, argv);	

	/* Select type of Display mode:	 
		 Double buffer 
		 RGBA color
		 Alpha components supported 
		 Depth buffer */	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);	

	/* get a 640 x 480 window */
	glutInitWindowSize(640, 480);	

	/* the window starts at the upper left corner of the screen */
	glutInitWindowPosition(0, 0);	

	/* Open a window */	
	window = glutCreateWindow("Curl Noise");	

	/* Register the function to do all our OpenGL drawing. */
	glutDisplayFunc(&DrawGLScene);	

	/* Go fullscreen.	This is as soon as possible. */
	glutFullScreen();

	/* Even if there are no events, redraw our gl scene. */
	glutIdleFunc(&DrawGLScene);

	/* Register the function called when our window is resized. */
	glutReshapeFunc(&ReSizeGLScene);

	/* Register the function called when the keyboard is pressed. */
	glutKeyboardFunc(&keyPressed);

	/* Initialize our window. */
	InitGL(640, 480);
	
	/* Start Event Processing Engine */	
	glutMainLoop();	

	return 1;
}


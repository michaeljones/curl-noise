
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <unistd.h>

#include <iostream>

#include <cmath>

#include <perlin/perlin.h>
#include <perlin/fractal.h>

#define ESCAPE 27
#define KEY_Q 938

/* The number of our GLUT window */
int window; 

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
	perlin::Noise3D* noise = noiseFactory.create3D();

	static float z=0;

	z += 0.01f;

	glBegin( GL_LINES );

    for ( int i=-50; i<50; ++i )
    {
        for ( int j=-50; j<50; ++j )
        {
			float posX = i/10.0f;
			float posY = j/10.0f;

			float rxp = fabs( 5.0f - posX );
			float rxn = fabs( -5.0f - posX );

			float ryp = fabs( 5.0f - posY );
			float ryn = fabs( -5.0f - posY );

			float rx = std::min( rxp, rxn );
			float ry = std::min( ryp, ryn );

			float r = std::min( rx, ry );

			r /= 4.0f;
			float scale = 1.0f;

			if ( r < 1.0f ) scale = (15.0f/8.0f) * r - ( 10.0f/8.0f ) * r * r * r + ( 3.0f/8.0f ) * r * r * r * r * r;

			float dy = scale * ( noise->generate( posX, posY + 0.01, z ) - noise->generate( posX, posY - 0.01, z ) );
			float dx = scale * ( noise->generate( posX + 0.01, posY, z ) - noise->generate( posX - 0.01, posY, z ) );

			float velX = dy;
			float velY = -dx;

			float vm = 20.0f;

			glColor3f( 0.0f, 0.0f, 0.0f );

			glVertex3f( posX, posY, -20.0f );

			glColor3f( 1.0f, 1.0f, 1.0f );

			glVertex3f( posX + velX * vm, posY + velY * vm, -20.0f );
        }
    }

    delete noise;

	glEnd();

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
	window = glutCreateWindow("Jeff Molofee's GL Code Tutorial ... NeHe '99");	

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


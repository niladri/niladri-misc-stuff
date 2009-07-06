#include <GLES/gl.h>
#include <EGL/egl.h>

#include <stdio.h>
#include <time.h>

GLfloat gRot = 0.0;

GLfloat gVertices[] = { 0.0, 0.5, 0.0, 
						-0.5, -0.5, 0.0,
						0.5, -0.5, 0.0,
						0.0, 0.5, 0.0,
						-0.5, -0.5, 0.0,
						0.0, 0.0, -0.5,
						0.0, 0.5, 0.0,
						0.0, 0.0, -0.5,
						0.5, -0.5, 0.0,
						0.0, 0.0, -0.5,
						-0.5, -0.5, 0.0,
						0.5, -0.5, 0.0};

GLubyte gIndices[] = { 	0, 1, 2,
						0, 1, 3,
						0, 3, 2,
						3, 1, 2};

GLfloat gColors[] = { 	0.0, 0.0, 1.0, 1.0, 	0.0, 1.0, 0.0, 1.0, 	1.0, 0.0, 0.0, 1.0,
						0.0, 1.0, 0.0, 1.0, 	0.0, 0.0, 1.0, 1.0,  	1.0, 0.0, 0.0, 1.0,
						1.0, 0.0, 0.0, 1.0, 	0.0, 0.0, 1.0, 1.0,  	0.0, 1.0, 0.0, 1.0,
						0.0, 1.0, 0.0, 1.0, 	1.0, 0.0, 0.0, 1.0, 	0.0, 0.0, 1.0, 1.0};

int main(int argc, char** argv)
{
	EGLint iMajorVersion, iMinorVersion;
	EGLint iConfigAttribs[16];
	int iConfigs;
	EGLDisplay eglDisplay = 0;
	EGLConfig eglConfig = 0;
	EGLSurface eglSurface = 0;
	EGLContext eglContext = 0;
	int i;
	
	eglDisplay = eglGetDisplay(NULL);

	if( !eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion) )
	{
		printf("eglInitialize() failed!\n");
		return 1;
	}

	iConfigAttribs[0] = EGL_RED_SIZE;
	iConfigAttribs[1] = 8;
	iConfigAttribs[2] = EGL_GREEN_SIZE;
	iConfigAttribs[3] = 8;
	iConfigAttribs[4] = EGL_BLUE_SIZE;
	iConfigAttribs[5] = 8;
	iConfigAttribs[6] = EGL_ALPHA_SIZE;
	iConfigAttribs[7] = 8;
	iConfigAttribs[8] = EGL_SURFACE_TYPE;
	iConfigAttribs[9] = EGL_WINDOW_BIT;
	iConfigAttribs[10] = EGL_NONE;

	if( !(eglChooseConfig(eglDisplay, iConfigAttribs, &eglConfig, 1, &iConfigs)))
	{
		printf("eglChooseConfig() failed!\n");
		return 1;
	}

	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, (EGLNativeWindowType)NULL, NULL);
	if(eglSurface == EGL_NO_SURFACE)
	{
		printf("Can't create EGL Window Surface\n");
	}
	
	eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, NULL);
	if(!eglContext)
	{
		printf("Can't create context\n");
	}
	
	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);

	glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	eglSwapBuffers(eglDisplay, eglSurface);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	eglSwapBuffers(eglDisplay, eglSurface);

	glMatrixMode(GL_MODELVIEW);
	
	for(i=0; i<36000; i++)
	{
		glLoadIdentity();
		glRotatef(gRot, 1.0, 1.0, 1.0);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, gVertices);

		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, 0, gColors);
	
		glDrawArrays(GL_TRIANGLES, 0, 12);

		eglSwapBuffers(eglDisplay, eglSurface);
	
		gRot += 2;

		/* Calculate FPS */
		{
			static long t0 = -1;
			static int frames = 0;
			long t = 0;
			time(&t);

			if (t0<0)
				t0 = t;

			frames++;

			if(t-t0 > 5.0)
			{
				GLfloat seconds = t-t0;
				GLfloat fps = frames/seconds;
				printf("%d frames in %3.1f seconds = %3.3f FPS\n", frames, seconds, fps);

				t0 = t;
				frames = 0;
			}
		}
	}
	
	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglTerminate(eglDisplay);

	return 0;
}

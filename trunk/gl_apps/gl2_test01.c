#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include <stdio.h>

GLuint gProgramObject;

// Create a shader object, load the shader source,
// and compile the shader
//
GLuint LoadShader(GLenum type, const char *shaderSrc)
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader(type);

	if (shader == 0)
		return 0;
	
	// Load the shader source
	glShaderSource(shader, 1, &shaderSrc, NULL);

	// Compile the shader
	glCompileShader(shader);

	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printf("glCompileShader failed!\n");
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

// Initialize the shader and program object
//
int Init()
{
	GLbyte vShaderStr[] = 
		"attribute vec4 vPosition; 		\n"
		"void main() 					\n"
		"{ 								\n"
		"	gl_Position = vPosition; 	\n"
		"} 								\n";

	GLbyte fShaderStr[] = 
		"precision mediump float; 						\n"
		"void main() 									\n"
		"{ 												\n"
		"	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); 	\n"
		"} 												\n";

	GLuint vertexShader;
	GLuint fragmentShader;
	GLint linked;

	printf("Init+\n");

	// Load the vertex/fragment shaders
	vertexShader = LoadShader(GL_VERTEX_SHADER, (char*)vShaderStr);
	fragmentShader = LoadShader(GL_FRAGMENT_SHADER, (char*)fShaderStr);

	// Create the program object
	gProgramObject = glCreateProgram();

	if (gProgramObject == 0)
	{
		printf("glCreateProgram failed!\n");
		return 0;
	}

	glAttachShader(gProgramObject, vertexShader);
	glAttachShader(gProgramObject, fragmentShader);

	// Bind vPosition to attribute 0
	glBindAttribLocation(gProgramObject, 0, "vPosition");

	// Link the program
	glLinkProgram(gProgramObject);

	// Check the link status
	glGetProgramiv(gProgramObject, GL_LINK_STATUS, &linked);
	if(!linked)
	{
		printf("glLinkProgram failed!\n");
		glDeleteProgram(gProgramObject);
		return 0;
	}

	printf("Init-\n");

	// Clear the color buffer
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	return 1;
}

// Draw a triangle using the shader pair
//
void Draw()
{
	GLfloat vVertices[] = { 	0.0, 0.5, 0.0,
								-0.5, -0.5, 0.0,
								0.5, -0.5, 0.0 };

	// Use the program object
	glUseProgram(gProgramObject);

	// Load the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(int argc, char *argv[])
{
	EGLint iMajorVersion, iMinorVersion;
	EGLint iConfigAttribs[16];
	EGLint iContextAttribs[4];
	int iConfigs;
	EGLDisplay eglDisplay = 0;
	EGLConfig eglConfig = 0;
	EGLSurface eglSurface = 0;
	EGLContext eglContext = 0;
	int i=0;
	
	eglDisplay = eglGetDisplay(NULL);

	if ( !eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion) )
	{
		printf("eglInitialize failed!\n");
		return 1;
	}

	iConfigAttribs[i++] = EGL_RENDERABLE_TYPE;
	iConfigAttribs[i++] = EGL_OPENGL_ES2_BIT;
	iConfigAttribs[i++] = EGL_RED_SIZE;
	iConfigAttribs[i++] = 8;
	iConfigAttribs[i++] = EGL_GREEN_SIZE;
	iConfigAttribs[i++] = 8;
	iConfigAttribs[i++] = EGL_BLUE_SIZE;
	iConfigAttribs[i++] = 8;
	iConfigAttribs[i++] = EGL_ALPHA_SIZE;
	iConfigAttribs[i++] = 8;
	iConfigAttribs[i++] = EGL_SURFACE_TYPE;
	iConfigAttribs[i++] = EGL_WINDOW_BIT;
	iConfigAttribs[i++] = EGL_NONE;

	i=0;
	iContextAttribs[i++] = EGL_CONTEXT_CLIENT_VERSION;
	iContextAttribs[i++] = 2;
	iContextAttribs[i++] = EGL_NONE;

	if ( !(eglChooseConfig(eglDisplay, iConfigAttribs, &eglConfig, 1, &iConfigs)) )
	{
		printf("eglChooseConfig failed!\n");
		return 1;
	}

	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, (EGLNativeWindowType)NULL, NULL);
	if(eglSurface == EGL_NO_SURFACE)
	{
		printf("Can't create EGL Window Surface\n");
	}

	eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, iContextAttribs);
	if(!eglContext)
	{
		printf("Can't create context\n");
	}

	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);

	Init();
	eglSwapBuffers(eglDisplay, eglSurface);
	Draw();
	eglSwapBuffers(eglDisplay, eglSurface);

	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglTerminate(eglDisplay);

	return 0;
}

// Texture01.c
//
//    This is a texture example that draws a quad with a 2D
//    texture image divided in 9 rects of different sizes.
//	  Requires esUtil lib ( OpenGL(R) ES 2.0 Programming Guide )
//
#include <stdlib.h>
#include "esUtil.h"

typedef struct
{
   // Handle to a program object
   GLuint programObject;

   // Attribute locations
   GLint  positionLoc;
   GLint  texCoordLoc;

   // Sampler location
   GLint samplerLoc;

   // Texture handle
   GLuint textureId;

} UserData;

///
// Create a simple 2x2 texture image with four different colors
//
GLuint CreateSimpleTexture2D( )
{
   // Texture object handle
   GLuint textureId;
   
   int width, height;
   char* fileName = "image.tga";
   char *buffer = esLoadTGA ( fileName, &width, &height );

   if ( buffer == NULL )
   {
      esLogMessage ( "Error loading (%s) image.\n", fileName );
      return 0;
   }

   // Use tightly packed data
   glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

   // Generate a texture object
   glGenTextures ( 1, &textureId );

   // Bind the texture object
   glBindTexture ( GL_TEXTURE_2D, textureId );

   // Load the texture
   glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer );

   // Set the filtering mode
   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

   return textureId;

}


///
// Initialize the shader and program object
//
int Init ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   GLbyte vShaderStr[] =  
      "attribute vec4 a_position;   \n"
      "attribute vec2 a_texCoord;   \n"
      "varying vec2 v_texCoord;     \n"
      "void main()                  \n"
      "{                            \n"
      "   gl_Position = a_position; \n"
      "   v_texCoord = a_texCoord;  \n"
      "}                            \n";
   
   GLbyte fShaderStr[] =  
      "precision mediump float;                            \n"
      "varying vec2 v_texCoord;                            \n"
      "uniform sampler2D s_texture;                        \n"
      "void main()                                         \n"
      "{                                                   \n"
      "  gl_FragColor = texture2D( s_texture, v_texCoord );\n"
      "}                                                   \n";

   // Load the shaders and get a linked program object
   userData->programObject = esLoadProgram ( vShaderStr, fShaderStr );

   // Get the attribute locations
   userData->positionLoc = glGetAttribLocation ( userData->programObject, "a_position" );
   userData->texCoordLoc = glGetAttribLocation ( userData->programObject, "a_texCoord" );
   
   // Get the sampler location
   userData->samplerLoc = glGetUniformLocation ( userData->programObject, "s_texture" );

   // Load the texture
   userData->textureId = CreateSimpleTexture2D ();

   glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
   return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

   GLfloat vVertices[] = { -0.8f,  0.8f, 0.0f,  // Position 0
                            0.00f,  0.00f,        // TexCoord 0 
                           -0.7f,  0.8f, 0.0f,  // Position 1
                            0.33f,  0.00f,        // TexCoord 1
                            0.7f,  0.8f, 0.0f,  // Position 2
                            0.67f,  0.00f,        // TexCoord 2
                            0.8f,  0.8f, 0.0f,  // Position 3
                            1.00f,  0.00f,        // TexCoord 3
                           -0.8f,  0.7f, 0.0f,  // Position 4
                            0.00f,  0.33f,        // TexCoord 4
                           -0.7f,  0.7f, 0.0f,  // Position 5
                            0.33f,  0.33f,        // TexCoord 5
                            0.7f,  0.7f, 0.0f,  // Position 6
                            0.67f,  0.33f,        // TexCoord 6
                            0.8f,  0.7f, 0.0f,  // Position 7
                            1.00f,  0.33f,        // TexCoord 7
                           -0.8f, -0.7f, 0.0f,  // Position 8
                            0.00f,  0.67f,        // TexCoord 8
                           -0.7f, -0.7f, 0.0f,  // Position 9
                            0.33f,  0.67f,        // TexCoord 9
                            0.7f, -0.7f, 0.0f,  // Position 10
                            0.67f,  0.67f,        // TexCoord 10
                            0.8f, -0.7f, 0.0f,  // Position 11
                            1.00f,  0.67f,        // TexCoord 11
                           -0.8f, -0.8f, 0.0f,  // Position 12
                            0.00f,  1.00f,        // TexCoord 12
                           -0.7f, -0.8f, 0.0f,  // Position 13
                            0.33f,  1.00f,        // TexCoord 13
                            0.7f, -0.8f, 0.0f,  // Position 14
                            0.67f,  1.00f,        // TexCoord 14
                            0.8f, -0.8f, 0.0f,  // Position 15
                            1.00f,  1.00f         // TexCoord 15
                         };

   GLushort indices1[] = { 4, 0, 5, 1, 6, 2, 7, 3 };
   GLushort indices2[] = { 8, 4, 9, 5, 10, 6, 11, 7 };
   GLushort indices3[] = { 12, 8, 13, 9, 14, 10, 15, 11 };
      
   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );
   
   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData->programObject );

   // Load the vertex position
   glVertexAttribPointer ( userData->positionLoc, 3, GL_FLOAT, 
                           GL_FALSE, 5 * sizeof(GLfloat), vVertices );
   // Load the texture coordinate
   glVertexAttribPointer ( userData->texCoordLoc, 2, GL_FLOAT,
                           GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3] );

   glEnableVertexAttribArray ( userData->positionLoc );
   glEnableVertexAttribArray ( userData->texCoordLoc );

   // Bind the texture
   glActiveTexture ( GL_TEXTURE0 );
   glBindTexture ( GL_TEXTURE_2D, userData->textureId );

   // Set the sampler texture unit to 0
   glUniform1i ( userData->samplerLoc, 0 );

   glDrawElements ( GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, indices1 );
   glDrawElements ( GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, indices2 );
   glDrawElements ( GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, indices3 );

   eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

///
// Cleanup
//
void ShutDown ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

   // Delete texture object
   glDeleteTextures ( 1, &userData->textureId );

   // Delete program object
   glDeleteProgram ( userData->programObject );
}


int main ( int argc, char *argv[] )
{
   ESContext esContext;
   UserData  userData;

   esInitContext ( &esContext );
   esContext.userData = &userData;

   esCreateWindow ( &esContext, "Simple Texture 2D", 320, 240, ES_WINDOW_RGB );
   
   if ( !Init ( &esContext ) )
      return 0;

   esRegisterDrawFunc ( &esContext, Draw );
   
   esMainLoop ( &esContext );

   ShutDown ( &esContext );
}


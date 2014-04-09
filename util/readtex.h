/* readtex.h */

#ifndef READTEX_H
#define READTEX_H

#include <GL/glut.h>
#include <GL/gl.h>

/* RGB Image Structure */

typedef struct _TK_RGBImageRec {
  GLint sizeX, sizeY;
  GLint components;
  GLenum format;
  unsigned char *data;
} RGBImage;

extern RGBImage* LoadRGB(const char *imageFile);

#endif

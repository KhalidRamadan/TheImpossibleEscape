#ifndef LAS
#define LAS

#include <bits/stdc++.h>
#include <GLUT/glut.h>
#include <FreeImage.h>

using namespace std;
void drawImg(double x0,double y0,double x1,double y1, bool r, GLuint textureID);
GLuint loadTexture(string filenameString, GLenum minificationFilter = GL_LINEAR, GLenum magnificationFilter = GL_LINEAR);
vector<GLuint> loadAllImg();
#endif

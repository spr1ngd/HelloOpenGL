#pragma once
#include <stdio.h>
#include <functional>
#include <windows.h>
#include <gl/GL.h>

unsigned char* LoadFileContent(const char*filePath);

GLuint CaptureScreen(int width,int height,std::function<void()>foo);

void SaveScreenPixel(int width,int height,std::function<void()>foo,const char*filePath);

GLuint GenerateProceduralTexture(int size);

// random util
float randf();
float srandf();
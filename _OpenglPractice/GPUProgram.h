#pragma once
#include <windows.h>
#include <gl/GL.h>
#include <vector>
#include <string>

using namespace std;

namespace spring 
{
	class GPUProgram
	{
	private:
		GLuint programId;
		std::vector<string, GLuint> locations;
	public:
		GPUProgram createProgram();
		GLuint getLocation(string name);
	};
}
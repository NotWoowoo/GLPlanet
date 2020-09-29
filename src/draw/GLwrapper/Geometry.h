#pragma once
#include <initializer_list>

class Geometry
{
public:
	enum Format {
		fmtFloat,
		fmtVec2f,
		fmtVec3f,
		fmtUint
	};
	
	Geometry(std::initializer_list<Format> format, bool dynamicDraw, int dataSize =  sizeof(float) * 9 );
	~Geometry();

	void bind();
	
private:
	unsigned int VBO = 0;
	unsigned int VAO = 0;
};


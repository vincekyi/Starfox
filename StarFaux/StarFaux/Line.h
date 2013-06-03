#ifndef _LINE_H_
#define _LINE_H_

#include "Shape.h"

class Line : public Shape {
public: 
    Line(GLuint program, double length);
	void draw(Camera& camera, Light* light, lightEffects effects);
	void resetPosition();
private:
    void generateLine(double length);
};

#endif

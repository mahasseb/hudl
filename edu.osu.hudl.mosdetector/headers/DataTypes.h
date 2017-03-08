/*
 * DataTypes.h
 *
 *  Created on: Apr 15, 2012
 *      Author: behrooz
 */

#ifndef DATATYPES_H_
#define DATATYPES_H_
#include "CommonLibs.h"
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
using namespace std;
using namespace cv;

namespace vision {
class Boundary {
public:
	int topLeftX;
	int topLeftY;
	int bottomRightX;
	int bottomRightY;
	Boundary();
	virtual ~Boundary();
};
class Line {
public:
	float rho;
	float theta;
	double a;
	float b;
	Line(Point point1, Point point2);
	Line(float rho, float theta);
	virtual ~Line();
};
class Window {
public:
	Mat image;
	/**
	 * Centers of the window
	 */
	float x;
	float y;
	/**
	 * Boundaries
	 */
	float topLeftX;
	float topLeftY;
	float width;
	float height;
	bool valid;
	float score;
	/**
	 * Row and Column Index
	 */
	int row;
	int column;
	Window();
	Window(float topLeftX, float topLeftY, float width, float height);
	virtual ~Window();
	void clone(Window window);
	void audit();
};

class Tube {
public:
	/**
	 * Centers of the window
	 */
	float x;
	float y;
	/**
	 * Boundaries
	 */
	int id;
	float startT;
	float topLeftX;
	float topLeftY;
	float width;
	float height;
	float duration;
	bool valid;
	float score;
	Tube();
	virtual ~Tube();
};

class File {
public:
	string name;
	string type;
	string parrent;
	string fullpath;
	File();
	virtual ~File();
};

class Interval {
public:
	float start;
	float end;

	Interval();
	virtual ~Interval();
};

class Track {
public:
	int trackId;
	int startFrame;
	int endFrame;
	vector<Point2d> points;
	vector<Point3d> points3D;
	float trackLength;
	float normalizedTrackLength;
	void process();
	Track();
	virtual ~Track();
};

class OpticalflowResult {
public:
	Mat flow;
	vector<Mat> flows;
	vector<float> flowOrientationHistogram;
	OpticalflowResult();
	virtual ~OpticalflowResult();
};
} /* namespace vision */
#endif /* DATATYPES_H_ */

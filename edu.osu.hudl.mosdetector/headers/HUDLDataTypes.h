/*
 * HUDLDataTypes.h
 *
 *  Created on: Apr 24, 2012
 *      Author: behrooz
 */

#ifndef HUDLDATATYPES_H_
#define HUDLDATATYPES_H_
#include "DataTypes.h"
using namespace vision;
namespace hudl {
class LineOfScrimmage {
public:
	Window originalWindow;
	Mat transformationMatrix;
	Mat inverseTransfromMatrix;
	vector<Point> points;
	vector<Point> leftPoints;
	vector<Point> rightPoints;
	Point point1, point2;
	float distance;
	float foregroundDifference;
	LineOfScrimmage();
	virtual ~LineOfScrimmage();
};
class HUDLWindow: public Window {
public:
	int degree;
	Mat opticalflow;
	double value;
	float* vector;
	bool containsPlayer;
	HUDLWindow();
	virtual ~HUDLWindow();

};

class VideoInterval: public Interval {
public:
	bool motion;
	VideoInterval();
	virtual ~VideoInterval();
};
class Frame {
public:
	vector<HUDLWindow> windows;
	vector<float> flowOrientationHistogram;
	Mat image;
	Frame();
	virtual ~Frame();
};
} // namespace hudl

#endif /* HUDLDATATYPES_H_ */

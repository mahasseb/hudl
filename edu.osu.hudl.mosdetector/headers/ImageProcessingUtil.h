/*
 * ImageProcessingUtil.h
 *
 *  Created on: Mar 18, 2012
 *      Author: behrooz
 */

#ifndef IMAGEPROCESSINGUTIL_H_
#define HISTOGRAM_TYPE_RED_COLOR  1
#define HISTOGRAM_TYPE_GREEN_COLOR  2
#define HISTOGRAM_TYPE_BLUE_COLOR  3
#define HISTOGRAM_TYPE_RGB_COLOR  4
#define HISTOGRAM_TYPE_GRAY_COLOR  5

#define CHI_SQUARE_DISTANCE 1
#define EUCLIDEAN_DISTANCE 2
#define IMAGEPROCESSINGUTIL_H_
/**
 * OpenCV libs
 */
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include "DataTypes.h"
using namespace cv;

namespace vision {
struct RGB {
	ushort red;
	ushort green;
	ushort blue;
};
enum GradientType {
	x, y, xy
};

enum Color {
	red, green, blue
};

class ImageProcessingUtil {
private:
	const static int precision = 1000;
	static void calculateGrayScaleHistogram(Mat image, float * histogram,
			int histSize, vector<Point> boundaryPoints, Point topLeft,
			Point bottomRight, Mat maskFrame, RGB color);
	static void calculateRGBHistogram(Mat image, float * histogram,
			int histSize, vector<Point> boundaryPoints, Point topLeft,
			Point bottomRight, Mat maskFrame, RGB color);

public:
	ImageProcessingUtil();
	virtual ~ImageProcessingUtil();
	static void setColor(Mat image, int x, int y, RGB rgb);
	static RGB getColor(Mat image, int x, int y);
	static short getGrayColor(Mat image, int x, int y);
	static Mat extractColorImage(Mat image, Color color);
	static Mat rotate(const Mat& frame, float angle);
	static Mat resize(Mat image, Size size);
	static Mat resize(Mat image, float scale);
	static Mat convertToGrayscale(Mat image);
	static Mat convertToGrayscale(Mat image, int range);
	static Mat convertToBlackWhite(Mat image, int treshold = 100);
	static Mat blur(Mat image, Size size);
	static Mat calculateImageGradient(Mat image, GradientType gradientType);
	static Mat crop(Mat image, Point topLeft, Point bottomRight);
	static Mat crop(Mat image, int topLeftX, int topLeftY, int bottomRightX,
			int bottomRightY);
	static float calculateAverageIntensity(Mat image);
	static Boundary fitRectangle(Mat image);
	static void drawTrapazoid(Mat image, vector<Point> boundaryPoints,
			RGB color);
	static void drawTrapazoid(Mat image, Point *boundaryPoints, RGB color);
	static void drawLine(Mat image, Line line);
	static float calculateBackgroundPercentage(Mat image,
			vector<Point> boundaryPoints, Point topLeft, Point bottomRight,
			RGB backgroundColor);
	static void calculateHistogram(Mat image, float * histogram, int type,
			int histSize);

	static void calculateHistogram(Mat image, float * histogram, int type,
			int histSize, vector<Point> boundaryPoints, Point topLeft,
			Point bottomRight);
	static void calculateHistogram(Mat image, float * histogram1,
			float * histogram2, int type, int histSize, Line line, int yMargin);

	static void calculateHistogram(Mat image, float * histogram, int type,
			int histSize, vector<Point> boundaryPoints, Point topLeft,
			Point bottomRight, Mat maskFrame, RGB color);

	static double calculateHistogramDistance(float* histogram1,
			float* histogram2, int size, int type);

	static float* summerizeHistogram(float *histogram, int oldSize,
			int newSize);
	static float* summerizeHistogram(vector<float> histogram, int newSize);

	static CvScalar calculateBackgroundColor(Mat frame);
	static void cutVideo(string inputVideo, string outputVideo, Point topLeft,
			Point bottomRight);
	static Mat createCubicView(vector<Mat> images, int xShift, int yShift);
	static void createVideo(string folder, string outputVideoName,
			Size newSize = Size(-1, -1));
}
;

} /* namespace vision */
#endif /* IMAGEPROCESSINGUTIL_H_ */

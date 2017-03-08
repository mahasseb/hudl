/*
 * ComputerVisionUtil.h
 *
 *  Created on: Mar 17, 2012
 *      Author: behrooz
 */

#ifndef COMPUTERVISIONUTIL_H_
#define COMPUTERVISIONUTIL_H_
/**
 * OpenCV libs
 */
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include "DataTypes.h"
#include "ImageProcessingUtil.h"
using namespace cv;

namespace vision {
struct WindowGenerationState {
	// Initialization Parameters
	int width;
	int height;
	int xShift;
	int yShift;
	Size minWindowSize;
	Size maxWindowSize;
	float xResizeScale;
	float yResizeScale;
	float topLeftXMargin;
	float topLeftYMargin;
	float bottomRightXMargin;
	float bottomRightYMargin;

	//State Parameters
	int y, x;
	float widthScale, heightScale;
};

struct TubeGenerationState {
	// Initialization Parameters
	int width;
	int height;
	int duration;
	int xShift;
	int yShift;
	int tShift;
	Size minWindowSize;
	Size maxWindowSize;
	int minimumDuration;
	int maximumDuration;
	float xResizeScale;
	float yResizeScale;
	float tResizeScale;
	float topLeftXMargin;
	float topLeftYMargin;
	float bottomRightXMargin;
	float bottomRightYMargin;
	int startTMargin;
	int endTMargin;
	//State Parameters
	int y, x, t;
	float widthScale, heightScale, durationScale;
};
class ComputerVisionUtil {
private:
	WindowGenerationState windowGenerationState;
	TubeGenerationState tubeGenerationState;
public:
	ComputerVisionUtil();
	virtual ~ComputerVisionUtil();

	static Point transformPoint(Point point, Mat transformationMatrix);
	static Point rotatePoint(Point point, Mat rotationMatrix);
	static void drawOpticalFlowMap(const Mat& flow, Mat& cflowmap,
			const Scalar& color, float threshold1, float threshold2);

	static void drawOpticalFlowMap(const Mat& flow, Mat& cflowmap, int step,
			const Scalar& color);
	static void drawOpticalFlowMap(const Mat& flow, Mat& cflowmap, int step,
			const Scalar& color, Window window);

	static vector<Mat> calculateOpticalFlow(Mat previousFrame, Mat currentFrame,
			vector<Window> windows);
	static OpticalflowResult calculateOpticalFlow(Mat previousFrame,
			Mat currentFrame, vector<Window> windows, int numberOfAngleBins,
			int numberOfRadiusBins);

	static Mat calculateOpticalFlow(Mat previousFrame, Mat currentFrame);
	static vector<Track> calculateKLT(File file, int numberOfFeatures,
			int endFrame);
	static vector<Track> calculateKLT(File file, int numberOfFeatures);
	static vector<float> calculateHOGDescriptor(Mat image);
	static vector<float> calculateHOGDescriptor(Mat image, vector<Point> points,
			Size window = Size(64, 64));
	static float computeOverlap(Rect rect1, Rect rect2);
	void initializeScanningWindow(int widht, int height, int xShift, int yShift,
			Size minWindowSize = Size(64, 64),
			Size maxWindowSize = Size(128, 128), float xResizeScale = 10,
			float yResizeScale = 10, float topLeftXMargin = 0,
			float topLeftYMargin = 0, float bottomRightXMargin = 0,
			float bottomRightYMargin = 0);
	Window nextWindow();
	void initializeScanningTube(int width, int height, int duration, int xShift,
			int yShift, int tShift, Size minWindowSize = Size(64, 64),
			Size maxWindowSize = Size(128, 128), int minimumDuration = 8,
			int maximumDuration = 32, float xResizeScale = 10,
			float yResizeScale = 10, float tResizeScale = 10,
			float topLeftXMargin = 0, float topLeftYMargin = 0,
			float bottomRightXMargin = 0, float bottomRightYMargin = 0,
			int startTMargin = 0, int endTMargin = 0);
	Tube nextTube();

	static vector<Window> reticulate(Mat image, int rows, int columns);

};

} /* namespace vision */
#endif /* COMPUTERVISIONUTIL_H_ */

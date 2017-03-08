/*
 * SnapRecognizer.h
 *
 *  Created on: Apr 15, 2012
 *      Author: behrooz
 */

#ifndef SNAPRECOGNIZER_H_
#define SNAPRECOGNIZER_H_
#include "DataTypes.h"
#include "CommonUtil.h"
#include "ComputerVisionUtil.h"
#include "HUDLDataTypes.h"
#define NUMBER_OF_CELLS_IN_ROW 10
#define NUMBER_OF_CELLS_IN_COLUMN 10
#define NUMBER_OF_ALGORITHMS 1
#define WINDOW_SIZE 5
using namespace vision;

struct Element {
	int frame;
	float averageDistance;
	float maximumDistance;
	float distance;
	float score;
	double numberOfActivationsBeforeTime;
	double numberOfActivationsAfterTime;
};
struct LocalMaxima {
	int frame;
	float threshold;
	double distance;
	double averageDistance;
	double numberOfActivationsBeforeTime;
	double numberOfActivationsAfterTime;
	double sumOfOpticalflowBefore;
	double sumOfOpticalflowAfter;
	double motionMinusT1;
	double motionPlusT1;
	double motionMinusT2;
	double motionPlusT2;
	double motionMinusT3;
	double motionPlusT3;
	double motionInFrameMinusT1Before;
	double motionInFrameMinusT1After;
	double motionInFramePlusT1Before;
	double motionInFramePlusT1After;
	double motionInFrameMinusT2Before;
	double motionInFrameMinusT2After;
	double motionInFramePlusT2Before;
	double motionInFramePlusT2After;
};
namespace hudl {
class MomentOfSnapDetector {
private:
	File file;
	VideoCapture capture;
	vector<Window> grid;
	vector<HUDLWindow> windows;
	vector<Frame> frames;
	vector<Track> tracks;
	int width, height;
	float maxTrackLength;
	float minTrackLength;
	int videoLength;
	int NUMBER_OF_FRAMES;
public:
	//int frameCount;
	MomentOfSnapDetector();
	virtual ~MomentOfSnapDetector();
	void init(File file);
//	vector<LocalMaxima> pruneLocalMaximums(vector<LocalMaxima> localMaximums,
//			float threshold);

	void createGrid();
	void generateOpticalflow(int frameGap, int downSample = 1);
	void showAverageVectorForCells();
	vector<int> processMotion(int videoLength);
	Mat generateKLTFeatures(int numberOfBins);
	bool hasCameraMotion(int frame, int threshold, float distanceThreshold);
	void findMomentOfSnap(string output, string profile, bool useCamera,
			int frameGap, int downSample = 1, int scanningWindowSize = 50);
	void findMomentOfSnapBaseLine(string output, string profile);

	static int getMomentOfSnap(File file);
	static vector<HUDLWindow> getActiveCells(File file);
	bool containsPlayers(Mat frame, Window window);
	bool containsPlayers(Frame frame, int windowIndex);
	vector<Track> getTracks();
	void calculateSumOfMagnitude(Mat flow, float*** analysis,
			float lowerThreshold, float upperThreshold, int algorithmIndex,
			int frameIndex, int blockIndex);
	float calculateSumOfMagnitude(Mat flow);
	float* calculateSumVector(Mat flow, float*** analysis, float lowerThreshold,
			float upperThreshold, int algorithmIndex, int frameIndex,
			int blockIndex);
}
;

} /* namespace vision */
#endif /* SNAPRECOGNIZER_H_ */

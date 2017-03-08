/*
 * MomentOfSnapDetector.cpp
 *  Created on: Apr 15, 2012
 *      Author: behrooz
 */

#include "MathUtil.h"
#include "MomentOfSnapDetector.h"
#include <map>
namespace hudl {

MomentOfSnapDetector::MomentOfSnapDetector() {
	// TODO Auto-generated constructor stub
	maxTrackLength = FLT_MIN;
	minTrackLength = FLT_MAX;
}

MomentOfSnapDetector::~MomentOfSnapDetector() {
	// TODO Auto-generated destructor stub
}
void MomentOfSnapDetector::init(File file) {
	this->grid.clear();
	this->windows.clear();
	this->frames.clear();
	this->file = file;
	if (this->capture.isOpened())
		this->capture.release();
	capture = VideoCapture(file.fullpath);
	NUMBER_OF_FRAMES = capture.get(CV_CAP_PROP_FRAME_COUNT);
}

vector<Track> MomentOfSnapDetector::getTracks() {
	return tracks;
}
void MomentOfSnapDetector::createGrid() {
}

float MomentOfSnapDetector::calculateSumOfMagnitude(Mat flow) {
	float sum = 0;
	for (int y = 0; y < flow.rows; y += 1) {
		for (int x = 0; x < flow.cols; x += 1) {
			const Point2f& fxy = flow.at<Point2f>(y, x);
			if (fxy.y <= fxy.x) {
				float magnitude = sqrt(pow(fxy.x, 2) + pow(fxy.y, 2));
				sum += magnitude;
			}
		}
	}
	return sum;
}
void MomentOfSnapDetector::calculateSumOfMagnitude(Mat flow, float*** analysis,
		float lowerThreshold, float upperThreshold, int algorithmIndex,
		int frameIndex, int blockIndex) {
	float sum = 0;
	for (int y = 0; y < flow.rows; y += 1) {
		for (int x = 0; x < flow.cols; x += 1) {
			const Point2f& fxy = flow.at<Point2f>(y, x);
			if (fxy.y <= fxy.x) {
				float magnitude = sqrt(pow(fxy.x, 2) + pow(fxy.y, 2));
				if ((lowerThreshold == -1 && upperThreshold == -1)
						|| (magnitude > lowerThreshold
								&& magnitude < upperThreshold))
					sum += magnitude;
			}
		}
	}
	analysis[algorithmIndex - 1][frameIndex - 1][blockIndex] = sum;
}
float* MomentOfSnapDetector::calculateSumVector(Mat flow, float*** analysis,
		float lowerThreshold, float upperThreshold, int algorithmIndex,
		int frameIndex, int blockIndex) {
	float* vector = new float[2];
	vector[0] = 0;
	vector[1] = 0;
	for (int y = 0; y < flow.rows; y += 1) {
		for (int x = 0; x < flow.cols; x += 1) {
			const Point2f& fxy = flow.at<Point2f>(y, x);
			vector[0] += fxy.x;
			vector[1] += fxy.y;
		}
	}
	vector[0] = vector[0] / (flow.rows * flow.cols);
	vector[1] = vector[1] / (flow.rows * flow.cols);
	vector[0] = vector[0] * 10;
	vector[1] = vector[1] * 10;
	return vector;
}

bool isInField(Window window, int width, int height) {
	float coeeficient1 = 0.2;
	float coeeficient2 = 0.3;
	if (window.topLeftX + window.width < coeeficient1 * width
			|| window.topLeftY + window.height < coeeficient2 * height
			|| window.topLeftX > (1 - coeeficient1) * width
			|| window.topLeftY > (1 - coeeficient2) * height)
		return false;
	return true;
}

void MomentOfSnapDetector::generateOpticalflow(int frameGap, int downSample) {
	Mat currentFrame, previousFrame;
	int frameIndex = -1;
	for (;;) {
		frameIndex++;
		if (frameIndex == 0) {
			capture >> previousFrame;
			if (previousFrame.empty())
				break;
			if (downSample != 1)
				previousFrame = ImageProcessingUtil::resize(previousFrame,
						Size(previousFrame.cols / downSample,
								previousFrame.rows / downSample));
			for (int i = 0; i < NUMBER_OF_CELLS_IN_COLUMN; i++) {
				for (int j = 0; j < NUMBER_OF_CELLS_IN_ROW; j++) {
					Window window;
					window.topLeftY = i
							* (previousFrame.rows / NUMBER_OF_CELLS_IN_COLUMN);
					window.topLeftX = j
							* (previousFrame.cols / NUMBER_OF_CELLS_IN_ROW);
					window.height = previousFrame.rows
							/ NUMBER_OF_CELLS_IN_COLUMN;
					window.width = previousFrame.cols / NUMBER_OF_CELLS_IN_ROW;
					window.row = j;
					window.column = i;
					HUDLWindow hudlWindow;
					hudlWindow.clone(window);
					grid.push_back(window);
				}
			}
			this->width = previousFrame.cols;
			this->height = previousFrame.rows;
		} else {
			Frame frame;
			for (unsigned int gridIndex = 0; gridIndex < grid.size();
					gridIndex++) {
				HUDLWindow hudlWindow;
				hudlWindow.clone(grid[gridIndex]);
				hudlWindow.containsPlayer = true;
				frame.windows.push_back(hudlWindow);

			}
			capture >> currentFrame;
			if (currentFrame.empty())
				break;

			if (frameIndex % frameGap != 0)
				continue;
			if (downSample != 1)
				currentFrame = ImageProcessingUtil::resize(currentFrame,
						Size(currentFrame.cols / downSample,
								currentFrame.rows / downSample));
			vector<Mat> opticalFlows;
			opticalFlows = ComputerVisionUtil::calculateOpticalFlow(
					previousFrame, currentFrame, grid);
			for (unsigned int i = 0; i < frame.windows.size(); i++) {
				opticalFlows[i].copyTo(frame.windows[i].opticalflow);
			}
			frames.push_back(frame);
			currentFrame.copyTo(previousFrame);
		}
	}
	previousFrame.release();
	currentFrame.release();
	videoLength = frameIndex;
}
void MomentOfSnapDetector::showAverageVectorForCells() {
	Mat currentFrame, previousFrame;
	int frameIndex = -1;
	float *** analysis;
	analysis = new float**[NUMBER_OF_ALGORITHMS];
	for (int i = 0; i < NUMBER_OF_ALGORITHMS; i++) {
		analysis[i] = new float*[NUMBER_OF_FRAMES];
	}
//	Field field;
	bool flag = true;
	for (; flag == true;) {
		frameIndex++;
		if (frameIndex == 0) {
			capture >> previousFrame;
			cout << (file.name + "." + file.type).c_str() << '\n';
			if (previousFrame.empty())
				break;
			for (int i = 0; i < NUMBER_OF_CELLS_IN_COLUMN; i++) {
				for (int j = 0; j < NUMBER_OF_CELLS_IN_ROW; j++) {
					Window window;
					window.topLeftY = i
							* (previousFrame.rows / NUMBER_OF_CELLS_IN_COLUMN);
					window.topLeftX = j
							* (previousFrame.cols / NUMBER_OF_CELLS_IN_ROW);
					window.height = previousFrame.rows
							/ NUMBER_OF_CELLS_IN_COLUMN;
					window.width = previousFrame.cols / NUMBER_OF_CELLS_IN_ROW;
					window.x = window.topLeftX + window.width / 2;
					window.y = window.topLeftY + window.height / 2;

					window.row = j;
					window.column = i;
					HUDLWindow hudlWindow;
					hudlWindow.clone(window);
					grid.push_back(window);
					windows.push_back(hudlWindow);
				}
			}
			for (int i = 0; i < NUMBER_OF_ALGORITHMS; i++) {
				for (int j = 0; j < NUMBER_OF_FRAMES; j++) {
					analysis[i][j] = new float[windows.size()];
					for (unsigned int k = 0; k < windows.size(); k++) {
						analysis[i][j][k] = -1;
					}
				}
			}
		} else {
			capture >> currentFrame;

			if (currentFrame.empty())
				break;
			vector<Mat> opticalFlows;
			opticalFlows = ComputerVisionUtil::calculateOpticalFlow(
					previousFrame, currentFrame, grid);

			float minimumValue = numeric_limits<float>::max();
			float maximumValue = numeric_limits<float>::min();
			float averageValue = 0;
			int index = 0;
			for (unsigned int i = 0; i < windows.size(); i++) {
				opticalFlows[i].copyTo(windows[i].opticalflow);
				calculateSumOfMagnitude(windows[i].opticalflow, analysis, -1,
						-1, 1, frameIndex, i);
				windows[i].vector = calculateSumVector(windows[i].opticalflow,
						analysis, -1, -1, 1, frameIndex, i);

				windows[i].value = analysis[0][frameIndex - 1][i];
				if (windows[i].value < 500)
					continue;
				windows[i].degree = 0;
				if (windows[i].value < minimumValue) {
					minimumValue = windows[i].value;
				}
				if (windows[i].value > maximumValue) {
					maximumValue = windows[i].value;
				}
				index++;
				averageValue += windows[i].value;
			}
			averageValue = averageValue / index;

			for (unsigned int i = 0; i < windows.size(); i++) {
				windows[i].valid = true;

			}

			for (unsigned int i = 0; i < windows.size(); i++) {
				rectangle(previousFrame,
						Point(windows[i].topLeftX, windows[i].topLeftY),
						Point(windows[i].topLeftX + windows[i].width,
								windows[i].topLeftY + windows[i].height),
						CV_RGB(255, 255, 0));

				line(previousFrame, Point(windows[i].x, windows[i].y),
						Point(windows[i].x + windows[i].vector[0],
								windows[i].y + windows[i].vector[1]),
						CV_RGB(255, 0, 255), 1, 1, 0);
			}
			if (frameIndex <= 2)
				cvWaitKey();
			imshow("Frame", previousFrame);
			cvWaitKey(10);
			currentFrame.copyTo(previousFrame);
		}

	}
}

int MomentOfSnapDetector::getMomentOfSnap(File file) {
	string fileName = "outputs/output_";
	fileName.append(file.name);
	fileName.append(".log");
	ifstream input;
	input.open(fileName.c_str());
	if (!input.good()) {
		cout << "Error openning " << fileName.c_str() << '\n';
		return -1;
	}
	string stringLine;
	getline(input, stringLine);
	int momentOfSnap = atoi(stringLine.c_str());
	return momentOfSnap;
}

vector<HUDLWindow> MomentOfSnapDetector::getActiveCells(File file) {
	vector<HUDLWindow> cells;
	string fileName = "outputs/output_";
	fileName.append(file.name);
	fileName.append(".log");
	ifstream input;
	input.open(fileName.c_str());
	if (!input.good()) {
		cout << "Error openning " << fileName << '\n';
		return cells;
	}
	string stringLine;
	getline(input, stringLine);
	while (input.good()) {
		getline(input, stringLine);
		char* pch = strtok((char*) stringLine.c_str(), ",");
		int index = 0;
		HUDLWindow hudlWindow;
		while (pch != NULL) {
			if (index == 0) {
				hudlWindow.topLeftX = atof(pch);
			} else if (index == 1) {
				hudlWindow.topLeftY = atof(pch);
			} else if (index == 2) {
				hudlWindow.width = atof(pch);
			} else if (index == 3) {
				hudlWindow.height = atof(pch);
			}
			pch = strtok(NULL, ",");
			index = (++index) % 4;
		}
		hudlWindow.x = hudlWindow.topLeftX + hudlWindow.width / 2;
		hudlWindow.y = hudlWindow.topLeftY + hudlWindow.height / 2;
		cells.push_back(hudlWindow);
	}
	return cells;
}
void MomentOfSnapDetector::findMomentOfSnapBaseLine(string output,
		string profile) {
	Mat frame, previousFrame, currentFrame;
	if (!capture.isOpened())
		cout << "Error!!!\n";
	int frameIndex = 0;
	vector<float> motions;
	for (;;) {
		if (frameIndex == 0) {
			capture >> previousFrame;
			if (previousFrame.empty())
				break;
			frameIndex++;
		} else {
			capture >> currentFrame;
			if (currentFrame.empty())
				break;
			Mat flow = ComputerVisionUtil::calculateOpticalFlow(previousFrame,
					currentFrame);
			float som = calculateSumOfMagnitude(flow);
			motions.push_back(som);
			cout << som << '\n';
			currentFrame.copyTo(previousFrame);
			frameIndex++;
		}
	}
	int firstMaximum = 50;
	bool flag = false;
	int maximum = 50;
	for (int i = 51; i < motions.size() - 50; i++) {
		if (motions[maximum] < motions[i])
			maximum = i;
		if (flag == false) {
			float sum1 = 0;
			float sum2 = 0;
			for (int j = i - 10; j < i; j++) {
				sum1 += motions[j];
			}

			for (int j = i + 1; j < i + 10; j++) {
				sum2 += motions[j];
			}
			sum1 = sum1 / 10;
			sum2 = sum2 / 10;
			if (motions[i] > sum1 && motions[i] > sum2) {
				firstMaximum = i;
				flag = true;
			}
		}
	}
	ofstream profiler;
	profiler.open(output.c_str(), ofstream::app);
	profiler << file.name << ":" << maximum << ":" << firstMaximum << "\n";
	profiler.close();
}
void MomentOfSnapDetector::findMomentOfSnap(string output, string profile,
		bool useCamera, int frameGap, int downSample, int scanningWindowSize) {
	int totalExecutionTime = 0;
	time_t begin, end;

	ofstream profiler, localMaximumProfiler, bumps;
	profiler.open(profile.c_str(), ofstream::app);
	bumps.open("bumps.log", ofstream::app);
	localMaximumProfiler.open(".localMaximums.log", ofstream::app);
	ofstream momentOfSnapOutput;
	momentOfSnapOutput.open(output.c_str(), ofstream::app);
	begin = time(NULL);
	profiler << "********************************\n";
	profiler << "Video Name : " << file.name << '\n';
	bumps << "********************************\n";
	bumps << "Video Name : " << file.name << '\n';

	localMaximumProfiler << "********************************\n";
	localMaximumProfiler << "Video Name : " << file.name << '\n';
	if (NUMBER_OF_FRAMES < scanningWindowSize) {
		profiler
				<< "Short Video detected. Can not proceed. Outputting default values\n";
		momentOfSnapOutput << "Video Name : " << file.name << " ;";
		momentOfSnapOutput << "Moment of Snap:" << NUMBER_OF_FRAMES / 2 << " ;";
		momentOfSnapOutput << "Confidence: " << 0.5 << "; Score1: " << 0.5
				<< "; Score2: " << 0.5 << "; Score3: " << 0.5 << '\n';
		momentOfSnapOutput.flush();
		profiler.flush();
		momentOfSnapOutput.close();
		profiler.close();
		localMaximumProfiler.close();
		return;
	}
	profiler << "Generating optical flow Started.\n";
	generateOpticalflow(frameGap, downSample);
	end = time(NULL);
	totalExecutionTime += end - begin;
	profiler << "Duration of Running optical flow: " << (end - begin)
			<< " seconds." << '\n';
	profiler << "Video Length : " << videoLength << '\n';
	profiler << "# Of Frames Processed in Optical flow: " << frames.size()
			<< '\n';
	profiler << "Generating optical flow Finished.\n\n";
	profiler.flush();

//	Field field;
	float *** analysis;
	analysis = new float**[NUMBER_OF_ALGORITHMS];
	for (int i = 0; i < NUMBER_OF_ALGORITHMS; i++) {
		analysis[i] = new float*[NUMBER_OF_FRAMES];
	}
	for (int i = 0; i < NUMBER_OF_ALGORITHMS; i++) {
		for (int j = 0; j < NUMBER_OF_FRAMES; j++) {
			analysis[i][j] = new float[grid.size()];
		}
	}
	float minimumValue = numeric_limits<float>::max();
	float maximumValue = numeric_limits<float>::min();
	float averageValue = 0;
	int index = 0;
	profiler << "Analyzing Video started.\n";
	profiler << "Scanning Window Size = " << scanningWindowSize << '\n';
	begin = time(NULL);
	for (unsigned int fIndex = 0; fIndex < frames.size(); fIndex++) {

		float sum = 0;
		int index_temp = 0;
		for (unsigned int i = 0; i < frames[fIndex].windows.size(); i++) {
			for (int y = 0; y < frames[fIndex].windows[i].opticalflow.rows; y +=
					1) {
				for (int x = 0; x < frames[fIndex].windows[i].opticalflow.cols;
						x += 1) {
					const Point2f& fxy =
							frames[fIndex].windows[i].opticalflow.at<Point2f>(y,
									x);
					float magnitude = sqrt(pow(fxy.x, 2) + pow(fxy.y, 2));
					sum += magnitude;
					index_temp++;
				}
			}
		}
		sum = sum / index_temp;

		for (unsigned int windowIndex = 0;
				windowIndex < frames[fIndex].windows.size(); windowIndex++) {
			calculateSumOfMagnitude(
					frames[fIndex].windows[windowIndex].opticalflow, analysis,
					-1, -1, 1, fIndex + 1, windowIndex);

			frames[fIndex].windows[windowIndex].value =
					analysis[0][fIndex][windowIndex];
			frames[fIndex].windows[windowIndex].degree = 0;
			if (frames[fIndex].windows[windowIndex].value < minimumValue) {
				minimumValue = frames[fIndex].windows[windowIndex].value;
			}
			if (frames[fIndex].windows[windowIndex].value > maximumValue) {
				maximumValue = frames[fIndex].windows[windowIndex].value;
			}
			index++;
			averageValue += frames[fIndex].windows[windowIndex].value;
		}
	}
	averageValue = averageValue / index;
	int window = scanningWindowSize / (2 * frameGap);
	int timeStep = 5;
	profiler << "Minimum Threshold Value:" << minimumValue << ","
			<< "  Maximum  Threshold Value" << maximumValue
			<< "   Average  Threshold Value:" << averageValue << '\n';
	float step = 0.05 * (maximumValue - minimumValue);
	float threshold = step;
	ofstream confidenceLog;
	confidenceLog.open(".confidence.csv", ofstream::app);
	confidenceLog << "Video: " << file.name << '\n';
	vector<LocalMaxima> localMaximums;
	map<float, vector<Element> > distanceMap;
	stringstream xValues;
	stringstream yValues;
	for (; threshold < maximumValue; threshold += step) {
		if (xValues.str().length() != 0) {
			localMaximumProfiler << xValues.str().c_str() << endl;
			localMaximumProfiler << yValues.str().c_str() << endl;
		}
		localMaximumProfiler.flush();
		localMaximumProfiler << "Threshold:" << threshold << endl;
		xValues.str("");
		yValues.str("");
		/**
		 * Calculating active cells based on the current threshold
		 */
		int activeCellsInFrame[frames.size()];
		activeCellsInFrame[0] = 0;
		for (unsigned int fIndex = 1; fIndex < frames.size(); fIndex++) {
			activeCellsInFrame[fIndex] = 0;
			for (unsigned int i = 0; i < frames[fIndex].windows.size(); i++) {
				if (frames[fIndex].windows[i].value < threshold
						|| !isInField(frames[fIndex].windows[i], width, height)
						|| !frames[fIndex].windows[i].containsPlayer) {
				} else {

					activeCellsInFrame[fIndex]++;
				}
			}
			if (activeCellsInFrame[fIndex] > 3) {
				activeCellsInFrame[fIndex] = 3;
			}
		}

		double distances[frames.size() - 2 * window];
		double numberOfActivationsBeforeTimes[frames.size() - 2 * window];
		double numberOfActivationsAfterTimes[frames.size() - 2 * window];
		double sumOfOpticalflowBefore[frames.size() - 2 * window];
		double sumOfOpticalflowAfter[frames.size() - 2 * window];
		double sumOfOpticalflowBeforeT1[frames.size() - 2 * window];
		double sumOfOpticalflowAfterT1[frames.size() - 2 * window];
		double sumOfOpticalflowBeforeT2[frames.size() - 2 * window];
		double sumOfOpticalflowAfterT2[frames.size() - 2 * window];
		double sumOfOpticalflowBeforeT3[frames.size() - 2 * window];
		double sumOfOpticalflowAfterT3[frames.size() - 2 * window];
		int t1 = 5;
		int t2 = 15;
		for (unsigned int timeIndex = window;
				timeIndex < frames.size() - window; timeIndex += timeStep) {

			double numberOfActivationsBeforeTime = 0;
			double numberOfActivationsAfterTime = 0;

			sumOfOpticalflowBefore[timeIndex - window] = 0;
			sumOfOpticalflowAfter[timeIndex - window] = 0;
			sumOfOpticalflowBeforeT1[timeIndex - window] = 0;
			sumOfOpticalflowAfterT1[timeIndex - window] = 0;
			sumOfOpticalflowBeforeT2[timeIndex - window] = 0;
			sumOfOpticalflowAfterT2[timeIndex - window] = 0;
			sumOfOpticalflowBeforeT3[timeIndex - window] = 0;
			sumOfOpticalflowAfterT3[timeIndex - window] = 0;

			//This loop calculates the actual optical flow value in active cells as clue for motion
			for (unsigned int fIndex = timeIndex - window;
					fIndex < timeIndex + window; fIndex++) {
				for (unsigned int i = 0; i < frames[fIndex].windows.size();
						i++) {

					if (!isInField(frames[fIndex].windows[i], width, height)) {
						continue;
					} else {
						double tempValue = (frames[fIndex].windows[i].value
								- minimumValue) / (maximumValue - minimumValue);
						if (fIndex < timeIndex) {
							sumOfOpticalflowBefore[timeIndex - window] +=
									tempValue;
							sumOfOpticalflowBeforeT3[timeIndex - window] +=
									tempValue;
							if (fIndex >= timeIndex - t1) {
								sumOfOpticalflowBeforeT1[timeIndex - window] +=
										tempValue;
							}
							if (fIndex >= timeIndex - t2) {
								sumOfOpticalflowBeforeT2[timeIndex - window] +=
										tempValue;
							}
						} else {
							sumOfOpticalflowAfter[timeIndex - window] +=
									tempValue;
							sumOfOpticalflowAfterT3[timeIndex - window] +=
									tempValue;
							if (fIndex <= timeIndex + t1) {
								sumOfOpticalflowAfterT1[timeIndex - window] +=
										tempValue;
							}
							if (fIndex <= timeIndex + t2) {
								sumOfOpticalflowAfterT2[timeIndex - window] +=
										tempValue;
							}

						}

					}
				}
			}
			for (unsigned int fIndex = timeIndex - window;
					fIndex < timeIndex + window; fIndex++) {

				if (fIndex < timeIndex) {
					numberOfActivationsBeforeTime += activeCellsInFrame[fIndex];
				} else {
					numberOfActivationsAfterTime += activeCellsInFrame[fIndex];

				}
			}
			numberOfActivationsBeforeTime = numberOfActivationsBeforeTime
					/ window;
			numberOfActivationsAfterTime = numberOfActivationsAfterTime
					/ window;

			double temp = pow(
					numberOfActivationsAfterTime
							- numberOfActivationsBeforeTime, 2);
			temp = sqrt(temp);
			if (numberOfActivationsAfterTime < numberOfActivationsBeforeTime)
				temp = -temp;
			distances[timeIndex - window] = temp;
			xValues << timeIndex - window << ",";
			yValues << temp << ",";
			numberOfActivationsBeforeTimes[timeIndex - window] =
					numberOfActivationsBeforeTime;
			numberOfActivationsAfterTimes[timeIndex - window] =
					numberOfActivationsAfterTime;
		}

		for (int timeIndex = 0; timeIndex < frames.size() - 2 * window;
				timeIndex += timeStep) {
			int previousIndex = timeIndex - timeStep;
			int nextIndex = timeIndex + timeStep;
			/**
			 * Creating list of local maximas
			 */
			if (previousIndex >= 0 && nextIndex < frames.size() - 2 * window) {
				if (distances[timeIndex] > distances[previousIndex]
						&& distances[timeIndex] > distances[nextIndex]) {
					LocalMaxima localMaxima;
					localMaxima.frame = (window + timeIndex) * frameGap;
					localMaxima.threshold = threshold;
					localMaxima.distance = distances[timeIndex];
					localMaxima.sumOfOpticalflowBefore =
							sumOfOpticalflowBefore[timeIndex];
					localMaxima.sumOfOpticalflowAfter =
							sumOfOpticalflowAfter[timeIndex];
					localMaxima.numberOfActivationsBeforeTime =
							numberOfActivationsBeforeTimes[timeIndex];
					localMaxima.numberOfActivationsAfterTime =
							numberOfActivationsAfterTimes[timeIndex];
					localMaxima.motionMinusT1 =
							sumOfOpticalflowBeforeT1[timeIndex];
					localMaxima.motionMinusT2 =
							sumOfOpticalflowBeforeT2[timeIndex];
					localMaxima.motionMinusT3 =
							sumOfOpticalflowBeforeT3[timeIndex];
					localMaxima.motionPlusT1 =
							sumOfOpticalflowAfterT1[timeIndex];
					localMaxima.motionPlusT2 =
							sumOfOpticalflowAfterT2[timeIndex];
					localMaxima.motionPlusT3 =
							sumOfOpticalflowAfterT3[timeIndex];

					if (timeIndex - timeStep < 0) {
						localMaxima.motionInFrameMinusT1Before = 0;
						localMaxima.motionInFrameMinusT1After = 0;

					} else {
						localMaxima.motionInFrameMinusT1Before =
								sumOfOpticalflowBefore[timeIndex - timeStep];
						localMaxima.motionInFrameMinusT1After =
								sumOfOpticalflowAfter[timeIndex - timeStep];

					}

					if (timeIndex + timeStep > frames.size() - 2 * window - 1) {
						localMaxima.motionInFramePlusT1Before = 0;
						localMaxima.motionInFramePlusT1After = 0;

					} else {
						localMaxima.motionInFramePlusT1Before =
								sumOfOpticalflowBefore[timeIndex + timeStep];
						localMaxima.motionInFramePlusT1After =
								sumOfOpticalflowAfter[timeIndex + timeStep];
					}

					if (timeIndex - 2 * timeStep < 0) {
						localMaxima.motionInFrameMinusT2Before = 0;
						localMaxima.motionInFrameMinusT2After = 0;

					} else {
						localMaxima.motionInFrameMinusT2Before =
								sumOfOpticalflowBefore[timeIndex - 2 * timeStep];
						localMaxima.motionInFrameMinusT2After =
								sumOfOpticalflowAfter[timeIndex - 2 * timeStep];

					}

					if (timeIndex + 2 * timeStep
							> frames.size() - 2 * window - 1) {
						localMaxima.motionInFramePlusT2Before = 0;
						localMaxima.motionInFramePlusT2After = 0;

					} else {
						localMaxima.motionInFramePlusT2Before =
								sumOfOpticalflowBefore[timeIndex + 2 * timeStep];
						localMaxima.motionInFramePlusT2After =
								sumOfOpticalflowAfter[timeIndex + 2 * timeStep];
					}
					if (localMaxima.distance > 0)
						localMaximums.push_back(localMaxima);
				} else {
					bool flag = true;
					if (distances[timeIndex] < distances[previousIndex]
							|| distances[timeIndex] < distances[nextIndex]) {
						flag = false;
					}
					if (distances[timeIndex] == distances[previousIndex]) {
						while (previousIndex >= 0
								&& distances[timeIndex]
										== distances[previousIndex]) {
							previousIndex -= timeStep;
						}
						if (previousIndex < 0
								|| distances[timeIndex]
										<= distances[previousIndex]) {
							flag = false;
						}
					}
					if (distances[timeIndex] == distances[nextIndex]) {
						while (nextIndex < frames.size() - 2 * window
								&& distances[timeIndex] == distances[nextIndex]) {
							nextIndex += timeStep;
						}
						if (nextIndex >= frames.size() - 2 * window
								|| distances[timeIndex]
										>= distances[nextIndex]) {
							flag = false;
						}
					}

					if (flag == true) {
						LocalMaxima localMaxima;
						localMaxima.frame = (window + timeIndex) * frameGap;
						localMaxima.threshold = threshold;
						localMaxima.distance = distances[timeIndex];
						localMaxima.numberOfActivationsBeforeTime =
								numberOfActivationsBeforeTimes[timeIndex];
						localMaxima.numberOfActivationsAfterTime =
								numberOfActivationsAfterTimes[timeIndex];
						localMaxima.sumOfOpticalflowBefore =
								sumOfOpticalflowBefore[timeIndex];
						localMaxima.sumOfOpticalflowAfter =
								sumOfOpticalflowAfter[timeIndex];
						localMaxima.motionMinusT1 =
								sumOfOpticalflowBeforeT1[timeIndex];
						localMaxima.motionMinusT2 =
								sumOfOpticalflowBeforeT2[timeIndex];
						localMaxima.motionMinusT3 =
								sumOfOpticalflowBeforeT3[timeIndex];
						localMaxima.motionPlusT1 =
								sumOfOpticalflowAfterT1[timeIndex];
						localMaxima.motionPlusT2 =
								sumOfOpticalflowAfterT2[timeIndex];
						localMaxima.motionPlusT3 =
								sumOfOpticalflowAfterT3[timeIndex];

						if (timeIndex - timeStep < 0) {
							localMaxima.motionInFrameMinusT1Before = 0;
							localMaxima.motionInFrameMinusT1After = 0;

						} else {
							localMaxima.motionInFrameMinusT1Before =
									sumOfOpticalflowBefore[timeIndex - timeStep];
							localMaxima.motionInFrameMinusT1After =
									sumOfOpticalflowAfter[timeIndex - timeStep];

						}

						if (timeIndex + timeStep
								> frames.size() - 2 * window - 1) {
							localMaxima.motionInFramePlusT1Before = 0;
							localMaxima.motionInFramePlusT1After = 0;

						} else {
							localMaxima.motionInFramePlusT1Before =
									sumOfOpticalflowBefore[timeIndex + timeStep];
							localMaxima.motionInFramePlusT1After =
									sumOfOpticalflowAfter[timeIndex + timeStep];
						}

						if (timeIndex - 2 * timeStep < 0) {
							localMaxima.motionInFrameMinusT2Before = 0;
							localMaxima.motionInFrameMinusT2After = 0;

						} else {
							localMaxima.motionInFrameMinusT2Before =
									sumOfOpticalflowBefore[timeIndex
											- 2 * timeStep];
							localMaxima.motionInFrameMinusT2After =
									sumOfOpticalflowAfter[timeIndex
											- 2 * timeStep];

						}

						if (timeIndex + 2 * timeStep
								> frames.size() - 2 * window - 1) {
							localMaxima.motionInFramePlusT2Before = 0;
							localMaxima.motionInFramePlusT2After = 0;

						} else {
							localMaxima.motionInFramePlusT2Before =
									sumOfOpticalflowBefore[timeIndex
											+ 2 * timeStep];
							localMaxima.motionInFramePlusT2After =
									sumOfOpticalflowAfter[timeIndex
											+ 2 * timeStep];
						}
						if (localMaxima.distance > 0)
							localMaximums.push_back(localMaxima);
					}
				}
			} else {

			}

			/**
			 * Creating distance map per frame
			 */
			map<float, vector<Element> >::iterator it;
			it = distanceMap.find(threshold);
			if (it == distanceMap.end()) {
				vector<Element> distanceValues;
				distanceMap[threshold] = distanceValues;
				it = distanceMap.find(threshold);
			}
			Element element;
			element.frame = (timeIndex + window) * frameGap;
			element.distance = distances[timeIndex];
			element.numberOfActivationsBeforeTime =
					numberOfActivationsBeforeTimes[timeIndex];
			element.numberOfActivationsAfterTime =
					numberOfActivationsAfterTimes[timeIndex];
			(*it).second.push_back(element);
		}
	}
	/**
	 * Logging the distance matrix per frame and threshold
	 * Finding the max and avg distance per frame
	 */
	confidenceLog << "----------------------------------------\n";
	confidenceLog << "Distance Values for different threshold per Frame:"
			<< '\n';
	confidenceLog << "----------------------------------------\n";
	int framesCount = (*distanceMap.begin()).second.size();
	map<float, vector<Element> >::iterator it;
	confidenceLog << "Frames,";
	for (it = distanceMap.begin(); it != distanceMap.end(); it++) {
		confidenceLog << (*it).first << ",";
	}
	confidenceLog << '\n';
	vector<Element> elements;
	for (int tempFrameCount = 0; tempFrameCount < framesCount;
			tempFrameCount++) {
		int t = 0;
		Element element;
		element.maximumDistance = -1;
		float averageDistancePerFrame = 0;
		for (it = distanceMap.begin(); it != distanceMap.end(); it++) {
			if (t == 0) {
				confidenceLog << (*it).second[tempFrameCount].frame << ",";
				element.frame = (*it).second[tempFrameCount].frame;
			}
			t++;
			if (((*it).second[tempFrameCount].numberOfActivationsAfterTime
					- (*it).second[tempFrameCount].numberOfActivationsBeforeTime)
					> 0 && (*it).second[tempFrameCount].frame > 50)
				averageDistancePerFrame +=
						(*it).second[tempFrameCount].distance;
			if (element.maximumDistance < (*it).second[tempFrameCount].distance
					&& ((*it).second[tempFrameCount].numberOfActivationsAfterTime
							- (*it).second[tempFrameCount].numberOfActivationsBeforeTime)
							> 0 && (*it).second[tempFrameCount].frame > 50) {
				element.maximumDistance = (*it).second[tempFrameCount].distance;
			}
			confidenceLog << (*it).second[tempFrameCount].distance << ",";
		}
		element.averageDistance = averageDistancePerFrame / t;
		elements.push_back(element);
		confidenceLog << '\n';
	}

	/**
	 *
	 */
	profiler << "Local Maximums Size Before Pruning:" << localMaximums.size()
			<< '\n';
	profiler << "Local Maximums Size After Pruning:" << localMaximums.size()
			<< '\n';
	map<int, LocalMaxima> maxPerFrame;
	profiler << "Local Maximums: " << '\n';
	for (unsigned int modeIndex = 0; modeIndex < localMaximums.size();
			modeIndex++) {
		profiler << localMaximums[modeIndex].frame << ": Distance: "
				<< localMaximums[modeIndex].distance << '\n';
		profiler << "---------------------\n";
		profiler << "BF T1: " << localMaximums[modeIndex].motionMinusT1
				<< ";AF T1: " << localMaximums[modeIndex].motionPlusT1
				<< ";BF T2: " << localMaximums[modeIndex].motionMinusT2
				<< ";AF T2: " << localMaximums[modeIndex].motionPlusT2
				<< ";In-T1B: "
				<< localMaximums[modeIndex].motionInFrameMinusT1Before
				<< ";In-T1A: "
				<< localMaximums[modeIndex].motionInFrameMinusT1After
				<< ";In+T1B: "
				<< localMaximums[modeIndex].motionInFramePlusT1Before
				<< ";In+T1A: "
				<< localMaximums[modeIndex].motionInFramePlusT1After
				<< ";In-T2B: "
				<< localMaximums[modeIndex].motionInFrameMinusT2Before
				<< ";In-T2A: "
				<< localMaximums[modeIndex].motionInFrameMinusT2After
				<< ";In+T2B: "
				<< localMaximums[modeIndex].motionInFramePlusT2Before
				<< ";In+T2A: "
				<< localMaximums[modeIndex].motionInFramePlusT2After << '\n';

		map<int, LocalMaxima>::iterator it;
		it = maxPerFrame.find(localMaximums[modeIndex].frame);
		if (it == maxPerFrame.end()) {
			maxPerFrame[localMaximums[modeIndex].frame] =
					localMaximums[modeIndex];
		} else {
			if (maxPerFrame[localMaximums[modeIndex].frame].distance
					< localMaximums[modeIndex].distance) {
				maxPerFrame[localMaximums[modeIndex].frame] =
						localMaximums[modeIndex];
			}
		}

	}

	/**
	 * Find Maximum Distances per frame:globalMaximums
	 */
	/**
	 * Finding the global maximum
	 */
	vector<LocalMaxima> globalMaximums;
	for (int ii = 0; ii < 3; ii++) {
		LocalMaxima globalMaximum;
		globalMaximum.distance = numeric_limits<double>::min();
		globalMaximum.frame = 0;
		map<int, LocalMaxima>::iterator it;
		for (it = maxPerFrame.begin(); it != maxPerFrame.end(); it++) {
			bool flag = false;
			for (int jj = 0; jj < globalMaximums.size(); jj++) {
				if (globalMaximums[jj].frame == (*it).second.frame)
					flag = true;
			}
			if (!flag && (*it).second.distance > globalMaximum.distance
					&& (*it).second.frame < videoLength - 60) {
				globalMaximum = (*it).second;
			}
		}
		globalMaximums.push_back(globalMaximum);
	}
	profiler << "-----------------\n";
	LocalMaxima globalMaximum = globalMaximums[0];
	bumps << "------------------\n";

	{
		bumps << "Top Local Maximums: \n";
		bumps << globalMaximums[0].frame << ": Distance: "
				<< globalMaximums[0].distance << ","
				<< " Opticalflow Before this frame: "
				<< globalMaximums[0].sumOfOpticalflowBefore << '\n';
		bool tempFlag = false;
		for (unsigned int jj = 1; jj < globalMaximums.size(); jj++) {
			bumps << globalMaximums[jj].frame << ": Distance: "
					<< globalMaximums[jj].distance << ","
					<< " Opticalflow Before this frame: "
					<< globalMaximums[jj].sumOfOpticalflowBefore << '\n';

			if (globalMaximum.sumOfOpticalflowBefore
					> globalMaximums[jj].sumOfOpticalflowBefore
					&& globalMaximums[jj].distance >= 2) { // && globalMaximums[jj].distance > globalMaximum.distance - 1.0) {
				tempFlag = true;
				globalMaximum = globalMaximums[jj];
			}
		}

	}

	profiler << "Top Local Maximums: \n";
	profiler << globalMaximums[0].frame << ": Distance: "
			<< globalMaximums[0].distance << ","
			<< " Opticalflow Before this frame: "
			<< globalMaximums[0].sumOfOpticalflowBefore << '\n';
	bool tempFlag = false;
	for (unsigned int jj = 1; jj < globalMaximums.size(); jj++) {
		profiler << globalMaximums[jj].frame << ": Distance: "
				<< globalMaximums[jj].distance << ","
				<< " Opticalflow Before this frame: "
				<< globalMaximums[jj].sumOfOpticalflowBefore << '\n';

		if (globalMaximum.sumOfOpticalflowBefore
				> globalMaximums[jj].sumOfOpticalflowBefore
				&& globalMaximums[jj].distance >= 2) { // && globalMaximums[jj].distance > globalMaximum.distance - 1.0) {
			tempFlag = true;
			globalMaximum = globalMaximums[jj];
		}
	}
	if (tempFlag == false)
		globalMaximum = globalMaximums[0];
	/**
	 *
	 */

	/**
	 * Assgining Score to selected frame.
	 */
	float score = 0;
	float score1 = 0;
	float score2 = 0;
	float score3 = 0;
	float totalCheckedLocalMaximums = 0;
	profiler << "...............\n";
	profiler << "Score1 Calculation:\n";
	float maxD = numeric_limits<double>::min();
	for (unsigned int indexI = 0; indexI < localMaximums.size(); indexI++) {
		if (localMaximums[indexI].distance > maxD)
			maxD = localMaximums[indexI].distance;
	}
	profiler << "MaxD:" << maxD << '\n';
	for (unsigned int indexI = 0; maxD != 0 && indexI < localMaximums.size();
			indexI++) {
		if (localMaximums[indexI].distance >= 2) {
			totalCheckedLocalMaximums += localMaximums[indexI].distance / maxD;
			profiler << localMaximums[indexI].frame << ":"
					<< localMaximums[indexI].distance << '\n';
			if (localMaximums[indexI].frame >= globalMaximum.frame - 20
					&& localMaximums[indexI].frame <= globalMaximum.frame + 20)
				score1 += localMaximums[indexI].distance / maxD;
		}
	}
	if (totalCheckedLocalMaximums == 0) {
		for (unsigned int indexI = 0;
				maxD != 0 && indexI < localMaximums.size(); indexI++) {
			if (localMaximums[indexI].distance >= 1.5) {
				totalCheckedLocalMaximums += localMaximums[indexI].distance
						/ maxD;
				profiler << localMaximums[indexI].frame << ":"
						<< localMaximums[indexI].distance << '\n';
				if (localMaximums[indexI].frame >= globalMaximum.frame - 20
						&& localMaximums[indexI].frame
								<= globalMaximum.frame + 20)
					score1 += localMaximums[indexI].distance / maxD;
			}
		}
	}
	if (totalCheckedLocalMaximums == 0) {
		for (unsigned int indexI = 0;
				maxD != 0 && indexI < localMaximums.size(); indexI++) {
			if (localMaximums[indexI].distance >= 1) {
				totalCheckedLocalMaximums += localMaximums[indexI].distance
						/ maxD;
				profiler << localMaximums[indexI].frame << ":"
						<< localMaximums[indexI].distance << '\n';
				if (localMaximums[indexI].frame >= globalMaximum.frame - 20
						&& localMaximums[indexI].frame
								<= globalMaximum.frame + 20)
					score1 += localMaximums[indexI].distance / maxD;
			}
		}
	}
	if (totalCheckedLocalMaximums == 0) {
		for (unsigned int indexI = 0;
				maxD != 0 && indexI < localMaximums.size(); indexI++) {
			if (localMaximums[indexI].distance >= 0.5) {
				totalCheckedLocalMaximums += localMaximums[indexI].distance
						/ maxD;
				profiler << localMaximums[indexI].frame << ":"
						<< localMaximums[indexI].distance << '\n';
				if (localMaximums[indexI].frame >= globalMaximum.frame - 20
						&& localMaximums[indexI].frame
								<= globalMaximum.frame + 20)
					score1 += localMaximums[indexI].distance / maxD;
			}
		}
	}
	if (maxD != 0 && totalCheckedLocalMaximums == 0) {
		for (unsigned int indexI = 0; indexI < localMaximums.size(); indexI++) {
			if (localMaximums[indexI].distance > 0) {
				totalCheckedLocalMaximums += localMaximums[indexI].distance
						/ maxD;
				profiler << localMaximums[indexI].frame << ":"
						<< localMaximums[indexI].distance << '\n';
				if (localMaximums[indexI].frame >= globalMaximum.frame - 20
						&& localMaximums[indexI].frame
								<= globalMaximum.frame + 20)
					score1 += localMaximums[indexI].distance / maxD;
			}
		}
	}
	if (maxD != 0)
		score1 = score1 / totalCheckedLocalMaximums;
	else
		score1 = 0;
	if (localMaximums.size() == 1)
		score1 = 1;
	profiler << "...............\n";
	profiler << "Score2 Calculation:\n";
	globalMaximums.clear();
	for (int ii = 0; ii < 5; ii++) {
		LocalMaxima globalMaximum;
		globalMaximum.distance = numeric_limits<double>::min();
		globalMaximum.frame = 0;
		map<int, LocalMaxima>::iterator it;
		for (it = maxPerFrame.begin(); it != maxPerFrame.end(); it++) {
			bool flag = false;
			for (unsigned int jj = 0; jj < globalMaximums.size(); jj++) {
				if (globalMaximums[jj].frame == (*it).second.frame)
					flag = true;
			}
			if (!flag && (*it).second.distance > globalMaximum.distance
					&& (*it).second.frame < videoLength - 60) {
				globalMaximum = (*it).second;
			}
		}
		globalMaximums.push_back(globalMaximum);
	}

	for (unsigned int ii = 0; ii < globalMaximums.size(); ii++) {
		profiler << globalMaximums[ii].frame << ":"
				<< globalMaximums[ii].distance << '\n';
		if (globalMaximums[ii].frame >= globalMaximum.frame - 20
				&& globalMaximums[ii].frame <= globalMaximum.frame + 20)
			score2++;
	}
	if (globalMaximums.size() == 0)
		score2 = 0.1;
	else
		score2 = score2 / globalMaximums.size();
	profiler << "...............\n";
	profiler << "Score3 Calculation:\n";
	globalMaximums.clear();
	for (int iii = 0; iii < 5; iii++) {
		LocalMaxima globalMaximum;
		globalMaximum.distance = numeric_limits<double>::min();
		globalMaximum.frame = 0;
//		map<int, LocalMaxima>::iterator it;
		for (unsigned int ii = 0; ii < localMaximums.size(); ii++) {
			bool flag = false;
			for (unsigned int jj = 0; jj < globalMaximums.size(); jj++) {
				if (globalMaximums[jj].frame == localMaximums[ii].frame
						&& globalMaximums[jj].threshold
								== localMaximums[ii].threshold)
					flag = true;
			}
			if (!flag && localMaximums[ii].distance > globalMaximum.distance
					&& localMaximums[ii].frame < videoLength - 60) {
				globalMaximum = localMaximums[ii];
			}
		}
		globalMaximums.push_back(globalMaximum);
	}

	for (unsigned int ii = 0; ii < globalMaximums.size(); ii++) {
		profiler << globalMaximums[ii].frame << ":"
				<< globalMaximums[ii].distance << '\n';
		if (globalMaximums[ii].frame >= globalMaximum.frame - 20
				&& globalMaximums[ii].frame <= globalMaximum.frame + 20)
			score3++;
	}
	if (globalMaximums.size() == 0)
		score3 = 0.1;
	else
		score3 = score3 / globalMaximums.size();
	score = 0.5 * score1 + 0.5 * score3;
	if (maxD > 1 && maxD < 2)
		score = score * 0.5f;
	else if (maxD < 1) {
		score = score * 0.25f;
	}
	/**
	 *
	 */
	end = time(NULL);
	totalExecutionTime += end - begin;
	profiler << "Duration of Processing Video: " << (end - begin) << " seconds."
			<< '\n';
	profiler << "Analyzing Video finished.\n";
	profiler << "Total Processing Time: " << totalExecutionTime << " seconds."
			<< '\n';
	profiler.flush();

	/*
	 * Results
	 */

	momentOfSnapOutput << "Video Name : " << file.name << " ;";
	momentOfSnapOutput << "Moment of Snap:" << globalMaximum.frame << " ;";
	momentOfSnapOutput << "Confidence: " << score << "; Score1: " << score1
			<< "; Score2: " << score2 << "; Score3: " << score3 << '\n';

	/**
	 * Profile
	 */
	profiler << "Video Name : " << file.name << "\n";
	profiler << "Moment of Snap:" << globalMaximum.frame << '\n';
	profiler << "Max Distance:" << globalMaximum.distance << '\n';
	profiler << "Threshold:" << globalMaximum.threshold << '\n';
	profiler << "Confidence: " << score << "; Score1: " << score1
			<< "; Score2: " << score2 << "; Score3: " << score3 << '\n';
	profiler << "BF T1: " << globalMaximum.motionMinusT1 << ";AF T1: "
			<< globalMaximum.motionPlusT1 << ";BF T2: "
			<< globalMaximum.motionMinusT2 << ";AF T2: "
			<< globalMaximum.motionPlusT2 << ";In-T1B: "
			<< globalMaximum.motionInFrameMinusT1Before << ";In-T1A: "
			<< globalMaximum.motionInFrameMinusT1After << ";In+T1B: "
			<< globalMaximum.motionInFramePlusT1Before << ";In+T1A: "
			<< globalMaximum.motionInFramePlusT1After << ";In-T2B: "
			<< globalMaximum.motionInFrameMinusT2Before << ";In-T2A: "
			<< globalMaximum.motionInFrameMinusT2After << ";In+T2B: "
			<< globalMaximum.motionInFramePlusT2Before << ";In+T2A: "
			<< globalMaximum.motionInFramePlusT2After << '\n';
	profiler << "----------------------------------------------------\n\n";
	delete analysis;
	localMaximumProfiler.flush();
	localMaximumProfiler.close();
	momentOfSnapOutput.close();
	profiler.close();
}

} /* namespace hudl */

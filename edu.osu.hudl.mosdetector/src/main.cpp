/*
 * main.cpp
 *
 *  Created on: Mar 30, 2012
 *      Author: behrooz
 */
#include "MomentOfSnapDetector.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <map>
using namespace vision;

using namespace hudl;
struct MotionModel {
	int numberOfTheFirstImage;
	float motionModelOriginRowC;
	float motionModelOriginColumnC;
	float c1, c2;
	float a1, a2, a3, a4;
	float q1, q2, q3, q4, q5, q6;
	float illuminationVariationParameter;
	float supporSize;
};

void findMomentOfSnap(char* name, string output, string profile, bool camera,
		int scanningWindowSize) {
	map<string, int> starts;
	ifstream startsFile;
	startsFile.open("starts.txt");
	string line;
	if (startsFile.is_open()) {
		while (startsFile.good()) {
			getline(startsFile, line);
			char* pch1 = strtok((char*) line.c_str(), ":");
			if (pch1 == NULL)
				continue;
			char* pch2 = strtok(NULL, ":");
			starts[pch1] = atoi(pch2);
		}
	}
	MomentOfSnapDetector snapRecognizer;
	cout << "Processing Video :" << name << " Started\n";
	snapRecognizer.init(CommonUtil::getFileStructure(name));
	snapRecognizer.findMomentOfSnap(output, profile, camera, 2, 3,
			scanningWindowSize);
	cout << "Processing Video " << name << " Finised. \n";
}

int findMomentOfSnap(int argc, char **argv) {
	string string1 = "-p";
	string string2 = "-w";
	string stringTrue = "true";
	string stringFalse = "false";
	int scanningWindowSize = 50;
	string profileFileName = ".profile.log";
	if (argc < 3) {
		cout << "Error in Input Parameters\n";
		cout
				<< "snapdetector input_video_file output_file [-p profile_file_name] [-w windowSize]\n";
		return -1;
	} else if (argc == 5) {
		if (string1.compare(argv[3]) == 0) {
			profileFileName = argv[4];
		} else if (string2.compare(argv[3]) == 0) {
			scanningWindowSize = atoi(argv[4]);
		} else {
			cout << "Error in Input Parameters\n";
			cout
					<< "snapdetector input_video_file output_file [-p profile_file_name] [-w windowSize]\n";
			return -1;

		}
	} else if (argc == 7) {
		if (string1.compare(argv[3]) == 0 && string2.compare(argv[5]) == 0) {
			profileFileName = argv[4];
			scanningWindowSize = atoi(argv[6]);
		} else if (string1.compare(argv[5]) == 0
				&& string2.compare(argv[3]) == 0) {
			profileFileName = argv[6];
			scanningWindowSize = atoi(argv[4]);
		} else {
			cout << "Error in Input Parameters\n";
			cout
					<< "snapdetector input_video_file output_file [-p profile_file_name] [-w windowSize]\n";
			return -1;
		}

	} else {
		cout << "Error in Input Parameters\n";
		cout
				<< "snapdetector input_video_file output_file [-p profile_file_name] [-w windowSize]\n";
		return -1;
	}
	findMomentOfSnap(argv[1], argv[2], profileFileName, false,
			scanningWindowSize);
	return 0;

}
int main(int argc, char **argv) {
	return findMomentOfSnap(argc, argv);
}

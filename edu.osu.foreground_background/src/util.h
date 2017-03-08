/*
 * util.h
 *
 *  Created on: Jan 25, 2015
 *      Author: behrooz
 */
#ifndef UTIL_H_
#define UTIL_H_

#include "CommonLibs.h"
#include "VisionLibs.h"
#include "VisionAPI.h"
#include <opencv2/video/background_segm.hpp>
using namespace std;
using namespace cv;

int k = 5;
int s = 2;
float t = 0.005;
int step = 1;

vector<Mat> readHomographyMatrices(string fileName) {
	ifstream fin(fileName.c_str());
	vector<Mat> results;
	int frameNum;
	fin >> frameNum;

	for (int i = 0; i < frameNum; i++) {
		int row, col;
		fin >> row >> col;
		Mat A(row, col, CV_64F);
		for (int r = 0; r < 3; r++) {
			for (int c = 0; c < 3; c++) {
				double tmp;
				fin >> tmp;
				A.at<double>(r, c) = tmp;
			}
		}
		results.push_back(A);
	}
	return results;
}

vector<Mat> scan(Mat backgroundPatch, Mat warppedFramePatch) {
	vector<Mat> patches;
	for (int y = 0; y <= backgroundPatch.rows - warppedFramePatch.rows; y +=
			step) {
		for (int x = 0; x <= backgroundPatch.cols - warppedFramePatch.cols; x +=
				step) {
			patches.push_back(
					Mat(backgroundPatch,
							Rect(x, y, warppedFramePatch.cols,
									warppedFramePatch.rows)).clone());
		}
	}
	return patches;
}

void scan(Mat backgroundPatch1, Mat backgroundPatch2, vector<Mat> &patches1,
		vector<Mat> &patches2, Mat warppedFramePatch) {

	for (int y = 0; y <= backgroundPatch1.rows - warppedFramePatch.rows; y++) {
		for (int x = 0; x <= backgroundPatch1.cols - warppedFramePatch.cols;
				x++) {
			patches1.push_back(
					Mat(backgroundPatch1,
							Rect(x, y, warppedFramePatch.cols,
									warppedFramePatch.rows)).clone());
			patches2.push_back(
					Mat(backgroundPatch2,
							Rect(x, y, warppedFramePatch.cols,
									warppedFramePatch.rows)).clone());

		}
	}
}

vector<float> getSingleChannelHistogram(Mat patch, int histSize = 10) {
	/// Establish the number of bins

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 255 };
	const float* histRange = { range };

	bool uniform = true;
	bool accumulate = false;

	Mat gray_hist;

	/// Compute the histograms:
	calcHist(&patch, 1, 0, Mat(), gray_hist, 1, &histSize, &histRange, uniform,
			accumulate);
	int z = patch.cols * patch.rows;
	gray_hist = gray_hist / z;
	vector<float> histo = gray_hist;
	return histo;
}

Mat fillHoles(Mat image) {
	cv::Mat image_thresh;
	cv::threshold(image, image_thresh, 125, 255, cv::THRESH_BINARY);

	// Loop through the border pixels and if they're black, floodFill from there
	cv::Mat mask;
	image_thresh.copyTo(mask);
	for (int i = 0; i < mask.cols; i++) {
		if (mask.at<char>(0, i) == 0) {
			cv::floodFill(mask, cv::Point(i, 0), 255, 0, 10, 10);
		}
		if (mask.at<char>(mask.rows - 1, i) == 0) {
			cv::floodFill(mask, cv::Point(i, mask.rows - 1), 255, 0, 10, 10);
		}
	}
	for (int i = 0; i < mask.rows; i++) {
		if (mask.at<char>(i, 0) == 0) {
			cv::floodFill(mask, cv::Point(0, i), 255, 0, 10, 10);
		}
		if (mask.at<char>(i, mask.cols - 1) == 0) {
			cv::floodFill(mask, cv::Point(mask.cols - 1, i), 255, 0, 10, 10);
		}
	}

	// Compare mask with original.
	cv::Mat newImage;
	image.copyTo(newImage);
	for (int row = 0; row < mask.rows; ++row) {
		for (int col = 0; col < mask.cols; ++col) {
			if (mask.at<char>(row, col) == 0) {
				newImage.at<char>(row, col) = 255;
			}
		}
	}
	return newImage;
}
void findBlobs(const cv::Mat &binary, std::vector<std::vector<Point> > &blobs) {
	blobs.clear();
	// Using labels from 2+ for each blob
	cv::Mat label_image;
	binary.copyTo(label_image);
	label_image = label_image / 255;
	int label_count = 2; // starts at 2 because 0,1 are used already

	for (int y = 0; y < binary.rows; y++) {
		for (int x = 0; x < binary.cols; x++) {
			if ((int) label_image.at<uchar>(y, x) != 1) {
				continue;
			}

			cv::Rect rect;
			cv::floodFill(label_image, cv::Point(x, y), cv::Scalar(label_count),
					&rect, cv::Scalar(0), cv::Scalar(0), 4);

			std::vector<Point> blob;
			for (int i = rect.y; i < (rect.y + rect.height); i++) {
				for (int j = rect.x; j < (rect.x + rect.width); j++) {
					if ((int) label_image.at<uchar>(i, j) != label_count) {
						continue;
					}

					blob.push_back(cv::Point(j, i));
				}
			}

			blobs.push_back(blob);

			label_count++;
		}
	}
}
void postprocess(Mat& image) {
	medianBlur(image, image, 7);
	threshold(image, image, 100, 255, CV_THRESH_BINARY);
	int dilation_size = 3;
	Mat element = getStructuringElement(MORPH_ELLIPSE,
			Size(2 * dilation_size + 1, 2 * dilation_size + 1),
			Point(dilation_size, dilation_size));
	/// Apply the dilation operation
	dilate(image, image, element);
	imshow("inter", image);
	std::vector<std::vector<Point> > blobs;
	findBlobs(image, blobs);
	int avgSize = 0;
	for (int i = 0; i < blobs.size(); i++) {
		avgSize += blobs[i].size();
	}
	avgSize = avgSize / blobs.size();
	int removed = 0;
	for (int i = 0; i < blobs.size(); i++) {
		if (blobs[i].size() < avgSize / 10) {
			removed++;
			for (int j = 0; j < blobs[i].size(); j++) {
				image.at<uchar>(blobs[i][j]) = 0;
			}
		}
	}

}
#endif /* UTIL_H_ */

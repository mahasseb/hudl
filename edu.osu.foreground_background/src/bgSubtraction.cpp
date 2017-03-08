/*
 * bgSubtraction.cpp
 *
 *  Created on: Jan 25, 2015
 *      Author: behrooz
 */
#include "util.h"

/**
 * Difference between minimum value of the normalized patches
 */
float method1(Mat backgroundPatch, Mat warppedFramePatch) {

	Mat normalizedFramePatch;
	Mat grayBackgroundPatch;
	cvtColor(warppedFramePatch, normalizedFramePatch, CV_BGR2GRAY);
	cvtColor(backgroundPatch, grayBackgroundPatch, CV_BGR2GRAY);

	normalizedFramePatch.convertTo(normalizedFramePatch, CV_64F);
	grayBackgroundPatch.convertTo(grayBackgroundPatch, CV_64F);
	float z = sum(normalizedFramePatch).val[0];
	if (z != 0)
		normalizedFramePatch = normalizedFramePatch / z;
	vector<Mat> patches = scan(grayBackgroundPatch, warppedFramePatch);
	float minDifference = numeric_limits<float>::max();

	for (int i = 0; i < patches.size(); i++) {

		Mat normalizedPatch;
		normalizedPatch = patches[i];
		float z1 = sum(patches[i]).val[0];
		if (z1 != 0) {
			normalizedPatch = patches[i] / z1;
		}
		double min1, max1, min2, max2;
		minMaxLoc(normalizedPatch, &min1, &max1);
		minMaxLoc(normalizedFramePatch, &min2, &max2);
		float difference = abs(min2 - min1);
		if (difference < minDifference) {
			minDifference = difference;
		}
	}
	return minDifference;
}

/**
 * Difference between minimum value of the (not normalized) patches
 */
float method2(Mat backgroundPatch, Mat warppedFramePatch) {

	Mat normalizedFramePatch;
	Mat grayBackgroundPatch;
	cvtColor(warppedFramePatch, normalizedFramePatch, CV_BGR2GRAY);
	cvtColor(backgroundPatch, grayBackgroundPatch, CV_BGR2GRAY);

	vector<Mat> patches = scan(grayBackgroundPatch, warppedFramePatch);
	float minDifference = numeric_limits<float>::max();
	for (int i = 0; i < patches.size(); i++) {
		Mat normalizedPatch;
		normalizedPatch = patches[i];
		double min1, max1, min2, max2;
		minMaxLoc(normalizedPatch, &min1, &max1);
		minMaxLoc(normalizedFramePatch, &min2, &max2);
		float difference = abs(min2 - min1);
		if (difference < minDifference) {
			minDifference = difference;
		}
	}
	return minDifference / 255;
}
/**
 * Difference between average value of the (not normalized) patches
 */
float method3(Mat backgroundPatch, Mat warppedFramePatch) {
	Mat normalizedFramePatch;
	Mat grayBackgroundPatch;
	cvtColor(warppedFramePatch, normalizedFramePatch, CV_BGR2GRAY);
	cvtColor(backgroundPatch, grayBackgroundPatch, CV_BGR2GRAY);
	float foregroundPatchAvg = sum(warppedFramePatch).val[0]
			/ (warppedFramePatch.cols * warppedFramePatch.rows);
	vector<Mat> patches = scan(grayBackgroundPatch, warppedFramePatch);
	float minDifference = numeric_limits<float>::max();

	for (int i = 0; i < patches.size(); i++) {

		Mat normalizedPatch;
		normalizedPatch = patches[i];
		float backgroundPatchAvg = sum(normalizedPatch).val[0]
				/ (normalizedPatch.cols * normalizedPatch.rows);
		float difference = abs(foregroundPatchAvg - backgroundPatchAvg);
		if (difference < minDifference) {
			minDifference = difference;
		}
	}
	return minDifference / 255;
}

/**
 * Difference between average rgb value of the patches
 */
float method4(Mat backgroundPatch1, Mat warppedFramePatch1,
		Mat backgroundPatch2, Mat warppedFramePatch2, Mat backgroundPatch3,
		Mat warppedFramePatch3) {

	vector<Mat> patches1 = scan(backgroundPatch1, warppedFramePatch1);
	vector<Mat> patches2 = scan(backgroundPatch2, warppedFramePatch2);
	vector<Mat> patches3 = scan(backgroundPatch3, warppedFramePatch3);

	int z = warppedFramePatch1.cols * warppedFramePatch1.rows;
	vector<double> avgRGBFrame;
	avgRGBFrame.push_back(sum(warppedFramePatch1).val[0] / z);
	avgRGBFrame.push_back(sum(warppedFramePatch2).val[0] / z);
	avgRGBFrame.push_back(sum(warppedFramePatch3).val[0] / z);

	float minDifference = numeric_limits<float>::max();
	for (int i = 0; i < patches1.size(); i++) {

		vector<double> avgRGBPatch;
		avgRGBPatch.push_back(sum(patches1[i]).val[0] / z);
		avgRGBPatch.push_back(sum(patches2[i]).val[0] / z);
		avgRGBPatch.push_back(sum(patches3[i]).val[0] / z);

		float difference = sqrt(
				pow(avgRGBFrame[0] - avgRGBPatch[0], 2)
						+ pow(avgRGBFrame[1] - avgRGBPatch[1], 2)
						+ pow(avgRGBFrame[2] - avgRGBPatch[2], 2));
		if (difference < minDifference) {
			minDifference = difference;
		}
	}
	return minDifference / 442;
}

/**
 * Euclidean distance between color histograms
 */

float method5(Mat backgroundPatch1, Mat warppedFramePatch1,
		Mat backgroundPatch2, Mat warppedFramePatch2, Mat backgroundPatch3,
		Mat warppedFramePatch3) {
	vector<Mat> patches1 = scan(backgroundPatch1, warppedFramePatch1);
	vector<Mat> patches2 = scan(backgroundPatch2, warppedFramePatch2);
	vector<Mat> patches3 = scan(backgroundPatch3, warppedFramePatch3);

	vector<float> fHistrogram(30);
	vector<float> histogram;
	histogram = getSingleChannelHistogram(warppedFramePatch1);
	copy(histogram.begin(), histogram.begin() + histogram.size(),
			fHistrogram.begin());
	histogram = getSingleChannelHistogram(warppedFramePatch2);
	copy(histogram.begin(), histogram.begin() + histogram.size(),
			fHistrogram.begin() + 10);
	histogram = getSingleChannelHistogram(warppedFramePatch3);
	copy(histogram.begin(), histogram.begin() + histogram.size(),
			fHistrogram.begin() + 20);

	float minDifference = numeric_limits<float>::max();
	for (int i = 0; i < patches1.size(); i++) {
		vector<float> bHistrogram(30);
		histogram = getSingleChannelHistogram(patches1[i]);
		copy(histogram.begin(), histogram.begin() + histogram.size(),
				bHistrogram.begin());

		histogram = getSingleChannelHistogram(patches2[i]);
		copy(histogram.begin(), histogram.begin() + histogram.size(),
				bHistrogram.begin() + 10);

		histogram = getSingleChannelHistogram(patches3[i]);
		copy(histogram.begin(), histogram.begin() + histogram.size(),
				bHistrogram.begin() + 20);

		float difference = 0;
		for (int j = 0; j < fHistrogram.size(); j++) {
			difference += pow(bHistrogram[j] - fHistrogram[j], 2);
		}
		difference = sqrt(difference);
		if (difference < minDifference) {
			minDifference = difference;
		}
	}

	return minDifference;
}

/**
 * Background Subtraction
 */
int backgroundSubtraction(int argc, char **argv) {
	int algo = atoi(argv[4]);
	s = atoi(argv[5]);
	t = atof(argv[6]);
	vector<Mat> homograpyMatrices = readHomographyMatrices(argv[2]);
	Mat panorama = imread(argv[3]);
	vector<Mat> panoramaChannels;
	split(panorama, panoramaChannels);

	VideoCapture videoCapture;
	videoCapture.open(argv[1]);
	Mat frame;
	videoCapture >> frame;
	int index = 0;
	int pK = s * k;
	VideoWriter videoWriter;

	while (!frame.empty()) {
		Mat result;
		int width = frame.cols * 2;
		int height = frame.rows * 1;
		if (index == 0) {
			stringstream ss;
			ss << argv[7] << "_result_method_" << algo << ".avi";
			videoWriter.open(ss.str(), CV_FOURCC('X', 'V', 'I', 'D'), 30,
					Size(width, height));

		}

		if (algo <= 5) {
			Mat warpped;
			warpPerspective(frame, warpped, homograpyMatrices[index],
					panorama.size());
			vector<Mat> warppedChannels;
			split(warpped, warppedChannels);
			vector<Point2f> points1, points2;
			points1.push_back(Point2f(0, 0));
			points1.push_back(Point2f(0, frame.rows));
			points1.push_back(Point2f(frame.cols, 0));
			points1.push_back(Point2f(frame.cols, frame.rows));
			perspectiveTransform(points1, points2, homograpyMatrices[index]);

			int ypad1, ypad2, xpad1, xpad2;

			if (points2[0].x < points2[1].x)
				xpad1 = points2[0].x;
			else
				xpad1 = points2[1].x;

			if (points2[2].x > points2[3].x)
				xpad2 = points2[2].x;
			else
				xpad2 = points2[3].x;

			if (points2[0].y < points2[2].y)
				ypad1 = points2[0].y;
			else
				ypad1 = points2[2].y;

			if (points2[1].y > points2[3].y)
				ypad2 = points2[1].y;
			else
				ypad2 = points2[3].y;

			int pK = s * k;
			cout << "Frame:" << index << endl;
			result = Mat::zeros(panorama.size(), CV_8U);
			for (int y = ypad1; y < ypad2; y++) {
				for (int x = xpad1; x < xpad2; x++) {
					Mat wBox1, wBox2, wBox3, pBox1, pBox2, pBox3;
					Mat wBox, pBox;
					if (algo > 3) {
						wBox1 = Mat(warppedChannels[0],
								Rect(x - k / 2, y - k / 2, k, k)).clone();
						pBox1 = Mat(panoramaChannels[0],
								Rect(x - pK / 2, y - pK / 2, pK, pK)).clone();
						wBox2 = Mat(warppedChannels[1],
								Rect(x - k / 2, y - k / 2, k, k)).clone();
						pBox2 = Mat(panoramaChannels[1],
								Rect(x - pK / 2, y - pK / 2, pK, pK)).clone();
						wBox3 = Mat(warppedChannels[2],
								Rect(x - k / 2, y - k / 2, k, k)).clone();
						pBox3 = Mat(panoramaChannels[2],
								Rect(x - pK / 2, y - pK / 2, pK, pK)).clone();
					} else {
						wBox =
								Mat(warpped, Rect(x - k / 2, y - k / 2, k, k)).clone();
						pBox = Mat(panorama,
								Rect(x - pK / 2, y - pK / 2, pK, pK)).clone();
					}

					float distance;
					switch (algo) {
					case 1:
						distance = method1(pBox, wBox);
						break;
					case 2:
						distance = method2(pBox, wBox);
						break;
					case 3:
						distance = method3(pBox, wBox);
						break;
					case 4:
						distance = method4(pBox1, wBox1, pBox2, wBox2, pBox3,
								wBox3);
						break;
					case 5:
						distance = method5(pBox1, wBox1, pBox2, wBox2, pBox3,
								wBox3);
						break;
					}
					if (distance > t) {
						result.at<uchar>(y, x) = 255;
					}
				}
			}
			Mat unwarpped;
			warpPerspective(result, unwarpped, homograpyMatrices[index].inv(),
					frame.size());
			unwarpped.copyTo(result);
		} else {
			Mat warppedPanorama;
			warpPerspective(panorama, warppedPanorama,
					homograpyMatrices[index].inv(), frame.size());

			drwnFilterBankResponse panoramFilters;
			vector<cv::Mat> hogResponse1;
			drwnHOGFeatures hogFeatures1;
			hogFeatures1.computeDenseFeatures(warppedPanorama, hogResponse1);
			panoramFilters.addResponseImages(hogResponse1);

			//		drwnLBPFilterBank lbpFilterBank1(false);
			//		vector<cv::Mat> lbpResponse1;
			//		lbpFilterBank1.filter(warppedPanorama, lbpResponse1);
			//		panoramFilters.addResponseImages(lbpResponse1);

			drwnTextonFilterBank filterBank1(SuperPixel::FILTER_BANDWIDTH);
			vector<cv::Mat> textonResponse1;
			filterBank1.filter(warppedPanorama, textonResponse1);
			panoramFilters.addResponseImages(textonResponse1);

			drwnFilterBankResponse frameFilters;
			vector<cv::Mat> hogResponse2;
			drwnHOGFeatures hogFeatures2;
			hogFeatures2.computeDenseFeatures(frame, hogResponse2);
			frameFilters.addResponseImages(hogResponse2);

			//		drwnLBPFilterBank lbpFilterBank2(false);
			//		vector<cv::Mat> lbpResponse2;
			//		lbpFilterBank2.filter(frame, lbpResponse2);
			//		frameFilters.addResponseImages(lbpResponse2);

			drwnTextonFilterBank filterBank2(SuperPixel::FILTER_BANDWIDTH);
			vector<cv::Mat> textonResponse2;
			filterBank2.filter(frame, textonResponse2);
			frameFilters.addResponseImages(textonResponse2);
			double maxD = numeric_limits<double>::min();
			cout << "Frame:" << index << endl;
			result = Mat::zeros(frame.size(), CV_8U);
			for (int y = k / 2; y < frame.rows - k / 2; y++) {
				for (int x = k / 2; x < frame.cols - k / 2; x++) {
					vector<double> features1(panoramFilters.size());
					Eigen::Map < VectorXd
							> (&features1[0], panoramFilters.size()) =
							panoramFilters.mean(x - k / 2, y - k / 2, k, k);
					vector<double> features2(frameFilters.size());
					Eigen::Map < VectorXd > (&features2[0], frameFilters.size()) =
							frameFilters.mean(x - k / 2, y - k / 2, k, k);

					Vec3b color1 = warppedPanorama.at<Vec3b>(y, x);
					Vec3b color2 = frame.at<Vec3b>(y, x);
					//				cout<<distance<<endl;
					//				features1.push_back(color1.val[0] / 255);
					//				features1.push_back(color1.val[1] / 255);
					//				features1.push_back(color1.val[2] / 255);
					//				features2.push_back(color2.val[0] / 255);
					//				features2.push_back(color2.val[1] / 255);
					//				features2.push_back(color2.val[2] / 255);

					float distance = 0;
					for (int i = 0; i < features1.size(); i++) {
						distance += pow(features1[i] - features2[i], 2);
					}

					if (distance > maxD)
						maxD = distance;
					if (distance > t) {
						result.at<uchar>(y, x) = 255;
					}
				}
			}
//			result = 255 / maxD * result;
//			imshow("Warpped Panorama", warppedPanorama);
//			imwrite("wpano.jpg", warppedPanorama);
//			imwrite("frame.jpg", frame);
		}
//		medianBlur(result, result, 7);
//		fillHoles(result);

		postprocess(result);
		stringstream kk;
		kk << argv[7] << "_mask_" << (index + 1) << ".bmp";
		imwrite(kk.str(), result);

//		cvtColor(result, result, CV_GRAY2BGR);
//		imshow("result", result);
//		imshow("Frame", frame);
//		kk << argv[7] << "_result_" << ".bmp";
//		imwrite(kk.str(), result);
//		imwrite("frame.jpg", frame);

//		cvWaitKey();
//		Mat wFrame = Mat(Size(width, height), frame.type());
//		frame.copyTo(Mat(wFrame, Rect(0, 0, width / 2, height)));
//		result.copyTo(Mat(wFrame, Rect(width / 2, 0, width / 2, height)));
//		videoWriter.write(wFrame);

		videoCapture >> frame;

		index++;
	}
	videoWriter.release();
	return 0;
}
int main(int argc, char **argv) {
	/**
	 * argv[1] --> Video Name
	 * argv[2] --> Homography Matrices
	 * argv[3] --> Panorama image
	 * argv[4] --> Method
	 * argv[5] --> Scale
	 * argvp6] --> Threshold
	 * argv[7] --> Image Name
	 *
	 */

	backgroundSubtraction(argc, argv);
}

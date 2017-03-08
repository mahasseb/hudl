/*
 * MachineLearningUtil.h
 *
 *  Created on: Mar 30, 2012
 *      Author: behrooz
 */

#ifndef MACHINELEARNINGUTIL_H_
#define MACHINELEARNINGUTIL_H_
/**
 * OpenCV libs
 */
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include <ml.h>
using namespace cv;
namespace vision {

struct trainingElement {
	vector<float> features;
	int label;
};
class MachineLearningUtil {
private:
//	Mat pcaMean;
	Mat pcaEigenVectors;
	Mat pcaEigenValues;
	PCA pca;
	KNearest knn;
public:
	MachineLearningUtil();
	virtual ~MachineLearningUtil();
	/**
	 * PCA Related Methods
	 */
	void computePCA(vector<Mat> images, unsigned int n);
	Mat projectToPCASpace(const Mat& src);
	Mat getEigenVectorsOfPCA();
	Mat getEigenValuesOfPCA();
	/**
	 * KNN Related Methods
	 */
	void learnKNearestNeighbour(Mat trainingData, Mat trainingClasses);
	int findNearest(unsigned int k, Mat input);

	/**
	 * SVM Related
	 */
	void learnSVM(vector<trainingElement> trainingElements, string modelFileName);
	/**
	 * General Static Methods
	 */
	static Mat asColumnMatrix(const vector<Mat>& src, int type);
	static Mat asRowMatrix(const vector<Mat>& src, int type);
	static Mat transpose(const Mat& src);

};

} /* namespace vision */
#endif /* MACHINELEARNINGUTIL_H_ */

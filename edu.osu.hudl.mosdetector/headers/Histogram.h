/*
 * Histogram.h
 *
 *  Created on: Apr 26, 2012
 *      Author: behrooz
 */

#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_
#include "CommonLibs.h"
using namespace std;

namespace vision {

class Histogram {
private:
	unsigned int numberOfSubHistograms;
	int numberOfBins;
	float *histogram;
	vector<Histogram> subHistograms;
	void recalculateHistogram();
public:
	Histogram();
	virtual ~Histogram();
	void init(int numberOfBins, float* histogram);
	void init(vector<Histogram> subHistograms, int numberOfSubHistograms);
	void init(int numberOfSubHistograms);
	void shiftAndAdd(Histogram subHistogram);
	void shiftAndAdd(vector<Histogram> subHistograms);
	float* getHistogram();
	int getNumberOfBins();
};

} /* namespace vision */
#endif /* HISTOGRAM_H_ */

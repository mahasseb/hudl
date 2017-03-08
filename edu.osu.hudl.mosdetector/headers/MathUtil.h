/*
 * MathUtil.h
 *
 *  Created on: Apr 14, 2012
 *      Author: behrooz
 */

#ifndef MATHUTIL_H_
#define MATHUTIL_H_

#include "CommonLibs.h"
#include "DataTypes.h"
using namespace std;
namespace vision {

class MathUtil {
public:
	MathUtil();
	virtual ~MathUtil();
	static float calculateEuclideanDistance(vector<float> vector1,
			vector<float> vector2);

	static float calculateEuclideanDistance(float ax, float ay, float bx,
			float by);
	static float calculateEuclideanDistance(float cx, float cy, float ax,
			float ay, float bx, float by);

	static float getY(Line line, float x);
	static float getX(Line line, float y);
};

} /* namespace vision */
#endif /* MATHUTIL_H_ */

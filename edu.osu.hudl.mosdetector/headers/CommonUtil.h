/*
 * CommonUtil.h
 *
 *  Created on: Apr 15, 2012
 *      Author: behrooz
 */

#ifndef COMMONUTIL_H_
#define COMMONUTIL_H_
#include "CommonLibs.h"
#include "DataTypes.h"
using namespace std;

namespace vision {

class CommonUtil {
public:
	CommonUtil();
	virtual ~CommonUtil();
	static File getFileStructure(string path);
};

} /* namespace vision */
#endif /* COMMONUTIL_H_ */

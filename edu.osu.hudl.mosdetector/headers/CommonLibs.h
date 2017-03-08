/*
 * CommonLibs.h
 *
 *  Created on: Mar 30, 2012
 *      Author: behrooz
 */

#ifndef COMMONLIBS_H_
#define COMMONLIBS_H_

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
#include <map>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#define PI 3.14159265
#define FILE_SPERATOR "/"
#ifdef __WIN32__
#undef FILE_SPERATOR
#define FILE_SPERATOR "\\"
#endif

#endif /* COMMONLIBS_H_ */

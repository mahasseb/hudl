/*
 * HUDLDataTypes.cpp
 *
 *  Created on: Apr 24, 2012
 *      Author: behrooz
 */

#include "HUDLDataTypes.h"
namespace hudl {
LineOfScrimmage::LineOfScrimmage() {

}

LineOfScrimmage::~LineOfScrimmage() {

}
HUDLWindow::HUDLWindow() {
	// TODO Auto-generated constructor stub
	degree = 0;
}

HUDLWindow::~HUDLWindow() {
	// TODO Auto-generated destructor stub
	opticalflow.release();
}

VideoInterval::VideoInterval() {
	motion = false;
}

VideoInterval::~VideoInterval() {

}

Frame::Frame() {

}

Frame::~Frame() {
	windows.clear();
}

} // namespace hudl


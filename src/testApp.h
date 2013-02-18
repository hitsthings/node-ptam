#pragma once


#include <cvd/image.h>
#include <cvd/byte.h>
#include <TooN/TooN.h>

#include "System.h"
typedef System PTAMthread;

class testApp {
public:
	void setup();
	void update();

	void GetAndFillFrameBWandRGB(CVD::Image<CVD::byte> &imBW,
								 CVD::Image<CVD::Rgb<CVD::byte> > &imRGB);
		
	PTAMthread ptam;
	CVD::ImageRef videoSize;
	
};

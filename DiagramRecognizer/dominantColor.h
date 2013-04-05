#pragma once

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdlib.h>
#include <stdio.h>

#include <vector>
#include <algorithm>


class dominantColor
{
public:
	dominantColor(IplImage *image);
	CvScalar domColor;

private:
	int getPixelColorType(int H, int S, int V);
	static bool colors_sort(std::pair< int, uint > a, std::pair< int, uint > b);
};

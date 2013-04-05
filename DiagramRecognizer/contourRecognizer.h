#pragma once
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdlib.h>
#include <stdio.h>
#include <QVector>
#include <QPoint>
#include "dominantColor.h"

class contourRecognizer
{
public:
	contourRecognizer();
	int getContour();
private:
	IplImage *mImage, *mResizedImage, *mGray, *mDst;
};

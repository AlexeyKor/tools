#include "contourRecognizer.h"
#include "stdio.h"

contourRecognizer::contourRecognizer()
{
	mImage = 0;
	mResizedImage = 0;
	mGray = 0;
	mDst = 0;
}

int contourRecognizer::getContour()
{ 
	mImage = cvLoadImage("contour.jpg", 1);

	assert( mImage != 0 );

	//resize original image
	int factor;
	if(mImage->height > mImage->width)
		factor = (int) mImage->height / 700;
	else
		factor = (int) mImage->width / 700;

	mResizedImage = cvCreateImage( cvSize(mImage->width / factor, mImage->height / factor), mImage->depth, mImage->nChannels );
	cvResize(mImage, mResizedImage, 3);

	CvScalar pixel;
	dominantColor mDomColor(mResizedImage);
	bool isOnlyDiagram = false;
	int left = 0, right = mResizedImage->width - 1, top = 0, bottom = mResizedImage->height - 1;

	/*
	while(!isOnlyDiagram && left <= right)
		{
			isOnlyDiagram = true;

			for(int i = 0; i < mResizedImage->height; i++)
			{
				pixel = cvGet2D(mResizedImage, i, left);
				for(int j = 0; j < 2; j++)
					if (pixel.val[j] < mDomColor.domColor.val[j] - 30 || pixel.val[j] > mDomColor.domColor.val[j] + 30 )
					{
						pixel.val[0] = mDomColor.domColor.val[0];
						pixel.val[1] = mDomColor.domColor.val[1];
						pixel.val[2] = mDomColor.domColor.val[2];
						cvSet2D(mResizedImage, i, left, pixel);
						isOnlyDiagram = false;
						break;
					}
			}

			left++;
		}*/

	while(!isOnlyDiagram)
	{
		isOnlyDiagram = true;
		pixel = cvGet2D(mResizedImage, mResizedImage->height/2, left);
		for(int j = 0; j < 2; j++)
			if (pixel.val[j] < mDomColor.domColor.val[j] - 30 || pixel.val[j] > mDomColor.domColor.val[j] + 30 )
			{
				left++;
				isOnlyDiagram = false;
				break;
			}
	}

	isOnlyDiagram = false;

	while(!isOnlyDiagram)
	{
		isOnlyDiagram = true;
		pixel = cvGet2D(mResizedImage, mResizedImage->height/2, right);
		for(int j = 0; j < 2; j++)
			if (pixel.val[j] < mDomColor.domColor.val[j] - 30 || pixel.val[j] > mDomColor.domColor.val[j] + 30 )
			{
				right--;
				isOnlyDiagram = false;
				break;
			}
	}

	isOnlyDiagram = false;

	while(!isOnlyDiagram)
	{
		isOnlyDiagram = true;
		pixel = cvGet2D(mResizedImage, top, mResizedImage->width/2);
		for(int j = 0; j < 2; j++)
			if (pixel.val[j] < mDomColor.domColor.val[j] - 30 || pixel.val[j] > mDomColor.domColor.val[j] + 30 )
			{
				top++;
				isOnlyDiagram = false;
				break;
			}
	}

	isOnlyDiagram = false;

	while(!isOnlyDiagram)
	{
		isOnlyDiagram = true;
		pixel = cvGet2D(mResizedImage, bottom, mResizedImage->width/2);
		for(int j = 0; j < 2; j++)
			if (pixel.val[j] < mDomColor.domColor.val[j] - 30 || pixel.val[j] > mDomColor.domColor.val[j] + 30 )
			{
				bottom--;
				isOnlyDiagram = false;
				break;
			}
	}

	cvSetImageROI(mResizedImage, cvRect(left + 10, top + 10, right - left - 20, bottom - top - 20));

	// create 1 chanel images
	mGray = cvCreateImage( cvGetSize(mResizedImage), IPL_DEPTH_8U, 1 );
	mDst = cvCreateImage( cvGetSize(mResizedImage), IPL_DEPTH_8U, 1 );

	// convert to grayscale
	cvCvtColor(mResizedImage, mGray, CV_RGB2GRAY);

	// get borders
	cvCanny(mGray, mDst, 90, 100, 3);

	// dilate image to merge borders
	cvDilate(mDst, mDst);

	//even after dilation contour may have some black pixels. These cycles may be written in extern function
	QVector<CvScalar> badPixels;
	QVector<QPoint> badPixelsXY;

	for(int i = 0; i < mDst->height; i++)
	{
		for(int j = 0; j < mDst->width; j++)
		{
			pixel = cvGet2D(mDst, i, j);
			if (pixel.val[0] == 0)
			{
				badPixels.append(pixel);
				badPixelsXY.append(QPoint(i,j));

			}
			else if(badPixels.size())
			{
				//if some pixels didn't disappear increase limit of badPixels.size
				if(badPixels.size() <= 2)
				{
					for(int k = 0; k < badPixels.size(); k++)
					{
						badPixels[k].val[0] = 255.0;
						cvSet2D(mDst, badPixelsXY[k].x(), badPixelsXY[k].y(), badPixels[k]);
					}
					badPixels.clear();
					badPixelsXY.clear();
				}
				else
				{
					badPixels.clear();
					badPixelsXY.clear();
				}
			}
		}
	}

	for(int j = 0; j < mDst->width; j++)
	{
		for(int i = 0; i < mDst->height; i++)
		{
			pixel = cvGet2D(mDst, i, j);
			if (pixel.val[0] == 0)
			{
				badPixels.append(pixel);
				badPixelsXY.append(QPoint(i,j));

			}
			else if(badPixels.size())
			{
				//if some pixels didn't disappear increase limit of badPixels.size
				if(badPixels.size() <= 2)
				{
					for(int k = 0; k < badPixels.size(); k++)
					{
						badPixels[k].val[0] = 255.0;
						cvSet2D(mDst, badPixelsXY[k].x(), badPixelsXY[k].y(), badPixels[k]);
					}
					badPixels.clear();
					badPixelsXY.clear();
				}
				else
				{
					badPixels.clear();
					badPixelsXY.clear();
				}
			}
		}
	}

	//erode image to make contour thiner
	cvErode(mDst, mDst);

	cvSaveImage("contour.jpg" , mDst);

	// cleaning
	cvReleaseImage(&mImage);
	cvReleaseImage(&mResizedImage);
	cvReleaseImage(&mGray);
	cvReleaseImage(&mDst);

	return 0;
}

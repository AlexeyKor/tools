#include "dominantColor.h"

// getting image pixel
#define CV_PIXEL(type,img,x,y) (((type*)((img)->imageData+(y)*(img)->widthStep))+(x)*(img)->nChannels)

// Various color types
//          0                   1         2              3              4                5                6             7               8               9                       10
enum {cBLACK=0, cWHITE, cGREY, cRED, cORANGE, cYELLOW, cGREEN, cAQUA, cBLUE, cPURPLE, NUM_COLOR_TYPES};
char* sCTypes[NUM_COLOR_TYPES] = {"Black", "White","Grey","Red","Orange","Yellow","Green","Aqua","Blue","Purple"};
uchar cCTHue[NUM_COLOR_TYPES] =    {0,       0,      0,     0,     20,      30,      60,    85,   120,    138  };
uchar cCTSat[NUM_COLOR_TYPES] =    {0,       0,      0,    255,   255,     255,     255,   255,   255,    255  };
uchar cCTVal[NUM_COLOR_TYPES] =    {0,      255,    120,   255,   255,     255,     255,   255,   255,    255  };

typedef unsigned int uint;

// Quantity of pixels of different colors
uint colorCount[NUM_COLOR_TYPES] = {0,          0,              0,              0,              0,              0,              0,              0,              0,              0 };

bool dominantColor::colors_sort(std::pair< int, uint > a, std::pair< int, uint > b)
{
	return (a.second > b.second);
}

int dominantColor::getPixelColorType(int H, int S, int V)
{
	int color = cBLACK;


	if (V < 75)
		color = cBLACK;
	else if (V > 190 && S < 27)
		color = cWHITE;
	else if (S < 53 && V < 185)
		color = cGREY;
	else

	{
		if (H < 7)
			color = cRED;
		else if (H < 25)
			color = cORANGE;
		else if (H < 34)
			color = cYELLOW;
		else if (H < 73)
			color = cGREEN;
		else if (H < 102)
			color = cAQUA;
		else if (H < 140)
			color = cBLUE;
		else if (H < 170)
			color = cPURPLE;
		else    // full circle
			color = cRED;   // back to Red
	}
	return color;
}

dominantColor::dominantColor(IplImage *image)
{
	// for image storing
	IplImage *hsv=0;

	// transform image into HSV

	hsv = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 3 );
	cvCvtColor( image, hsv, CV_BGR2HSV );

	// for storing RGB colors
	CvScalar rgb_colors[NUM_COLOR_TYPES];

	int i=0, x=0, y=0;

	// clear colors
	for(i=0; i<NUM_COLOR_TYPES; i++) {
		rgb_colors[i] = cvScalarAll(0);
	}

	for (y=0; y<hsv->height; y++) {
		for (x=0; x<hsv->width; x++) {

			// get HSV pixel components
			uchar H = CV_PIXEL(uchar, hsv, x, y)[0];        // Hue
			uchar S = CV_PIXEL(uchar, hsv, x, y)[1];        // Saturation
			uchar V = CV_PIXEL(uchar, hsv, x, y)[2];        // Value (Brightness)

			// color detection
			int ctype = getPixelColorType(H, S, V);

			// getting RGB components
			rgb_colors[ctype].val[0] += CV_PIXEL(uchar, image, x, y)[0]; // B
			rgb_colors[ctype].val[1] += CV_PIXEL(uchar, image, x, y)[1]; // G
			rgb_colors[ctype].val[2] += CV_PIXEL(uchar, image, x, y)[2]; // R

			colorCount[ctype]++;
		}
	}

	// averaging RGB components
	for(i=0; i<NUM_COLOR_TYPES; i++) {
		rgb_colors[i].val[0] /= colorCount[i];
		rgb_colors[i].val[1] /= colorCount[i];
		rgb_colors[i].val[2] /= colorCount[i];
	}

	// write in array
	std::vector< std::pair< int, uint > > colors;
	colors.reserve(NUM_COLOR_TYPES);

	for(i=0; i<NUM_COLOR_TYPES; i++){
		std::pair< int, uint > color;
		color.first = i;
		color.second = colorCount[i];
		colors.push_back( color );
	}
	// sort
	std::sort( colors.begin(), colors.end(), colors_sort );

	domColor = rgb_colors[colors[0].first];

	// for debugging - print codes, names of colors and their quantity
	for(i=0; i<colors.size(); i++){
		printf("[i] color %d (%s) - %d\n", colors[i].first, sCTypes[colors[i].first], colors[i].second );
	}

	// clean resources
	cvReleaseImage(&hsv);
}

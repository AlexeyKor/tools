#pragma once

#include <QMainWindow>
#include "bitmap.h"
#include "formSegmentator.h"
#include "abstractRecognizer.h"
#include "output.h"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "contourRecognizer.h"

namespace Ui {
class mainWindow;
}

class mainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit mainWindow(QWidget *parent = 0);
	~mainWindow();

public slots:
	void clear();
	void recognize();
	void recognizeImage();
	void showStage(int);
signals:
	void print(PathVector mDiagram, Bitmap * mBitmap, FormSegmentator *mFormSegmentator, int stageNum);

private:
	void showInput();
	void showContour();
	void showUniStage();
	void showBitmap();
	void showComponents();
	void clearScene();
	QGraphicsPixmapItem *inputContour, *inputImage;
	Output *printedDiagram;
	Ui::mainWindow *ui;
	void recognizeDiagram();
	PathVector mDiagram;
	Bitmap * mBitmap;
	FormSegmentator *mFormSegmentator;
	bool mRecognized;
	QPoint mComponentPoint;
	GesturesManager *mGesturesManager;
	AbstractRecognizer *mAbstractRecognizer;
	QGraphicsScene *scene;
};

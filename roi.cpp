#include "stdafx.h"
#include "obraz.h"
#include "roi.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

ROI::ROI()
{
	gorny_rog = Point(0, 0);
	dolny_rog = Point(0, 0);
}

ROI::ROI(Point g_rog, Point d_rog, Mat src)
{
	gorny_rog = g_rog;
	dolny_rog = d_rog;
	kolor = Scalar(200, 244, 0);
	roi_ptr = src(Rect(g_rog.x, g_rog.y, d_rog.x - g_rog.x, d_rog.y - g_rog.y));	//wspó³rzêdne kwadracika próbki

}


void ROI::rysuj_kwadrat(Mat src)
{
	rectangle(src, gorny_rog, dolny_rog, kolor, 2);

}

ROI::~ROI()
{
}

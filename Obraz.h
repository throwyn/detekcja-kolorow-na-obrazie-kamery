#pragma once
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;
class Obraz
{
public:
	Obraz(int webCamera);
	~Obraz();
	Mat srcLR;			//obraz pomniejszony
	Mat src;			//obraz oryginalny
	Mat bw;				//black white
	vector<Mat> bwLista;	//black white lista
	VideoCapture cap;
};


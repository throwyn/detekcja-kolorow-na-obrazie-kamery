#include "stdafx.h"
#include "Obraz.h"
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

Obraz::Obraz(int webCamera)
{
	cap = VideoCapture(webCamera);
}

Obraz::~Obraz()
{
}

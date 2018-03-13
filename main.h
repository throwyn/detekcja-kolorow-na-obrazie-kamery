#pragma once
#include "stdafx.h"
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>


#define ilosc_ROI 7
int fontFace = FONT_HERSHEY_PLAIN;
int wielkosc_kwadratow = 20;			//wielko�� kwadrat�w
int kolor_sredni[ilosc_ROI][3];			//�redni kolor punkt�w
int dolna_granica_HLS[ilosc_ROI][3];
int gorna_granica_HLS[ilosc_ROI][3];


vector<vector<Point>> contours;
vector<Vec4i> hierarchy;				
vector <ROI> v_roi;						//wektor z kwadratami sampli

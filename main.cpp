#include "stdafx.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "Obraz.h"
#include "roi.h"
#include <vector>
#include <cmath>
#include "main.h"

using namespace cv;
using namespace std;

//wyswietlenie tekstu na obrazie
void tekst(Mat src, string tekst) {
	putText(src, tekst, Point(src.cols *0.01, src.rows *0.05), FONT_ITALIC, 1.0f, Scalar(0, 255, 0), 1);
}
//tworzy i rysuje kwadraty ROI
void kwadraty(Obraz* o) {
	o->cap >> o->src;				//nastepna klatka
	flip(o->src, o->src, 1);		//dowrócenie macierzy obrazu
	
	//tworzenie obiektow ROI i dodawanie do wektora
	v_roi.push_back(ROI(Point(o->src.cols / 3, o->src.rows / 6), Point(o->src.cols / 3 + wielkosc_kwadratow, o->src.rows / 6 + wielkosc_kwadratow), o->src));
	v_roi.push_back(ROI(Point(o->src.cols / 4, o->src.rows / 2), Point(o->src.cols / 4 + wielkosc_kwadratow, o->src.rows / 2 + wielkosc_kwadratow), o->src));
	v_roi.push_back(ROI(Point(o->src.cols / 3, o->src.rows / 1.5), Point(o->src.cols / 3 + wielkosc_kwadratow, o->src.rows / 1.5 + wielkosc_kwadratow), o->src));
	v_roi.push_back(ROI(Point(o->src.cols / 2, o->src.rows / 2), Point(o->src.cols / 2 + wielkosc_kwadratow, o->src.rows / 2 + wielkosc_kwadratow), o->src));
	v_roi.push_back(ROI(Point(o->src.cols / 2.5, o->src.rows / 2.5), Point(o->src.cols / 2.5 + wielkosc_kwadratow, o->src.rows / 2.5 + wielkosc_kwadratow), o->src));
	v_roi.push_back(ROI(Point(o->src.cols / 2, o->src.rows / 1.5), Point(o->src.cols / 2 + wielkosc_kwadratow, o->src.rows / 1.5 + wielkosc_kwadratow), o->src));
	v_roi.push_back(ROI(Point(o->src.cols / 2.5, o->src.rows / 1.8), Point(o->src.cols / 2.5 + wielkosc_kwadratow, o->src.rows / 1.8 + wielkosc_kwadratow), o->src));


	for (int i = 0; i<50; i++) {
		o->cap >> o->src;					//pobranie obrazu
		flip(o->src, o->src, 1);			//odwrocenie orbazu
		for (int j = 0; j<ilosc_ROI; j++) {	//rysowanie kwadratów dla ilosc_ROI (ilosci kwadratow)
			v_roi[j].rysuj_kwadrat(o->src);
		}
		string imgText = string("Przykryj obszar kwadratow dlonia");
		tekst(o->src, imgText);							//rysowanie tekstu
		imshow("Obraz z kamery", o->src);				//pokazanie obrazu
		if (waitKey(40) >= 0) break;					//przy przycisnieciu wylacza 
	}
}

//obliczanie mediany
int getMedian(vector<int> val) {
	int median;
	size_t size = val.size();
	sort(val.begin(), val.end());
	if (size % 2 == 0) {
		median = val[size / 2 - 1];
	}
	else {
		median = val[size / 2];
	}
	return median;
}


void mediana_HLS(ROI roi, int avg[3]) {
	Mat r;
	roi.roi_ptr.copyTo(r);		//kopia wskaznika 
	vector<int>hm, sm, lm;		//wektory sk³adowych obrazu HLS
								//wype³nianie wektorów 
	for (int i = 2; i<r.rows - 2; i++) {
		for (int j = 2; j<r.cols - 2; j++) {
			hm.push_back(r.data[r.channels()*(r.cols*i + j) + 0]);
			sm.push_back(r.data[r.channels()*(r.cols*i + j) + 1]);
			lm.push_back(r.data[r.channels()*(r.cols*i + j) + 2]);
		}
	}
	//obliczanie mediany
	avg[0] = getMedian(hm);
	avg[1] = getMedian(sm);
	avg[2] = getMedian(lm);
}

//znajdowanie 
void sr_kolor(Obraz *o) {
	for (int i = 0; i<30; i++) {
		o->cap >> o->src;
		flip(o->src, o->src, 1);

		for (int j = 0; j<ilosc_ROI; j++) {
			v_roi[j].rysuj_kwadrat(o->src);			//rysuje nowy kwadrat
		}
		string imgText = string("Obliczanie kolorow dloni");
		tekst(o->src, imgText);
		imshow("Obraz z kamery", o->src);
		if (cv::waitKey(30) >= 0) break;
	}
	cvtColor(o->src, o->src, CV_BGR2HLS);		//zmiana obrazu kamery z BGR na HLS
	for (int j = 0; j < ilosc_ROI; j++) {
		mediana_HLS(v_roi[j], kolor_sredni[j]);	//obliczanie mediany koloru
	}
	cvtColor(o->src, o->src, CV_HLS2BGR);	//zmiana obrazu kamery z HLS na BGR 
}
void ustaw_granice_HLS() {
	for (int i = 0; i<ilosc_ROI; i++) {
		dolna_granica_HLS[i][0] = 20;
		dolna_granica_HLS[i][1] = 30;
		dolna_granica_HLS[i][2] = 20;
		gorna_granica_HLS[i][0] = 20;
		gorna_granica_HLS[i][1] = 40;
		gorna_granica_HLS[i][2] = 80;
	}
}

void normalizacja_kolorow() {
	// ustawianie granic kolorów próbek skóry 
	for (int i = 0; i<ilosc_ROI; i++) {
		if ((kolor_sredni[i][0] - dolna_granica_HLS[i][0]) <0) {
			dolna_granica_HLS[i][0] = kolor_sredni[i][0];
		}if ((kolor_sredni[i][1] - dolna_granica_HLS[i][1]) <0) {
			dolna_granica_HLS[i][1] = kolor_sredni[i][1];
		}if ((kolor_sredni[i][2] - dolna_granica_HLS[i][2]) <0) {
			dolna_granica_HLS[i][2] = kolor_sredni[i][2];
		}if ((kolor_sredni[i][0] + gorna_granica_HLS[i][0]) >255) {
			gorna_granica_HLS[i][0] = 255 - kolor_sredni[i][0];
		}if ((kolor_sredni[i][1] + gorna_granica_HLS[i][1]) >255) {
			gorna_granica_HLS[i][1] = 255 - kolor_sredni[i][1];
		}if ((kolor_sredni[i][2] + gorna_granica_HLS[i][2]) >255) {
			gorna_granica_HLS[i][2] = 255 - kolor_sredni[i][2];
		}
	}
}

void binaryzacja(Obraz *o) {
	Scalar lowerBound;
	Scalar upperBound;
	ustaw_granice_HLS();	//ustawianie wartoœci akceptowalnej róznicy skadowych HLS obliczonych srednich kolorow
	normalizacja_kolorow();
	for (int i = 0; i<ilosc_ROI; i++) {	//dla wszystkich sampli kolorów
		lowerBound = Scalar(kolor_sredni[i][0] - dolna_granica_HLS[i][0], kolor_sredni[i][1] - dolna_granica_HLS[i][1], kolor_sredni[i][2] - dolna_granica_HLS[i][2]);
		upperBound = Scalar(kolor_sredni[i][0] + gorna_granica_HLS[i][0], kolor_sredni[i][1] + gorna_granica_HLS[i][1], kolor_sredni[i][2] + gorna_granica_HLS[i][2]);
		o->bwLista.push_back(Mat(o->srcLR.rows, o->srcLR.cols, CV_8U));	//dodaje obiekt do listy obrazow binarnych o danej wielkoœci 
		inRange(o->srcLR, lowerBound, upperBound, o->bwLista[i]);		//binaryzacja
	}
	o->bwLista[0].copyTo(o->bw);
	for (int i = 1; i<ilosc_ROI; i++) {	//sumaryzacja otrzymanych obrazów binarnych
		o->bw += o->bwLista[i];
	}
	medianBlur(o->bw, o->bw, 7);		//rozmycie

	imshow("Binaryzacja", o->bw);		//ukazanie obrazu
}

int findBiggestContour(vector<vector<Point> > contours) {
	int indexOfBiggestContour = -1;
	int sizeOfBiggestContour = 0;
	for (int i = 0; i < contours.size(); i++) {
		if (contours[i].size() > sizeOfBiggestContour) {
			sizeOfBiggestContour = contours[i].size();
			indexOfBiggestContour = i;
		}
	}
	return indexOfBiggestContour;
}
void szukaj_dloni(Obraz *o) {
	Mat aBw;
	int index_najwiekszego_obiektu;
	pyrUp(o->bw, o->bw);
	o->bw.copyTo(aBw);
	findContours(aBw, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);					//szukanie konturów
	index_najwiekszego_obiektu = findBiggestContour(contours);										//znajdowanie najwiêkszego konturu
	drawContours(o->src, contours, index_najwiekszego_obiektu, cv::Scalar(200, 0, 0),2,7);			//rysowanie konturu

}
  
int main(int argc, char** argv)
{
	Obraz o(0);
	o.cap >> o.src;													//pobranie kolejnej klatki
	kwadraty(&o);													//rysowanie kwadratów 
	sr_kolor(&o);													//obliczanie kolorów z kwadratów próbek
	while (1) {
		o.cap >> o.src;
		flip(o.src, o.src, 1);
		pyrDown(o.src, o.srcLR);						//pomniejszenie obrazu
		blur(o.srcLR, o.srcLR, Size(3, 3));				//zamazanie
		cvtColor(o.srcLR, o.srcLR, CV_BGR2HLS);			//BRG na HLS
		binaryzacja(&o);								//normalizuje kolory i tworzy binarne
		cvtColor(o.srcLR, o.srcLR, CV_HLS2BGR);			//HLS na BRG
		szukaj_dloni(&o);								//szukanie i rysowanie najwiêkszego konturu

		imshow("Obraz z kamery", o.src);
		if (cv::waitKey(30) == char('c')) break;


	}
	return 0;
}
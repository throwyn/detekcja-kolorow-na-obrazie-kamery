#pragma once
class ROI
{
public:
	ROI();
	ROI(Point gorny_rog, Point dolny_rog, Mat src);
	Point dolny_rog,gorny_rog;
	Mat roi_ptr;												//wyci�ty obraz
	Scalar kolor;
	void rysuj_kwadrat(Mat src);
	~ROI();
};


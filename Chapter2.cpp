#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// ----------------------------  Basic Function ----------------------

void main()
{
	string path = "Resources/cat.jpg";
	Mat img = imread(path);
	Mat imgGray, imgBlur , imgCanny, imgDil, imgErode;

	cvtColor(img, imgGray, COLOR_BGR2GRAY);   // Èæ¹é»öÀ¸·Î º¯È¯
	GaussianBlur(img, imgBlur, Size(7, 7), 5, 0);       //  Èå¸²
	Canny(imgBlur, imgCanny, 50, 150);                 // Å×µÎ¸®¸¸ ³ª¿È

	Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));    // È¬¼ö
	dilate(imgCanny, imgDil, kernel);                     // Å×µÎ¸® ÆØÃ¢

	erode(imgDil, imgErode, kernel);                     //  Ä§½Ä, 

	imshow("Image", img);
	imshow("Image Gray", imgGray);
	imshow("Image Blur", imgBlur);
	imshow("Image Canny", imgCanny);
	imshow("Image Dilation", imgDil);
	imshow("Image Erode", imgErode);
	waitKey(0);
}
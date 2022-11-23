#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void main()
{
	string path = "Resources/cat.jpg";
	Mat img = imread(path);
	Mat imgResize ,  imgCrop;

	//cout << img.size() << endl;
	resize(img, imgResize, Size(320, 223));         // size 변경

	Rect roi(100, 100, 300, 250);                        //  자를 위치 , 출력할 사이즈
	imgCrop = img(roi);                                   //  이미지 자르기


	imshow("Image", img);
	imshow("Image Resize", imgResize);
	imshow("Image Crop", imgCrop);

	waitKey(0);
}
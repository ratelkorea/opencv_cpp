#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// ---------------------  Draw Shapes and Text  ------------------

void main()
{
	Mat img(512, 512, CV_8UC3, Scalar(255, 255, 255));    //  흰색 사각형 출력

	circle(img, Point(256, 256), 155, Scalar(0, 69, 255), FILLED);     //  중심에 원 그리기 ,마지막 매개변수 테두리 두께
	rectangle(img, Point(130, 226), Point(382, 286), Scalar(255, 255, 255), FILLED);                    //  4각형 그리기
	line(img, Point(130, 296), Point(382, 296), Scalar(255, 255, 255), 2);                             //  선 그리기

	// image, text, point, font, size, scalar, thickness
	putText(img, "Murtaza's Workshop", Point(137, 262), FONT_HERSHEY_DUPLEX,  0.75 , Scalar(0, 69, 255), 2);
	imshow("Image", img);

	waitKey(0);
}
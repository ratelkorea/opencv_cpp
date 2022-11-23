#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace cv;
using namespace std;

Mat img;
vector<vector<int>> newPoints;

vector<int> myColors{ 81, 0, 116, 179,  255,  204 };

vector<Scalar> myColorValues{ 255, 0, 255 };

Point getContours(Mat imgDil)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); // 윤곽선 구하기

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	Point myPoint(0, 0);

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);                       // 형태의 넓이 구하기
		cout << area << endl;

		if (area > 200)
		{
			float peri = arcLength(contours[i], true);                  // 윤곽 둘레
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);   // 등고선 근사

			cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);                 // 경계 사각형
			myPoint.x = boundRect[i].x + boundRect[i].width / 2;
			myPoint.y = boundRect[i].y;

			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);   // 윤곽선 그리기 
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
		}

	}
	return myPoint;
}

void findColor(Mat img)
{
	Mat imgHSV, mask;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);  // 색 채널 바꾸는 함수

	Scalar lower(myColors[0], myColors[1], myColors[2]);
	Scalar upper(myColors[3], myColors[4], myColors[5]);
	inRange(imgHSV, lower, upper, mask);    // HSV 색상 공간의 픽셀 값 범위를 기반으로 개체를 감지
	//imshow("mask", mask);
	Point myPoint = getContours(mask);

	if (myPoint.x != 0 && myPoint.y != 0)
	{
		newPoints.push_back({ myPoint.x, myPoint.y });
	}
	
}

void drawOnCanvas()
{
	for (int i = 0; i < newPoints.size(); i++)
	{
		circle(img, Point(newPoints[i][0], newPoints[i][1]), 10, myColorValues[0], FILLED);
	}
}

void main()
{
	VideoCapture cap(0);    // 웹캠 입력 
	

	while (true)
	{
		cap.read(img);

		findColor(img);
		drawOnCanvas();
		imshow("Image", img);
		waitKey(1);
	}
}
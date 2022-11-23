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
	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); // ������ ���ϱ�

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	Point myPoint(0, 0);

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);                       // ������ ���� ���ϱ�
		cout << area << endl;

		if (area > 200)
		{
			float peri = arcLength(contours[i], true);                  // ���� �ѷ�
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);   // ��� �ٻ�

			cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);                 // ��� �簢��
			myPoint.x = boundRect[i].x + boundRect[i].width / 2;
			myPoint.y = boundRect[i].y;

			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);   // ������ �׸��� 
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
		}

	}
	return myPoint;
}

void findColor(Mat img)
{
	Mat imgHSV, mask;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);  // �� ä�� �ٲٴ� �Լ�

	Scalar lower(myColors[0], myColors[1], myColors[2]);
	Scalar upper(myColors[3], myColors[4], myColors[5]);
	inRange(imgHSV, lower, upper, mask);    // HSV ���� ������ �ȼ� �� ������ ������� ��ü�� ����
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
	VideoCapture cap(0);    // ��ķ �Է� 
	

	while (true)
	{
		cap.read(img);

		findColor(img);
		drawOnCanvas();
		imshow("Image", img);
		waitKey(1);
	}
}
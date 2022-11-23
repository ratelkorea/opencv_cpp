#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace cv;
using namespace std;

vector<Point> initialPoints, docPoints;

Mat imgOriginal, imgGray,  imgBlur, imgCanny,  imgDil,imgThre, imgErode, imgWarp, imgCrop;

float w = 420, h = 596;

Mat preProcessing(Mat img)
{
	cvtColor(img, imgGray, COLOR_BGR2GRAY);  // gray �� ��ȯ
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);  // �帲 ó�� (������ ����)
	Canny(imgBlur, imgCanny, 25, 75);                  // ��輱 ����

	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);                   // Ȯ��
	//erode(imgDil, imgErode, kernel);                    // ħ��

	return imgDil;
}

vector<Point> getContours(Mat imgDil)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); // ������ ���ϱ�

	vector<vector<Point>> conPoly(contours.size());
	//vector<Rect> boundRect(contours.size());

	vector<Point> biggest;
	int maxArea = 0;

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);                       // ������ ���� ���ϱ�
		cout << area << endl;

		if (area > 1000)
		{
			float peri = arcLength(contours[i], true);                  // ���� �ѷ�
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);   // ��� �ٻ�

			if (area > maxArea && conPoly[i].size() == 4)
			{
				//drawContours(imgOriginal, conPoly, i, Scalar(255, 0, 255), 3);   // ������ �׸��� 
				biggest = {conPoly[i][0], conPoly[i][1], conPoly[i][2], conPoly[i][3]};
				maxArea = area;
			}
			//rectangle(imgOriginal, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
		}

	}
	return biggest;
}

void drawPoints(vector<Point> points, Scalar color)
{
	for (int i = 0;i< points.size(); i++)
	{
		circle(imgOriginal, points[i], 10, color, FILLED);
		putText(imgOriginal, to_string(i), points[i], FONT_HERSHEY_PLAIN, 5, color, 3);
	}
}

vector<Point> reorder(vector<Point> points)
{
	vector<Point> newPoints;
	vector<int> sumPoints, subPoints;

	for (int i = 0; i < 4; i++)
	{
		sumPoints.push_back(points[i].x + points[i].y);
		subPoints.push_back(points[i].x - points[i].y);
	}

	newPoints.push_back(points[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);
	newPoints.push_back(points[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);
	newPoints.push_back(points[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);
	newPoints.push_back(points[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);

	return newPoints;
}

Mat getWarp(Mat img, vector<Point> points, float w, float h)
{
	Point2f src[4] = { points[0], points[1], points[2], points[3] };
	Point2f dst[4] = { {0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h} };

	Mat matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix, Point(w, h));

	return imgWarp;
}
void main()
{
	string path = "paper.jpg";
	imgOriginal = imread(path);

	// ��ó��
	imgThre = preProcessing(imgOriginal);  
	// Get Contours - Biggest   ���� ū �簢�� ã��

	initialPoints = getContours(imgThre);
	//drawPoints(initialPoints, Scalar(0, 0, 255));
	docPoints = reorder(initialPoints);
	//drawPoints(docPoints, Scalar(0, 255, 0));

	// Warp
	imgWarp = getWarp(imgOriginal, docPoints, w, h);

	// Crop
	Rect roi(5, 5, w - (2 * 5), h - (2 * 5));
	imgCrop = imgWarp(roi);

	imshow("Image Thre", imgThre);
	imshow("img", imgOriginal);
	imshow("Img warp", imgWarp);
	imshow("Img Crop", imgCrop);
	waitKey(0);
}
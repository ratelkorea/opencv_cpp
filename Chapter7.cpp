#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat imgGray, imgBlur, imgCanny, imgDil, imgErode;

void getContours(Mat imgDil, Mat img)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);  // ������ ���ϱ�
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 2);
	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);   // ���� ���� ���ϱ�
		cout << area << endl;


		string objectType;

		if (area > 1000)
		{
			float peri = arcLength(contours[i], true);                          //  ���� �ѷ�
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);   // ��� �ٻ�
			
			cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);                       // ��� �簢��

			int objCor = (int)conPoly[i].size();

			if (objCor == 3) { objectType = "Tri"; }
			if (objCor == 4)
			{
				float aspRatio = (float)boundRect[i].width / (float)boundRect[i].height;
				cout << aspRatio << endl;
				if (aspRatio > 0.95 && aspRatio < 1.05)
				{
					objectType = "Square";
				}
				else
				{
					objectType = "Rect";
				}
				objectType = "Rect";
			}
			if (objCor > 4) { objectType = "Circle"; }
			

			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);                      // ������ �׸��� 
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);            // �簢�� �׸���
			putText(img, objectType, { boundRect[i].x, boundRect[i].y - 5 }, FONT_HERSHEY_DUPLEX, 0.75, Scalar(0, 69, 255), 2);  // �� ���� 
		}
	}
}

int main()
{
	string path = "Resources/shapes.jpg";
	Mat img = imread(path);

	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray,  imgBlur,  Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);                      //  �׵θ� ����
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);


	getContours(imgDil, img);


	imshow("Image", img);
	//imshow("Image Gray", imgGray);
	//imshow("Image Blur", imgBlur);
	//imshow("Image Canny", imgCanny);
	imshow("Image Dil", imgDil);
	
	waitKey(0);

	return 0;
}
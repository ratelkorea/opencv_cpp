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
	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);  // 윤곽선 구하기
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 2);
	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);   // 형태 넓이 구하기
		cout << area << endl;


		string objectType;

		if (area > 1000)
		{
			float peri = arcLength(contours[i], true);                          //  윤곽 둘레
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);   // 등고선 근사
			
			cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);                       // 경계 사각형

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
			

			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);                      // 윤곽선 그리기 
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);            // 사각형 그리기
			putText(img, objectType, { boundRect[i].x, boundRect[i].y - 5 }, FONT_HERSHEY_DUPLEX, 0.75, Scalar(0, 69, 255), 2);  // 글 쓰기 
		}
	}
}

int main()
{
	string path = "Resources/shapes.jpg";
	Mat img = imread(path);

	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray,  imgBlur,  Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);                      //  테두리 검출
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
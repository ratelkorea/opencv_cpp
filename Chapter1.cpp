#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//     --------------        Images  81  179  0 255 116  204 
   
//void main()
//{
//	string path = "Resources/cat.jpg";
//	Mat img = imread(path);
//	imshow("Image", img);
//	waitKey(0);
//}

 //  ------------------    Video

void main()
{
	string path = "Resources/maple.mp4";
	VideoCapture cap(path);
	Mat img;
	while (true)
	{
		cap.read(img);

		imshow("Image", img);
		waitKey(20);
	}

}
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat img;
vector<vector<int>> mousePoint;

vector<vector<int>> myColors  { {139,153,10,179,231,128},//red
	{112,146,16,125,239,71},// blue
	//{55,59,0,135,173,85}// black 
};
		//139,153,10,179,231,128 red
		//112,146,16,125,239,71 blue
		//55,59,0,135,173,85 black

vector<Scalar> myColorValues{ {0,0,255}//red
						,{255,0,0}//blue
					 };//black
Point getContours(Mat imgDil) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());
	Point myPoint(0, 0);
	for (int i = 0;i < contours.size();i++) {
		int area = contourArea(contours[i]);
		cout << area << endl;
		
		string objectType;

		if (area > 1000) {
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);
			myPoint.x = boundRect[i].x + boundRect[i].width / 2;
			myPoint.y = boundRect[i].y;
			drawContours(img, conPoly, i,Scalar(255,0,255) , 2);
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);


		}

	}
	return myPoint;
}
vector<vector<int>> findColor(Mat img)
{
	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);
	for (int i = 0;i < myColors.size();i++) {
		Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
		Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
		Mat mask;
		inRange(imgHSV, lower, upper, mask);
		imshow(to_string(i), mask);
		Point myPoint=getContours(mask);
		if (myPoint.x !=0 && myPoint.y !=0)
		mousePoint.push_back({ myPoint.x, myPoint.y, i });
	}
	return mousePoint;
}
void drawOnCanvas(vector<vector<int>> mousePoint) {
	for (int i = 0;i < mousePoint.size();i++) {
		circle(img, Point(mousePoint[i][0], mousePoint[i][1]), 10, myColorValues[mousePoint[i][2]], FILLED);
	}
}

int main() {
	VideoCapture cap(0);
	//Mat img;
	while (true) {
		cap.read(img);
		mousePoint=findColor(img);
		drawOnCanvas(mousePoint);
		imshow("Image", img);
		waitKey(1);
	}
	return 0;
}

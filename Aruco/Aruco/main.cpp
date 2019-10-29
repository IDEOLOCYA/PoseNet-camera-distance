#include<opencv2/aruco/charuco.hpp>
#include <opencv2\aruco.hpp>
#include <opencv2\calib3d.hpp>
#include <opencv2/core/core_c.h>
#include <opencv2/calib3d/calib3d_c.h>
#include <opencv2\highgui.hpp>
#include <opencv2\imgcodecs.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc.hpp>
#include "opencv2\opencv.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdint.h>
#define _USE_MATH_DEFINES
#include <math.h>
# define M_PI 3.141592653589793238462643383279502884L /* pi */
using namespace std;
using namespace cv;







static bool readCameraParameters(string filename, Mat &camMatrix, Mat &distCoeffs) {
	FileStorage fs(filename, FileStorage::READ);
	if (!fs.isOpened())
		return false;
	fs["cameraMatrix"] >> camMatrix;
	fs["dist_coeffs"] >> distCoeffs;
	return true;
}


double rad2deg(double radians)
{
	double degrees;
	

	degrees = radians * 180 / M_PI;
	return degrees;
}


int main(int argc, char **argv)
{
	double  cy, cp, cr, sy, sp, sr, roll_x, pith_y, yaw_z, x, y, z, w, p, q, r;
	Mat cameraMatrix, distCoeffs;
	cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_250);
	Ptr<aruco::CharucoBoard> board = aruco::CharucoBoard::create(5, 7, 0.028, 0.013, dictionary);
	readCameraParameters("C:\\Users\\IDEOLOCYA\\Desktop\\mycamera.xml", cameraMatrix, distCoeffs);
	int waitTime = 5;
	int counter = 0;

	String path = "D:\\my_test\\";
	ifstream file("demo.txt");
	string   line;
	Mat img, imgCopy;
	double roll, pitch, yaw;




	while (getline(file, line))
	{
		stringstream   linestream(line);
		string         data;
		double x,y,z,q1,q2,q3,q4;
		getline(linestream, data, ' '); 

		linestream >> x>>y>>z>>q1>>q2>>q3>>q4;

		//cout  << x << " " <<y << " " <<z << " : " <<q1 << " " <<q2 << " " <<q3 << " " <<q4<<endl;

		yaw = atan2(((2 * q2*q3)* (2 * q1*q4)), (2 * q1*q1) + (2 * q2*q2) + 1);
		pitch = -asin((2 * q2*q4) + (2 * q1*q3));
		roll = atan2((2 * q3*q4) - (2 * q1*q2), (2 * q1*q1) + (2 * q4*q4) - 1);

		/*
		double sqw;
		double sqx;
		double sqy;
		double sqz;



		double row, pitch, yaw;
		double rotxrad;
		double rotyrad;
		double rotzrad;

		sqw = q1 * q1;
		sqx = q2 * q2;
		sqy = q3 * q3;
		sqz = q4 * q4;

		rotxrad = (double)atan2l(2.0 * (q3 *q4 + q2 *q1), (-sqx - sqy + sqz + sqw));
		rotyrad = (double)asinl(-2.0 * (q2* q4 - q3* q1));
		rotzrad = (double)atan2l(2.0 * (q2* q3+ q4* q1), (sqx - sqy - sqz + sqw));

		roll = rotxrad;
		pitch = rotyrad;
		yaw = rotzrad;



		*/

		/*
		orjini tespıt et(tvec)
		0.1 ver nerey gittigıne bak
		rotasyon matrıxı elde et ama quaternıonda et
		en son ucları tespır edıp lıne cızdır.

		*/

		/*
		// roll (x-axis rotation)
		double sinr_cosp = +2.0 * (q1 * q2 + q3 * q4);
		double cosr_cosp = +1.0 - 2.0 * (q2 * q2 + q3 * q3);
		roll = atan2(sinr_cosp, cosr_cosp) ;


		// pitch (y-axis rotation)
		double sinp = +2.0 * (q1 * q3 - q4 * q2);
		if (fabs(sinp) >= 1)

			pitch = copysign(M_PI / 2, sinp)* 180 / M_PI; // use 90 degrees if out of range
		else
			pitch = asin(sinp);

		// yaw (z-axis rotation)
		double siny_cosp = +2.0 * (q1 * q4+ q2 * q3);
		double cosy_cosp = +1.0 - 2.0 * (q3 * q3 + q4 * q4);
		yaw = atan2(siny_cosp, cosy_cosp) ;


		*/

		Vec3d rvec, tvec;

		tvec[0]=x;
		tvec[1]=y;
		tvec[2]=z;
		
		rvec[0]=pitch;
	    rvec[1]=roll;
		rvec[2]=yaw;

		img = imread(path + data);
		img.copyTo(imgCopy);
		
		cv::drawFrameAxes(imgCopy, cameraMatrix, distCoeffs, rvec, tvec, 0.1, 5);
		cout <<"Tahmin: "<< x << " " << y << " " << z << " " << pitch << " " << roll << " " << yaw<< endl;

		Mat imageCopy;
		img.copyTo(imageCopy);
		vector<int> ids;
		vector<vector<Point2f> > corners;
		aruco::detectMarkers(img, dictionary, corners, ids);
		if (ids.size() > 0)
		{
			vector<Point2f> charucoCorners;
			vector<int> charucoIds;
			aruco::interpolateCornersCharuco(corners, ids, img, board, charucoCorners, charucoIds, cameraMatrix, distCoeffs);

			// if at least one charuco corner detected
			if (charucoIds.size() > 0)
			{
			
				aruco::drawDetectedCornersCharuco(imageCopy, charucoCorners, charucoIds, Scalar(255, 0, 0));
				Vec3d rvec, tvec;
				bool valid = aruco::estimatePoseCharucoBoard(charucoCorners, charucoIds, board, cameraMatrix, distCoeffs, rvec, tvec);

				// if charuco pose is valid
				if (valid)
				{
					y = tvec[0];
					x = tvec[1];
					z = tvec[2];

					roll_x = rvec[0];
					pith_y = rvec[1];
					yaw_z = rvec[2];

					cy = cos(yaw_z * 0.5);
					sy = sin(yaw_z * 0.5);
					cp = cos(pith_y * 0.5);
					sp = sin(pith_y * 0.5);
					cr = cos(roll_x * 0.5);
					sr = sin(roll_x * 0.5);

					w = cy * cp * cr + sy * sp * sr;
					p = cy * cp * sr - sy * sp * cr;
					q = sy * cp * sr + cy * sp * cr;
					r = sy * cp * cr - cy * sp * sr;

					aruco::drawAxis(imgCopy, cameraMatrix, distCoeffs, rvec, tvec, 0.2);

					
					cout<<"Asolan: "<< x << " " << y << " " << z << " " << roll_x << " " << pith_y<< " " <<yaw_z<<endl <<endl;
					
					
					cout << "QTahmin: " << q1 << " " << q2 << " " << q3 << " " << q4 << endl;
					cout << "QAsolan: " << w << " " <<p<< " " <<q<<" "<<r<<endl<<endl<<endl<<endl;
				
					
					//imshow("CHARUCO", imageCopy);
				}
			}
		}

		
		imshow("CNN",imgCopy);
		waitKey(10000);

	}
	waitKey(100000);
}






/*

int main(int argc, char **argv)
{
	Mat cameraMatrix, distCoeffs;
	cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_250);
	Ptr<aruco::CharucoBoard> board = aruco::CharucoBoard::create(5, 7, 0.028, 0.013, dictionary);
	readCameraParameters("C:\\Users\\IDEOLOCYA\\Desktop\\mycamera.xml", cameraMatrix, distCoeffs);
	int waitTime = 5;
	int counter = 0;
	Mat img;
	double  cy, cp, cr, sy, sp, sr, roll_x, pith_y, yaw_z, x, y, z, w, p, q, r;

	ofstream myfile;
	myfile.open("D:\\my_test\\posenet.txt");

	ifstream file("D:\\my_test\\name.txt");
	string line;
	String path = "D:\\my_test\\";

	while (getline(file, line)) {

		istringstream stream(line);

		//double x;
		//while (stream >> x) {
		while (stream)
		{
			string s;
			if (!getline(stream, s, '\n')) break;
			img = imread(path + s);
			Mat imageCopy;


			img.copyTo(imageCopy);
			vector<int> ids;
			vector<vector<Point2f> > corners;
			aruco::detectMarkers(img, dictionary, corners, ids);


			// if at least one marker detected
			if (ids.size() > 0)
			{
				vector<Point2f> charucoCorners;
				vector<int> charucoIds;
				aruco::interpolateCornersCharuco(corners, ids, img, board, charucoCorners, charucoIds, cameraMatrix, distCoeffs);

				// if at least one charuco corner detected
				if (charucoIds.size() > 0)
				{
					cout << charucoIds.size() << endl;
					aruco::drawDetectedCornersCharuco(imageCopy, charucoCorners, charucoIds, Scalar(255, 0, 0));




					//imshow("test", imageCopy);
					cv::waitKey(5);

					Vec3d rvec, tvec;
					bool valid = aruco::estimatePoseCharucoBoard(charucoCorners, charucoIds, board, cameraMatrix, distCoeffs, rvec, tvec);

					// if charuco pose is valid
					if (valid)
					{

						aruco::drawAxis(imageCopy, cameraMatrix, distCoeffs, rvec, tvec, 0.1);

						y = tvec[0];
						x = tvec[1];
						z = tvec[2];

						roll_x = rvec[0];
						pith_y = rvec[1];
						yaw_z = rvec[2];

						cy = cos(yaw_z * 0.5);
						sy = sin(yaw_z * 0.5);
						cp = cos(pith_y * 0.5);
						sp = sin(pith_y * 0.5);
						cr = cos(roll_x * 0.5);
						sr = sin(roll_x * 0.5);

						w = cy * cp * cr + sy * sp * sr;
						p = cy * cp * sr - sy * sp * cr;
						q = sy * cp * sr + cy * sp * cr;
						r = sy * cp * cr - cy * sp * sr;

						aruco::drawAxis(imageCopy, cameraMatrix, distCoeffs, rvec, tvec, 0.1);

						String name = format("img%03d.png", counter++);
						imwrite(s, imageCopy);
						//rvec = rvec * (57.295779513);
						myfile << name << " " << x << " " << y << " " << z << " " << w << " " << p << " " << q << " " << r << endl;


						imshow("Yazdi", imageCopy);

					}

				}

				imshow("out", img);

				char key = (char)waitKey(waitTime);
				if (key == 27)
					break;
				imshow("baba", img);
			}
			counter++;
		}
	}
}



/*
int main(int argc, char **argv)
{
	double  cy,cp,cr,sy,sp ,sr, roll_x, pith_y, yaw_z,x,y,z, w, p, q, r;

	ofstream myfile;
	myfile.open("C:\\Users\\IDEOLOCYA\\source\\repos\\Aruco\\Aruco\\dataset\\posenet.txt");
	



	int baba = 0;
	int dede = 0;


	int waitTime =5;
	int counter = 0;
	//const string videoStreamAddress = "http://192.168.3.21:8080/shot.jpg";
	const string videoStreamAddress = "C:\\Users\\IDEOLOCYA\\Desktop\\ikinci.mp4";

	VideoCapture vid(videoStreamAddress);
	//VideoCapture vid;
	Mat cameraMatrix, distCoeffs;
	cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_250);
	Ptr<aruco::CharucoBoard> board = aruco::CharucoBoard::create(5, 7, 0.028, 0.013, dictionary);
	readCameraParameters("C:\\Users\\IDEOLOCYA\\Desktop\\mycamera.xml",cameraMatrix, distCoeffs);
	

	if(!vid.open(videoStreamAddress))
	{
		cout << "Error opening video stream or file" << endl;
		return -1;
	}

	
	while (true)
	{
		Mat image, imageCopy;
		//vid.open(videoStreamAddress);

		if (!vid.read(image))
		{
			cout << "No frame" << endl;
			waitKey();
		}
		image.copyTo(imageCopy);
		vector<int> ids;
		vector<vector<Point2f> > corners;
		aruco::detectMarkers(image, dictionary, corners, ids);


			// if at least one marker detected
			if (ids.size() > 0)
			{
				vector<Point2f> charucoCorners;
				vector<int> charucoIds;
				aruco::interpolateCornersCharuco(corners, ids, image, board, charucoCorners, charucoIds, cameraMatrix, distCoeffs);			

				// if at least one charuco corner detected
				if (charucoIds.size() > 0) 
				{
					cout << charucoIds.size() << endl;
					aruco::drawDetectedCornersCharuco(imageCopy, charucoCorners, charucoIds, Scalar(255, 0, 0));

					


					//imshow("test", imageCopy);
					cv::waitKey(5);
					
					Vec3d rvec, tvec;
					bool valid = aruco::estimatePoseCharucoBoard(charucoCorners, charucoIds, board, cameraMatrix, distCoeffs, rvec, tvec);

					// if charuco pose is valid
					if (valid)
					{
						cout <<"iceri giren sayisi: "<<baba++<< endl;
						aruco::drawAxis(imageCopy, cameraMatrix, distCoeffs, rvec, tvec, 0.1);
						
						y = tvec[0];
						x = tvec[1];
						z=tvec[2];

						roll_x = rvec[0];
						pith_y = rvec[1];
						yaw_z= rvec[2];

						cy = cos(yaw_z * 0.5);
						sy = sin(yaw_z * 0.5);
						cp = cos(pith_y * 0.5);
						sp = sin(pith_y * 0.5);
						cr = cos(roll_x * 0.5);
						sr = sin(roll_x * 0.5);
					
						w = cy * cp * cr + sy * sp * sr;
						p = cy * cp * sr - sy * sp * cr;
						q = sy * cp * sr + cy * sp * cr;
						r = sy * cp * cr - cy * sp * sr;

						aruco::drawAxis(imageCopy, cameraMatrix, distCoeffs, rvec, tvec, 0.1);

						String name = format("img%03d.png", counter++);
						imwrite(name, image);
						//rvec = rvec * (57.295779513);
						myfile << name << " " << x <<  " " << y << " "<< z <<" " << w << " " << p << " " << q << " " << r << endl;
						
						
						imshow("Yazdi", imageCopy);
						
					}
					else {
						cout << "elenen kare sayısı: "<<dede++<< endl;
					}
				}
			}
			
			imshow("out", image);
		
			char key = (char) waitKey(waitTime);
			if (key == 27)	
				break;
	}
	waitKey(waitTime);

		vid.release();
		myfile.close();
		
		
}



*/

/*


int main(int argv, char **argc)
{

	int waitTime = 5;
	int counter = 0;
	//const string videoStreamAddress = "http://10.100.253.155:8080/shot.jpg";
	const string videoStreamAddress = "C:\\Users\\IDEOLOCYA\\Desktop\\VID_20190807_092804.mp4";
	VideoCapture vid(videoStreamAddress);
	Mat image, imageCopy;
	vid.open(videoStreamAddress);

	while (true)
	{
		Mat image;
		//vid.open(videoStreamAddress);
		if (!vid.read(image))
		{
			cout << "No frame" << endl;
			waitKey();
		}

		cv::waitKey(5);
		imshow("out", image);
		int a=waitKey(10);
		String adres = format("C:\\Users\\IDEOLOCYA\\Desktop\\MY_DATASET\\img%04d.png", counter++);
		imwrite(adres, image);

/*	if (a == 27)
			String name = format("img%04d.png", counter++);
			String adres = format("C:\\Users\\IDEOLOCYA\\Desktop\\MY_DATASET\\img%04d.png", counter++);
			imwrite(adres, image);

	}
	waitKey(waitTime);

	vid.release();



}



*/

/*
int main(int argv, char* argc)
{
	cv::Mat m(700, 500, CV_8UC1);
	cv::Ptr< cv::aruco::Dictionary > dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_250);
	cv::Ptr< cv::aruco::CharucoBoard > charuco = cv::aruco::CharucoBoard::create(2, 2, 0.028, 0.013, dict);
	charuco->draw(cv::Size(700, 500), m, 0, 1);
	cv::imwrite("board.png", m);

	return 0;
}
*/

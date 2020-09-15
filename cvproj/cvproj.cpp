#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void detectAndDisplay(Mat frame);
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

int main()
{
    setlocale(LC_ALL, "RUS");
    String face_cascade_name = "D:/opencv/data/haarcascades/haarcascade_frontalface_alt.xml";
    String eyes_cascade_name = "D:/opencv/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";

    if (!face_cascade.load(face_cascade_name))
    {
        cout << "--(!)Error loading face cascade\n";
        return -1;
    };
    if (!eyes_cascade.load(eyes_cascade_name))
    {
        cout << "--(!)Error loading eyes cascade\n";
        return -1;
    };

    string url;
    VideoCapture capture;
    cout << "Как осуществить захват камеры?"<< 
            "Enter- подключенная камера. Введите ip камеры чтобы подключиться удаленно";
    getline(cin,url);
    if (url.empty())
        capture.open(0, CAP_ANY);
    else
        capture.open(url);
    capture.set(CAP_PROP_FRAME_WIDTH, 320);
    capture.set(CAP_PROP_FRAME_HEIGHT,240 );

	Mat frame ;
    int frameNum=0;

    for (;;) //Show the image captured in the window and repeat
    {
        capture >> frame;
        if (frame.empty())
        {
            cout << " < < <  Game over!  > > > ";
            break;
        }
        ++frameNum;
        cout << "Frame: " << frameNum << "# ";
        cout << endl;
        //imshow("capture", frame);
        detectAndDisplay(frame);
        char c = (char)waitKey(10);
        if (c == 27) break;
    }
	capture.release();
	return 0;
}

void detectAndDisplay(Mat frame)
{
    Mat frame_gray;
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);
    //-- Detect faces
    std::vector<Rect> faces;
    face_cascade.detectMultiScale(frame_gray, faces);
    for (size_t i = 0; i < faces.size(); i++)
    {
        Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
        ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);
        Mat faceROI = frame_gray(faces[i]);
        //-- In each face, detect eyes
        vector<Rect> eyes;
        eyes_cascade.detectMultiScale(faceROI, eyes);
        for (size_t j = 0; j < eyes.size(); j++)
        {
            Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
            int radius = cvRound((eyes[j].width + eyes[j].height) * 0.25);
            circle(frame, eye_center, radius, Scalar(255, 0, 0), 4);
        }
    }
    //-- Show what you got
    imshow("Capture - Face detection", frame);
}
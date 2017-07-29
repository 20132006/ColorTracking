#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include<iostream>

/*
TRACK A YELLOW BALL - OBJECT DETECTION METHOD USING COLOR SEPERATION OPEN CV 3.1.0
author - Rachit Gulati
*/

int main()
{
    cv::VideoCapture capWebcam(0);      // declare a VideoCapture object to associate webcam, 0 means use 1st (default) webcam

    if (capWebcam.isOpened() == false)   //  To check if object was associated to webcam successfully
    {               
        std::cout << "error: Webcam connect unsuccessful\n";    // if not then print error message
        return(0);                                              // and exit program
    }

    cv::Mat imgOriginal;            // Input image
    cv::Mat hsvImg;                 // HSV Image
    cv::Mat threshImgYellow;        // Thresh Image
    cv::Mat threshImgGreen;        // Thresh Image
    cv::Mat threshImgRed;        // Thresh Image

    std::vector<cv::Vec3f> v3fCirclesYellow;      // 3 element vector of floats, this will be the pass by reference output of HoughCircles()
    std::vector<cv::Vec3f> v3fCirclesGreen;      // 3 element vector of floats, this will be the pass by reference output of HoughCircles()
    std::vector<cv::Vec3f> v3fCirclesRed;      // 3 element vector of floats, this will be the pass by reference output of HoughCircles()

    char charCheckForEscKey = 0;
    // HUE for YELLOW is 21-30.
    int lowH_yellow = 21;                          // Set Hue
    int highH_yellow = 30;

    int lowS_yellow = 200;                         // Set Saturation
    int highS_yellow = 255;

    int lowV_yellow = 102;                         // Set Value
    int highV_yellow = 225;
    // HUE for GREEN is 38-76.
    int lowH_green = 38;                          // Set Hue
    int highH_green = 76;

    int lowS_green = 119;                         // Set Saturation
    int highS_green = 255;

    int lowV_green = 129;                         // Set Value
    int highV_green = 255;
    // HUE for RED is 160-179.
    int lowH_red = 160;                          // Set Hue
    int highH_red = 179;

    int lowS_red = 75;                         // Set Saturation
    int highS_red = 255;

    int lowV_red = 96;                         // Set Value
    int highV_red = 255;
    // Adjust Saturation and Value depending on the lighting condition of the environment as well as the surface of the object.

    while (charCheckForEscKey != 27 && capWebcam.isOpened())                // until the Esc is pressed or webcam connection is lost
    {
        bool blnFrameReadSuccessfully = capWebcam.read(imgOriginal);        // get next frame

        if (!blnFrameReadSuccessfully || imgOriginal.empty()) {             // if frame read unsuccessfully
            std::cout << "error: frame can't read \n";                      // print error message
            break;                                                          // jump out of loop
        }
        // Yellow----------------------------------------------------------------------------------------------------------------------------------
        cv::cvtColor(imgOriginal, hsvImg, CV_BGR2HSV);                      // Convert Original Image to HSV Thresh Image

        cv::inRange(hsvImg, cv::Scalar(lowH_yellow, lowS_yellow, lowV_yellow), cv::Scalar(highH_yellow, highS_yellow, highV_yellow), threshImgYellow);

        cv::GaussianBlur(threshImgYellow, threshImgYellow, cv::Size(3, 3), 0);          //Blur Effect
        cv::dilate(threshImgYellow, threshImgYellow, 0);                                // Dilate Filter Effect
        cv::erode(threshImgYellow, threshImgYellow, 0);                                 // Erode Filter Effect

        // fill circles vector with all circles in processed image
        cv::HoughCircles(threshImgYellow,v3fCirclesYellow,CV_HOUGH_GRADIENT,2,threshImgYellow.rows / 4,100,50,10,800);  // algorithm for detecting circles        

        // Green-----------------------------------------------------------------------------------------------------------------------------------
        cv::cvtColor(imgOriginal, hsvImg, CV_BGR2HSV);                      // Convert Original Image to HSV Thresh Image

        cv::inRange(hsvImg, cv::Scalar(lowH_green, lowS_green, lowV_green), cv::Scalar(highH_green, highS_green, highV_green), threshImgGreen);

        cv::GaussianBlur(threshImgGreen, threshImgGreen, cv::Size(3, 3), 0);          //Blur Effect
        cv::dilate(threshImgGreen, threshImgGreen, 0);                                // Dilate Filter Effect
        cv::erode(threshImgGreen, threshImgGreen, 0);                                 // Erode Filter Effect

        // fill circles vector with all circles in processed image
        cv::HoughCircles(threshImgGreen,v3fCirclesGreen,CV_HOUGH_GRADIENT,2,threshImgGreen.rows / 4,100,50,10,800);  // algorithm for detecting circles        


        // Red-------------------------------------------------------------------------------------------------------------------------------------
        cv::cvtColor(imgOriginal, hsvImg, CV_BGR2HSV);                      // Convert Original Image to HSV Thresh Image

        cv::inRange(hsvImg, cv::Scalar(lowH_red, lowS_red, lowV_red), cv::Scalar(highH_red, highS_red, highV_red), threshImgRed);

        cv::GaussianBlur(threshImgRed, threshImgRed, cv::Size(3, 3), 0);          //Blur Effect
        cv::dilate(threshImgRed, threshImgRed, 0);                                // Dilate Filter Effect
        cv::erode(threshImgRed, threshImgRed, 0);                                 // Erode Filter Effect

        // fill circles vector with all circles in processed image
        cv::HoughCircles(threshImgRed,v3fCirclesRed,CV_HOUGH_GRADIENT,2,threshImgRed.rows / 4,100,50,10,800);  // algorithm for detecting circles        
        //-----------------------------------------------------------------------------------------------------------------------------------------
        
        for (int i = 0; i < v3fCirclesYellow.size(); i++) {                       // for each circle
                                                            
            std::cout << "Yellow Ball position X = "<< v3fCirclesYellow[i][0]            // x position of center point of circle
                <<",\tY = "<< v3fCirclesYellow[i][1]                              // y position of center point of circle
                <<",\tRadius = "<< v3fCirclesYellow[i][2]<< "\n";                 // radius of circle

                                                                                        // draw small green circle at center of object detected
            cv::circle(imgOriginal,                                                     // draw on original image
                cv::Point((int)v3fCirclesYellow[i][0], (int)v3fCirclesYellow[i][1]),    // center point of circle
                3,                                                                      // radius of circle in pixels
                cv::Scalar(0, 255, 0),                                                  // draw green
                CV_FILLED);                                                             // thickness

                                                                                            // draw red circle around object detected 
            cv::circle(imgOriginal,                                                         // draw on original image
                cv::Point((int)v3fCirclesYellow[i][0], (int)v3fCirclesYellow[i][1]),        // center point of circle
                (int)v3fCirclesYellow[i][2],                                                // radius of circle in pixels
                cv::Scalar(0, 0, 255),                                                      // draw red
                3);                                                                         // thickness
        }
        for (int i = 0; i < v3fCirclesGreen.size(); i++) {                              // for each circle
                                                            
            std::cout << "Green Ball position X = "<< v3fCirclesGreen[i][0]             // x position of center point of circle
                <<",\tY = "<< v3fCirclesGreen[i][1]                                     // y position of center point of circle
                <<",\tRadius = "<< v3fCirclesGreen[i][2]<< "\n";                        // radius of circle

                                                                                        // draw small green circle at center of object detected
            cv::circle(imgOriginal,                                                     // draw on original image
                cv::Point((int)v3fCirclesGreen[i][0], (int)v3fCirclesGreen[i][1]),      // center point of circle
                3,                                                                      // radius of circle in pixels
                cv::Scalar(0, 255, 0),                                                  // draw green
                CV_FILLED);                                                             // thickness

                                                                                        // draw red circle around object detected 
            cv::circle(imgOriginal,                                                     // draw on original image
                cv::Point((int)v3fCirclesGreen[i][0], (int)v3fCirclesGreen[i][1]),      // center point of circle
                (int)v3fCirclesGreen[i][2],                                             // radius of circle in pixels
                cv::Scalar(0, 0, 255),                                                  // draw red
                3);                                                                     // thickness
        }   

        for (int i = 0; i < v3fCirclesRed.size(); i++) {                       // for each circle
                                                            
            std::cout << "Red Ball position X = "<< v3fCirclesRed[i][0]            // x position of center point of circle
                <<",\tY = "<< v3fCirclesRed[i][1]                              // y position of center point of circle
                <<",\tRadius = "<< v3fCirclesRed[i][2]<< "\n";                 // radius of circle

                                                                                        // draw small green circle at center of object detected
            cv::circle(imgOriginal,                                                     // draw on original image
                cv::Point((int)v3fCirclesRed[i][0], (int)v3fCirclesRed[i][1]),          // center point of circle
                3,                                                                      // radius of circle in pixels
                cv::Scalar(0, 255, 0),                                                  // draw green
                CV_FILLED);                                                             // thickness

                                                                                    // draw red circle around object detected 
            cv::circle(imgOriginal,                                                 // draw on original image
                cv::Point((int)v3fCirclesRed[i][0], (int)v3fCirclesRed[i][1]),      // center point of circle
                (int)v3fCirclesRed[i][2],                                              // radius of circle in pixels
                cv::Scalar(0, 0, 255),                                              // draw red
                3);                                                                 // thickness
        }   


        // declare windows
        cv::namedWindow("imgOriginal", CV_WINDOW_AUTOSIZE);
        cv::namedWindow("threshImgYellow", CV_WINDOW_AUTOSIZE);   

        /* Create trackbars in "threshImgYellow" window to adjust according to object and environment.*/
        cv::createTrackbar("lowH_yellow", "threshImgYellow", &lowH_yellow, 179);    //Hue (0 - 179)
        cv::createTrackbar("highH_yellow", "threshImgYellow", &highH_yellow, 179);

        cv::createTrackbar("lowS_yellow", "threshImgYellow", &lowS_yellow, 255);    //Saturation (0 - 255)
        cv::createTrackbar("highS_yellow", "threshImgYellow", &highS_yellow, 255);

        cv::createTrackbar("lowV_yellow", "threshImgYellow", &lowV_yellow, 255);    //Value (0 - 255)
        cv::createTrackbar("highV_yellow", "threshImgYellow", &highV_yellow, 255);
        

        cv::imshow("imgOriginal", imgOriginal);                 // show windows
        cv::imshow("threshImgYellow", threshImgYellow);
        cv::imshow("threshImgGreen", threshImgGreen);
        cv::imshow("threshImgRed", threshImgRed);

        charCheckForEscKey = cv::waitKey(1);                    // delay and get key press
    }
    
    return(0);                                          
}
#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>

#include <opencv2/opencv.hpp>

#include "System.h"
#include "depthai/depthai.hpp"


// TODO List:
//  * Finish swap to gen2_develop of depthai-core
//  * Add back the camera feature detection instead of only doing monoSLAM
//  * ORB Vocab and Settings file
//  * Command line camera type override
//  * Read IMU code
//  * Possible data massaging (e.g. timestamps, depth map size vs RGB image size, etc.)


using namespace std;

int main(int argc, char** argv)
{
    // Create pipeline
    dai::Pipeline p = createMonoPipeline();

    // Connect to device with above created pipeline
    dai::Device d(p);
    // Start the pipeline
    d.startPipeline();

    cv::Mat frame;
    auto monoQueue = d.getOutputQueue("mono");
    
    // Detect OAK-D or OAK-1 or override with user settings
    ORB_SLAM3::System::eSensor camera_type = ORB_SLAM3::System::MONOCULAR;
    /*
    if (d.is_left_connected() && d.is_right_connected())
        if (!d.is_rgb_connected())
            camera_type = ORB_SLAM3::System::STEREO;
        else
            camera_type = ORB_SLAM3::System::RGBD;
    */

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM3::System SLAM(/* ORB Vocab file */, /* Settings file */, camera_type, true);

    while(1)
    {
        auto imgFrame = monoQueue->get<dai::ImgFrame>();
        if(imgFrame)
        {
            frame = cv::Mat(imgFrame->getHeight(), imgFrame->getWidth(), CV_8UC1, imgFrame->getData().data());

            SLAM.TrackMonocular(imRGB, imgFrame->getTimestamp());
            
            int key = cv::waitKey(1);
            if (key == 'q'){
                return 0;
            } 
        }
    }

    // Stop all threads
    SLAM.Shutdown();

    // Save camera trajectory
    SLAM.SaveTrajectoryTUM("CameraTrajectory.txt");
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");   

    return 0;
}

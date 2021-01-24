#include <iostream>
#include <algorithm>
#include <fstream>

#include <opencv2/core/core.hpp>

#include "System.h"
#include "depthai/device.hpp"


// TODO List:
//  1. ORB Vocab and Settings file
//  2. Camera type detection and command line argument overrides
//  3. Read image code
//  4. Read IMU code
//  5. Possible data massaging (e.g. timestamps, depth map size vs RGB image size, etc.)


using namespace std;

int main(int argc, char** argv)
{
    // Setup OAK device
    Device d("", true);
    
    // TODO - Detect OAK-D or OAK-1 or overrid with user settings
    ORB_SLAM3::System::eSensor camera_type = ORB_SLAM3::System::RGBD;

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM3::System SLAM(/* ORB Vocab file */, /* Settings file */, camera_type, true);

    // Main loop
    cv::Mat imRGB, imD, imL, imR;
    vector<ORB_SLAM3::IMU::Point>& vImuMeas
    char key_pressed = 'a'
    while (key_pressed != 'q')
    {
        // Pull data from the camera(s)
        if (camera_type == ORB_SLAM3::System::MONOCULAR || camera_type == ORB_SLAM3::System::IMU_MONOCULAR || camera_type == ORB_SLAM3::System::RGBD)
        {
            imRGB = // RGB image
        }
        else if(camera_type == ORB_SLAM3::System::STEREO || camera_type == ORB_SLAM3::System::IMU_STEREO)
        {
            imL = // Left image
            imR = // Right image
        }
        else if(camera_type == ORB_SLAM3::System::RGBD)
        {
            imD = // depth map
        }
        else if(camera_type == ORB_SLAM3::System::IMU_MONOCULAR || camera_type == ORB_SLAM3::System::IMU_STEREO)
        {
            vImuMeas = // imu measurement
        }

        double tframe = // Timestamp of the image(s)

        // Pass the image to the SLAM system
        if (camera_type == ORB_SLAM3::System::MONOCULAR)
            SLAM.TrackMonocular(imRGB, tframe);
        else if(camera_type == ORB_SLAM3::System::STEREO)
            SLAM.TrackStereo(imL, imR, tframe);
        else if(camera_type == ORB_SLAM3::System::RGBD)
            SLAM.TrackRGBD(imRGB, imD, tframe);
        else if(camera_type == ORB_SLAM3::System::IMU_MONOCULAR)
            SLAM.TrackMonocular(imRGB, tframe, vImuMeas);
        else if(camera_type == ORB_SLAM3::System::IMU_STEREO)
            SLAM.TrackStereo(imL, imR, tframe, vImuMeas);
        
        key_pressed = getchar();
    }

    // Stop all threads
    SLAM.Shutdown();

    // Save camera trajectory
    SLAM.SaveTrajectoryTUM("CameraTrajectory.txt");
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");   

    return 0;
}

#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>

#include <opencv2/core/core.hpp>

#include "System.h"
#include "depthai/device.hpp"


// TODO List:
//  * ORB Vocab and Settings file
//  * Command line camera type override
//  * Read image code
//  * Read IMU code
//  * Possible data massaging (e.g. timestamps, depth map size vs RGB image size, etc.)


using namespace std;

int main(int argc, char** argv)
{
    // Setup OAK device
    Device d();
    
    // Detect OAK-D or OAK-1 or override with user settings
    ORB_SLAM3::System::eSensor camera_type = ORB_SLAM3::System::MONOCULAR;
    if (d.is_left_connected() && d.is_right_connected())
        if (!d.is_rgb_connected())
            camera_type = ORB_SLAM3::System::STEREO;
        else
            camera_type = ORB_SLAM3::System::RGBD;

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM3::System SLAM(/* ORB Vocab file */, /* Settings file */, camera_type, true);

    cv::Mat imRGB, imD, imL, imR;
    //vector<ORB_SLAM3::IMU::Point>& vImuMeas
    shared_ptr<CNNHostPipeline> pipeline = d.create_pipeline(/* config */)

    char key_pressed = 'a'
    while (key_pressed != 'q')
    {
        // Pull data from the camera(s)
        tuple<list<shared_ptr<NNetPacket> >,list<shared_ptr<HostDataPacket> > > packet = pipeline->getAvailableNNetAndDataPackets(true)
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
        //else if(camera_type == ORB_SLAM3::System::IMU_MONOCULAR || camera_type == ORB_SLAM3::System::IMU_STEREO)
        //{
        //    vImuMeas = // imu measurement
        //}

        double tframe = chrono::system_clock::now();

        // Pass the image to the SLAM system
        if (camera_type == ORB_SLAM3::System::MONOCULAR)
            SLAM.TrackMonocular(imRGB, tframe);
        else if(camera_type == ORB_SLAM3::System::STEREO)
            SLAM.TrackStereo(imL, imR, tframe);
        else if(camera_type == ORB_SLAM3::System::RGBD)
            SLAM.TrackRGBD(imRGB, imD, tframe);
        //else if(camera_type == ORB_SLAM3::System::IMU_MONOCULAR)
        //    SLAM.TrackMonocular(imRGB, tframe, vImuMeas);
        //else if(camera_type == ORB_SLAM3::System::IMU_STEREO)
        //    SLAM.TrackStereo(imL, imR, tframe, vImuMeas);
        
        key_pressed = getchar();
    }

    // Stop all threads
    SLAM.Shutdown();

    // Save camera trajectory
    SLAM.SaveTrajectoryTUM("CameraTrajectory.txt");
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");   

    return 0;
}

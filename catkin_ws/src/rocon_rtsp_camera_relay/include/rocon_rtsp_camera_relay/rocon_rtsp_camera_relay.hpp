/*
 License: BSD
   https://raw.github.com/robotics-in-concert/rocon_devices/license/LICENSE
*/

#ifndef ROCON_RTSP_CAMERA_RELAY
#define ROCON_RTSP_CAMERA_RELAY

#include <memory>
#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>

#include <std_msgs/String.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <camera_info_manager/camera_info_manager.h>

namespace rocon {

class RoconRtspCameraRelay {
  public:

    RoconRtspCameraRelay(ros::NodeHandle& n);
    ~RoconRtspCameraRelay();

    bool init(const std::string video_stream_url);
    bool reset(const std::string video_stream_url);

    void spin();
  
  protected:
    void convertCvToRosImg(const cv::Mat& mat, sensor_msgs::Image& ros_img);

  private:
    cv::VideoCapture                vcap_;
    std::string                     video_stream_address_;
    std::string                     status_;

    // shared image message
    std::string camera_name_;
    std::string camera_info_url_;

    std::shared_ptr<camera_info_manager::CameraInfoManager> cinfo_;
    sensor_msgs::Image img_;
    image_transport::Publisher pub_video_;

    image_transport::CameraPublisher image_pub_;
    ros::Publisher pub_camera_info_;
    ros::Publisher pub_status_;
    ros::NodeHandle nh_;
    std::string frame_id_;
};
}

#endif

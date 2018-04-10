/*
 License: BSD
   https://raw.github.com/robotics-in-concert/rocon_devices/license/LICENSE
*/

#include <rocon_rtsp_camera_relay/rocon_rtsp_camera_relay.hpp>

namespace rocon {

RoconRtspCameraRelay::RoconRtspCameraRelay(ros::NodeHandle& n) : nh_(n)
{
  image_transport::ImageTransport it(nh_);    

  image_pub_ = it.advertiseCamera("image_raw", 1);
  pub_status_ = nh_.advertise<std_msgs::String>("status", 1);

  // load the camera info
  nh_.param("camera_frame_id", frame_id_, std::string("head_camera"));
  nh_.param("camera_name", camera_name_, std::string("head_camera"));
  nh_.param("camera_info_url", camera_info_url_, std::string(""));

  cinfo_.reset(new camera_info_manager::CameraInfoManager(nh_, camera_name_, camera_info_url_));

}

RoconRtspCameraRelay::~RoconRtspCameraRelay()
{
  vcap_.release();
}

bool RoconRtspCameraRelay::init(const std::string video_stream_url) {
  video_stream_address_ = video_stream_url;

  if (!vcap_.open(video_stream_address_)) 
    return false; 
  else
    return true;
}

bool RoconRtspCameraRelay::reset(const std::string video_stream_url)
{
  vcap_.release();
  return init(video_stream_url);
}

/*
  Convert cv::Mat to sensor_msgs:Image and CameraInfo
 */
void RoconRtspCameraRelay::convertCvToRosImg(const cv::Mat& mat, sensor_msgs::Image& ros_img)
{
  cv_bridge::CvImage cv_img;

  cv_img.encoding = sensor_msgs::image_encodings::BGR8;
  cv_img.image = mat;
  cv_img.toImageMsg(ros_img);
  ros_img.header.stamp = ros::Time::now();
  return;
}


void RoconRtspCameraRelay::spin()
{
  cv::Mat mat;
  std_msgs::String ros_str;

  while(ros::ok())
  {
    if(!vcap_.read(mat)) {
      status_ = "No frame from camera";
      cv::waitKey();
    }
    else {
      status_ = "live";
    }

    ros_str.data = status_;

    sensor_msgs::CameraInfo ci (cinfo_->getCameraInfo());


    ci.header.frame_id = frame_id_;
    
    ci.header.stamp = img_.header.stamp;


    convertCvToRosImg(mat, img_);
    
    img_.header.frame_id = frame_id_;
    img_.header.stamp = ros::Time::now();

    image_pub_.publish(img_, ci);
    pub_status_.publish(ros_str);
    cv::waitKey(1);
  }
}
}

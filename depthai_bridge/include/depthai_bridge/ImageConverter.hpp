#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <unordered_map>

#include "depthai-shared/common/CameraBoardSocket.hpp"
#include "depthai/depthai.hpp"

#ifdef IS_ROS2
    #include "rclcpp/rclcpp.hpp"
    #include "sensor_msgs/msg/camera_info.hpp"
    #include "sensor_msgs/msg/image.hpp"
    #include "std_msgs/msg/header.hpp"
#else
    #include <ros/ros.h>

    #include <boost/make_shared.hpp>
    #include <boost/range/algorithm.hpp>

    #include "sensor_msgs/CameraInfo.h"
    #include "sensor_msgs/Image.h"
    #include "std_msgs/Header.h"
#endif

namespace dai {

namespace ros {

#ifdef IS_ROS2
namespace StdMsgs = std_msgs::msg;
namespace ImageMsgs = sensor_msgs::msg;
using ImagePtr = ImageMsgs::Image::SharedPtr;
#else
namespace StdMsgs = std_msgs;
namespace ImageMsgs = sensor_msgs;
using ImagePtr = ImageMsgs::ImagePtr;
#endif
using TimePoint = std::chrono::time_point<std::chrono::steady_clock, std::chrono::steady_clock::duration>;

class ImageConverter {
   public:
    // ImageConverter() = default;
    ImageConverter(const std::string frameName, bool interleaved);
    ImageConverter(bool interleaved);

    void toRosMsg(std::shared_ptr<dai::ImgFrame> inData, ImageMsgs::Image& outImageMsg);
    ImagePtr toRosMsgPtr(std::shared_ptr<dai::ImgFrame> inData);

    void toDaiMsg(const ImageMsgs::Image& inMsg, dai::ImgFrame& outData);

    /** TODO(sachin): Add support for ros msg to cv mat since we have some
     *  encodings which cv supports but ros doesn't
     **/
    cv::Mat rosMsgtoCvMat(ImageMsgs::Image& inMsg);

    ImageMsgs::CameraInfo calibrationToCameraInfo(dai::CalibrationHandler calibHandler,
                                                  dai::CameraBoardSocket cameraId,
                                                  int width = -1,
                                                  int height = -1,
                                                  Point2f topLeftPixelId = Point2f(),
                                                  Point2f bottomRightPixelId = Point2f());

   private:
    static std::unordered_map<dai::RawImgFrame::Type, std::string> encodingEnumMap;
    static std::unordered_map<dai::RawImgFrame::Type, std::string> planarEncodingEnumMap;

    // dai::RawImgFrame::Type _srcType;
    bool _daiInterleaved;
    // bool c
    const std::string _frameName = "";
    void planarToInterleaved(const std::vector<uint8_t>& srcData, std::vector<uint8_t>& destData, int w, int h, int numPlanes, int bpp);
    void interleavedToPlanar(const std::vector<uint8_t>& srcData, std::vector<uint8_t>& destData, int w, int h, int numPlanes, int bpp);
};

}  // namespace ros

namespace rosBridge = ros;

}  // namespace dai

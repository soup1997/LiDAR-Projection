#ifndef POINTFLOW_ODOMETRY_PFO_HPP
#define POINTFLOW_ODOMETRY_PFO_HPP

#include <iostream>
#include <chrono>
#include <string>
#include <queue>
#include <mutex>

#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/Imu.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/PoseStamped.h>

#include <pcl_ros/transforms.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/conversions.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

//#include <pointflow_odometry/ieskf.hpp>
#include <pointflow_odometry/util.hpp>

class PFO{
    private:
        /*---------ROS Definition---------*/
        ros::Subscriber pcd_sub; // PointCloud2 Subscriber
        ros::Subscriber imu_sub; // Imu Subscriber
        ros::Subscriber img_sub; // Img Subscriber
        ros::Publisher path_pub; // path publisher

        std::string point_cloud_topic;   
        std::string img_topic;
        std::string imu_topic;

        /*---------LiDAR Parameters---------*/
        double _hres, _vres;                // horizontal, vertical resolution
        double _vfov, _vfov_min, _vfov_max; // vertical fov
        double _range;                      // max range
        double _yfudge;                     // yfudge factor

        /*---------IMU Parameters---------*/
        double _acc_std; // acceleration standard deviation
        double _gyro_std; // gyro standard deviation
        double _acc_bias; // acceleration bias
        double _gyro_bias; // gyro bias

        /*---------IMU Variables---------*/
        Eigen::Vector3d _gyro_val;
        Eigen::Vector3d _acc_val;
        double _curr_time;
        double _prev_time;

        /*---------Projected Img Parameters---------*/
        int _xmax, _ymax;
        cv::Mat _stacked_img;
        std::queue<cv::Mat> _img_queue;

        /*---------Pretrained Model Variables---------*/
        torch::jit::Module _model;

        /*---------6-DOF Variables---------*/
        Eigen::Vector3d _translation;
        Eigen::Quaterniond _orientation;
        Eigen::Matrix3d _pose;

    public:
        PFO(ros::NodeHandle nh, ros::NodeHandle private_nh, const std::string model_path);
        ~PFO(){};

        void cloudCallback(const sensor_msgs::PointCloud2::ConstPtr &msg);
        void imgCallback(const sensor_msgs::Image::ConstPtr &msg);
        void imuCallback(const sensor_msgs::Imu::ConstPtr &msg);
        void pathPublisher(const Eigen::Vector3f &translation, const Eigen::Quaternionf &orientation);
        
        void pointCloud2ParnomaicView(const pcl::PointCloud<pcl::PointXYZ> &pcd, const bool &show);
        void stack_image(void);
};

#endif

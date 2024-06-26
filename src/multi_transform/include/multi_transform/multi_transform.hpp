#ifndef MULTI_TRANSFORM
#define MULTI_TRANSFORM

#include <memory>

#include <netinet/in.h>
#include <queue>
#include <rclcpp/node.hpp>
#include <rclcpp/node_options.hpp>
#include <rclcpp/rclcpp.hpp>

#include <pcl/impl/point_types.hpp>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <tf2/LinearMath/Transform.h>
#include <thread>

#include "tf2/transform_datatypes.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include "tf2_eigen/tf2_eigen.hpp"
#include "tf2_ros/transform_listener.h"

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "sensor_msgs/msg/point_cloud2.hpp"
#include "nav_msgs/msg/odometry.hpp"

#define BUFFER_SIZE 1024

namespace multi_transform
{
class MultiTransformNode : public rclcpp::Node
{
public:
  MultiTransformNode(const rclcpp::NodeOptions& options);
  ~MultiTransformNode() override;

private:

  int port;
  std::string ip;
  int sockfd;
  char buffer[BUFFER_SIZE];
  struct sockaddr_in server_addr;

  int robot_id;

  double multiOffsetPositionX;
  double multiOffsetPositionY;
  double multiOffsetPositionZ;
  double multiOffsetRotateR;
  double multiOffsetRotateP;
  double multiOffsetRotateY;

  std::thread send_thread_;
  std::thread recv_thread_;
  std::queue<sensor_msgs::msg::PointCloud2> total_registered_scan_queue;

  std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
  std::shared_ptr<geometry_msgs::msg::TransformStamped> transformStamped;
  std::shared_ptr<Eigen::Matrix4d> fromIdMapToMap;

  void SendTotalRegisteredScan();
  void NetworkThread();

  void TerrainMapCallBack(const sensor_msgs::msg::PointCloud2::ConstSharedPtr terrain_map_msg);
  void TerrainMapExtCallBack(const sensor_msgs::msg::PointCloud2::ConstSharedPtr terrain_map_ext_msg);
  void RegisteredScanCallBack(const sensor_msgs::msg::PointCloud2::ConstSharedPtr registered_scan_msg);
  void StateEstimationAtScanCallBack(const nav_msgs::msg::Odometry::ConstSharedPtr state_estimation_at_scan_msg);
  void WayPointCallBack(const geometry_msgs::msg::PointStamped::ConstSharedPtr way_point_msg);

  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr terrain_map_sub_;
  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr terrain_map_ext_sub_;
  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr registered_scan_sub_;
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr state_estimation_at_scan_sub_;
  rclcpp::Subscription<geometry_msgs::msg::PointStamped>::SharedPtr way_point_sub_;

  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr total_terrain_map_pub_;
  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr total_terrain_map_ext_pub_;
  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr total_registered_scan_pub_;
  rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr total_state_estimation_at_scan_pub_;
  rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr local_way_point_pub_;
};
}  // namespace multi_transform

#endif
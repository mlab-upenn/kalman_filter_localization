#ifndef KFL_EKFL_COMPONENT_H_INCLUDED
#define KFL_EKFL_COMPONENT_H_INCLUDED

#if __cplusplus
extern "C" {
#endif

// The below macros are taken from https://gcc.gnu.org/wiki/Visibility and from
// demos/composition/include/composition/visibility_control.h at https://github.com/ros2/demos
#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define KFL_EKFL_EXPORT __attribute__ ((dllexport))
    #define KFL_EKFL_IMPORT __attribute__ ((dllimport))
  #else
    #define KFL_EKFL_EXPORT __declspec(dllexport)
    #define KFL_EKFL_IMPORT __declspec(dllimport)
  #endif
  #ifdef KFL_EKFL_BUILDING_DLL
    #define KFL_EKFL_PUBLIC KFL_EKFL_EXPORT
  #else
    #define KFL_EKFL_PUBLIC KFL_EKFL_IMPORT
  #endif
  #define KFL_EKFL_PUBLIC_TYPE KFL_EKFL_PUBLIC
  #define KFL_EKFL_LOCAL
#else
  #define KFL_EKFL_EXPORT __attribute__ ((visibility("default")))
  #define KFL_EKFL_IMPORT
  #if __GNUC__ >= 4
    #define KFL_EKFL_PUBLIC __attribute__ ((visibility("default")))
    #define KFL_EKFL_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define KFL_EKFL_PUBLIC
    #define KFL_EKFL_LOCAL
  #endif
  #define KFL_EKFL_PUBLIC_TYPE
#endif

#if __cplusplus
} // extern "C"
#endif

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/transform.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <tf2/transform_datatypes.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2_eigen/tf2_eigen.h>
#include <rclcpp_components/register_node_macro.hpp>

namespace kalman_filter_localization
{
    class EkfLocalizationComponent: public rclcpp::Node
    {
    public:
        KFL_EKFL_PUBLIC
        explicit EkfLocalizationComponent(const rclcpp::NodeOptions & options);
    private:
        std::string initial_pose_topic_;
        std::string imu_topic_;
        std::string odom_topic_;
        std::string gnss_pose_topic_;
        int num_state_;
        int num_error_state_;
        int num_obs_;
        bool initial_pose_recieved_;
        rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr sub_initial_pose_;
        rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr sub_imu_;
        rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr sub_odom_;
        rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr sub_gnss_pose_;
        rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr current_pose_pub_;
        void predictUpdate(const sensor_msgs::msg::Imu input_imu_msg);
        void measurementUpdate(const geometry_msgs::msg::PoseStamped input_pose_msg);
        geometry_msgs::msg::PoseStamped current_pose_;
        Eigen::Matrix4f initial_pose_;
    };
}

#endif  //KFL_EKFL_COMPONENT_H_INCLUDED
#include <iostream>
#include <spdlog/spdlog.h>
#include <rclcpp/rclcpp.hpp>

#include <glim_ros/glim_ros.hpp>
#include <glim/util/config.hpp>
#include <glim/util/extension_module_ros2.hpp>

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  rclcpp::executors::SingleThreadedExecutor exec;
  rclcpp::NodeOptions options;

  auto glim = std::make_shared<glim::GlimROS>(options);

  // Declare parameters before spin
  std::string dump_path = "/tmp/dump";
  glim->declare_parameter<std::string>("dump_path", dump_path);
  glim->get_parameter<std::string>("dump_path", dump_path);

  std::string load_path = "";
  glim->declare_parameter<std::string>("load_path", load_path);
  glim->get_parameter<std::string>("load_path", load_path);

  // Load previous map if load_path is specified
  if (!load_path.empty()) {
    spdlog::info("load_path: {}", load_path);
    if (!glim->load(load_path)) {
      spdlog::error("failed to load map from {}, starting fresh", load_path);
    }
  }

  rclcpp::spin(glim);
  rclcpp::shutdown();

  glim->wait();
  glim->save(dump_path);

  return 0;
}
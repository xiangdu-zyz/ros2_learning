# include "rclcpp/rclcpp.hpp"

int main(int argc ,char **argv)
{
    // 启动节点
    rclcpp::init(argc,argv);
    // 创建节点
    auto node = rclcpp::Node::make_shared("hello_world_node");
    // 输出文本
    RCLCPP_INFO(node->get_logger(),"hello world!");
    //释放资源
    rclcpp::shutdown();
    return 0;
}
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "base_interfaces_demo/msg/student.hpp"

using std::placeholders::_1;
using base_interfaces_demo::msg::Student;

class MinimalSubscriber : public rclcpp::Node
{
    private:
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriber_;

        void topic_callback(const std_msgs::msg::String & msg) const
        {
        RCLCPP_INFO(this->get_logger(), "订阅的消息： '%s'", msg.data.c_str());
        }

    public:
        MinimalSubscriber()
            : Node("MinimalSubscriber")
        {
            subscriber_ = this->create_subscription<std_msgs::msg::String>("topic", 10, std::bind(&MinimalSubscriber::topic_callback, this, _1));
        }
};

class StudentSubscriber : public rclcpp::Node
{
    private:
        rclcpp::Subscription<Student>::SharedPtr subscriber_;

        void topic_callback(const Student & msg) const
        {
            RCLCPP_INFO(
                this->get_logger(),
                "学生的信息：姓名='%s', 年龄=%d, 身高=%.2f",
                msg.name.c_str(), msg.age, msg.height);
        }

    public:
        StudentSubscriber()
            : Node("StudentSubscriber")
        {
            subscriber_ = this->create_subscription<Student>("topic_stu", 10, std::bind(&StudentSubscriber::topic_callback, this, _1));
        }
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<StudentSubscriber>());

    rclcpp::shutdown();

    return 0;
}

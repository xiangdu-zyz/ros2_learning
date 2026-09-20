#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "base_interfaces_demo/msg/student.hpp"

using namespace std::chrono_literals;
using base_interfaces_demo::msg::Student;

class MinimalPublisher : public rclcpp::Node
{
    public:
        MinimalPublisher() : Node("minimal_publisher"), count_(0)
    {
        publisher_ = this->create_publisher<std_msgs::msg::String>("topic",10);
        timer_ = this->create_wall_timer(500ms,std::bind(&MinimalPublisher::timer_callback, this));
    }

    private:
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    size_t count_;
        void timer_callback()
    {
      // 3-3.组织消息并发布。
      auto message = std_msgs::msg::String();
      message.data = "Hello, world! " + std::to_string(count_++);
      RCLCPP_INFO(this->get_logger(), "发布的消息：'%s'", message.data.c_str());
      publisher_->publish(message);
    }

};

class StudentPublisher : public rclcpp::Node
{
    private:
        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Publisher<Student>::SharedPtr publisher_;
        size_t count_;
        void timer_callback()
        {
            auto Stu = Student();
            Stu.name = "张三";
            Stu.age = count_++;
            Stu.height = 1.78;
            RCLCPP_INFO(this->get_logger(), "学生信息:name=%s,age=%d,height=%.2f",Stu.name.c_str(),  Stu.age ,Stu.height);
            publisher_->publish(Stu);

        };

    public:
        StudentPublisher()
            : Node("StudentPublisher"), count_(0)
        {
            publisher_ = this->create_publisher<Student>("topic_stu",10);
            timer_ = this->create_wall_timer(500ms,std::bind(&StudentPublisher::timer_callback, this));
        }
};

int main(int argc,char *argv[])
{
      // 2.初始化 ROS2 客户端；
  rclcpp::init(argc, argv);
  // 4.调用spin函数，并传入节点对象指针。
  rclcpp::spin(std::make_shared<StudentPublisher>());
  // 5.释放资源；
  rclcpp::shutdown();
  return 0;
}

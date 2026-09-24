#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/srv/add_ints.hpp"
#include "base_interfaces_demo/srv/multi.hpp"

using base_interfaces_demo::srv::AddInts;
using base_interfaces_demo::srv::Multi;
using std::placeholders::_1;
using std::placeholders::_2;

class MinimalService:public rclcpp::Node{

    private:
        rclcpp::Service<AddInts>::SharedPtr server;

        void add(const AddInts::Request::SharedPtr req,const AddInts::Response::SharedPtr res){
        res->sum = req->num1 + req->num2;
        RCLCPP_INFO(this->get_logger(),"请求数据:(%d,%d),响应结果:%d", req->num1, req->num2, res->sum);
        }
    public:
        MinimalService()
            :rclcpp::Node("Minimal_service")
            {
                server = this->create_service<AddInts>("add_ints",std::bind(&MinimalService::add, this, _1, _2));
                RCLCPP_INFO(this->get_logger(),"add_ints 服务端启动完毕，等待请求提交...");
            }
};

class MultiService : public rclcpp::Node
{
    private:
        rclcpp::Service<Multi>::SharedPtr server;

        void multi(const Multi::Request::SharedPtr req , const Multi::Response::SharedPtr res)
        {
            res->result = req->num1*req->num2;
            RCLCPP_INFO(this->get_logger(),"请求数据：%d,%d   返回数据：%d",req->num1,req->num2,res->result);
        }

    public:
        MultiService()
            :rclcpp::Node("MultiService_topic")
            {
                server = this->create_service<Multi>("multi",std::bind(&MultiService::multi,this,_1,_2));
            }
};
int main(int argc, char const *argv[])
{
    rclcpp::init(argc,argv);

    auto server = std::make_shared<MultiService>();
    rclcpp::spin(server);

    rclcpp::shutdown();
    return 0;
}
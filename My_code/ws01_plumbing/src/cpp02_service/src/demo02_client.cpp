#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/srv/add_ints.hpp"
#include "base_interfaces_demo/srv/multi.hpp"

#include <cstdlib>

using base_interfaces_demo::srv::AddInts;
using base_interfaces_demo::srv::Multi;
using namespace std::chrono_literals;

class MinimalClient : public rclcpp::Node
{
    private:
        rclcpp::Client<AddInts>::SharedPtr client;

    public:
        MinimalClient()
            :Node("MinimalClient")
        {
            client = this->create_client<AddInts>("add_ints");
            RCLCPP_INFO(this->get_logger(),"客户端创建，等待连接服务端！");
        }
          // 3-2.等待服务连接；
        bool connect_server()
        {
            while (!client->wait_for_service(1s))
            {
                if (!rclcpp::ok())
                {
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"强制退出！");
                return false;
                }

                RCLCPP_INFO(this->get_logger(),"服务连接中，请稍候...");
            }
            return true;
        }
        rclcpp::Client<AddInts>::FutureAndRequestId send_request(int32_t num1, int32_t num2)
        {
            auto request = std::make_shared<AddInts::Request>();
            request->num1 = num1;
            request->num2 = num2;
            return client->async_send_request(request);
        }
};

class MultiClient : public rclcpp::Node
{
    private:
        rclcpp::Client<Multi>::SharedPtr client;


    public:
        MultiClient()
            :Node("Multiclient_topic")
        {
            client = this->create_client<Multi>("multi");
            RCLCPP_INFO(get_logger(),"客户端建立");
        }
        bool connect_server()
        {
            while(!client->wait_for_service(1s))
            {
                if(!rclcpp::ok())
                {
                    RCLCPP_INFO(get_logger(),"连接失败！");
                    return false;
                }
                RCLCPP_INFO(get_logger(),"等待连接---");
            }
            return true;

        }

        rclcpp::Client<Multi>::FutureAndRequestId send_request(int num1,int num2)
        {
            auto request = std::make_shared<Multi::Request>();
            request->num1 = num1;
            request->num2 = num2;

            return client->async_send_request(request);
        }

};
int main(int argc, char const *argv[])
{

    if (argc != 3){
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"请提交两个整型数据！");
    return 1;
    }

    rclcpp::init(argc,argv);

    auto client = std::make_shared<MultiClient>();
    bool flag = client->connect_server();

    if(!flag)
    {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"服务连接失败");
        return 0;
    }

    auto future = client->send_request(std::atoi(argv[1]), std::atoi(argv[2]));

    if (rclcpp::spin_until_future_complete(client,future) == rclcpp::FutureReturnCode::SUCCESS)
    {
        auto response = future.get();

        RCLCPP_INFO(client->get_logger(),"请求正常处理");
        RCLCPP_INFO(client->get_logger(),"响应结果:%d!", response->result);

    } else {
        RCLCPP_INFO(client->get_logger(),"请求异常");
    }

    rclcpp::shutdown();


    return 0;
}

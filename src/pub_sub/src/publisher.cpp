#include <chrono> // waktu-waktu-an
#include <memory> // smart-pointer
#include <string>

// dependencies tambahan 
#include "rclcpp/rclcpp.hpp"
#include "custom_msgs/msg/controller.hpp"

using namespace std::chrono_literals;

auto topic = "custom_message_topic";

class PublisherNode : public rclcpp::Node {
public:
    PublisherNode() : Node("publisher_node") {
        pub = this->create_publisher<custom_msgs::msg::Controller>(topic, 10);

        period = 5000ms;
        timer = this->create_wall_timer(
            period, 
            std::bind(&PublisherNode::callback, this)
        );
    }

private:
    void callback() {
        auto msg = custom_msgs::msg::Controller();

        msg.velx = 1.5f;
        msg.vely = 0.0f;   
        msg.angvel = 0.5f;

        pub->publish(msg);

        RCLCPP_INFO(this->get_logger(), "Publishing: velx=%.2f, vely=%.2f, angvel=%.2f", msg.velx, msg.vely, msg.angvel);
    }
    
    rclcpp::Publisher<custom_msgs::msg::Controller>::SharedPtr pub;
    custom_msgs::msg::Controller controller_msg;
    
    rclcpp::TimerBase::SharedPtr timer;
    std::chrono::milliseconds period;
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv); // init, biar bisa makai rclcpp api

    auto publisher_node = std::make_shared<PublisherNode>(); // objek-kan class publisher

    rclcpp::spin(publisher_node);

    rclcpp::shutdown(); // cleaning up, nodes, sm global resources
    // biasanya ke-trigger kalau user menekan ctrl+c

    return 0;
}
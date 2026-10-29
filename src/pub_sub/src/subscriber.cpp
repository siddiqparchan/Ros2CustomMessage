#include <memory> // smart-pointer

#include "rclcpp/rclcpp.hpp"
#include "custom_msgs/msg/controller.hpp"

using namespace std::placeholders;

auto topic = "custom_message_topic";

class SubscriberNode : public rclcpp::Node {
public:
    SubscriberNode() : Node("subscriber_node") {
        sub = this->create_subscription<custom_msgs::msg::Controller>(
            topic, 
            10, 
            std::bind(&SubscriberNode::callback, this, _1)
        );
    }
private:
    void callback(const custom_msgs::msg::Controller::SharedPtr msg) {
        float save[3] = {msg->velx, msg->vely, msg->angvel};
        RCLCPP_INFO(this->get_logger(), "Received: velx=%.2f, vely=%.2f, angvel=%.2f", save[0], save[1], save[2]);
    }

    rclcpp::Subscription<custom_msgs::msg::Controller>::SharedPtr sub;
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv); // init, biar bisa makai rclcpp api

    auto subscriber_node = std::make_shared<SubscriberNode>(); 

    rclcpp::spin(subscriber_node);

    rclcpp::shutdown(); // cleaning up, nodes, sm global resources
    // biasanya ke-trigger kalau user menekan ctrl+c

    return 0;
}
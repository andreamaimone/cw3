#include "ros/ros.h"
#include "sin_pkg/my_msg.h"
#include "math.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv){

ros::init(argc, argv, "publisher");
ros::NodeHandle n;
ros::Publisher topic_pub = n.advertise<sin_pkg::my_msg>("/sinu", 1);

ros::Rate rate(100);

//create the custom data type
sin_pkg::my_msg data;

    cout << "Insert amplitude: " << endl;
    cin >> data.amp;
    cout << "Insert period: " << endl;
    cin >> data.T;

float count = 0;

    while(ros::ok()) {

        data.value = data.amp*sin(2*3.14*(1/data.T)*count);

        ROS_INFO("sin: %.3f\t time: %.2f", data.value, count);

        topic_pub.publish(data);

        count += 1.0/100.0;
        
        rate.sleep(); //object rate I created before
    }

return 0;
}
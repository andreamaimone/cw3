#include "ros/ros.h"
#include "sin_pkg/my_msg.h"
#include "boost/thread.hpp"
#include "sin_pkg/lpf.h"

using namespace std;

//Use a class to store the topic data 
//	Just a choice: the best way to share data between a main loop and the ROS callbacks
class ROS_SUB {
	public:
		ROS_SUB();
		void topic_cb( sin_pkg::my_msg data); //receives the published datas
		void filter();

	private:
		float _v;
		ros::NodeHandle _nh;
        ros::Publisher topic_lpf;
        float _lpf_b;
		//Subscriber object
		ros::Subscriber topic_sub;
};

ROS_SUB::ROS_SUB() {
	_v=0.0;
    topic_lpf = _nh.advertise<sin_pkg::lpf>("/lpf", 1); //i create the lpf publisher (i only puclish value)
    _lpf_b = 1/(1+0.01);
	boost::thread loop_t(&ROS_SUB::filter, this);  //thread for the filter
	//Initialize a subscriber:
	//	Input: 	topic name: /numbers
	//			queue:	1
	//			Callback function
	//			Object context: the value of data members
	topic_sub = _nh.subscribe("/sinu", 1, &ROS_SUB::topic_cb, this); //this is my subscriber
}

//Callback function: the input of the function is the data to read
void ROS_SUB::topic_cb(sin_pkg::my_msg d) {	//takes the sinusoid value and puts it in v

	_v = d.value;

    }

void ROS_SUB::filter(){
	sin_pkg::lpf out;
	out.value=0;
	float count = 0;
	while(ros::ok()){
	out.value = _lpf_b*out.value + (1 - _lpf_b)*_v;
	count += 1.0/100.0;
	ROS_INFO("sin: %.3f\t time: %.2f", out.value, count);
	topic_lpf.publish(out);
	usleep(10000);

	}

}

int main( int argc, char** argv ) {

	//Init the ros node with ros_subscriber name
	ros::init(argc, argv, "lpf_topic");

	//Create the ROS_SUB class object
	ROS_SUB rs;
	
	//ros::spin() blocks the main thread from exiting until ROS invokes a shutdown - via a Ctrl + C for example
	// It is written as the last line of code of the main thread of the program.
	//Also the spin invokes the callbacks to flush their queue and process incoming data
	ros::spin(); 

	//----This function will be never overcome

	return 0;
}
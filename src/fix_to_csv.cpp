#include <ros/ros.h>
#include <sensor_msgs/NavSatFix.h>
#include <iostream>
#include <fstream>

// FILE *fptr;
std::ofstream fptr;
int num_rows = 1;
std::string linestring("\"LINESTRING (");

void fixCallback(const sensor_msgs::NavSatFix::ConstPtr &msg)
{
    std::string longitude = std::to_string(msg->longitude);
    std::string latitude =  std::to_string(msg->latitude);

    ROS_INFO("Line %d", num_rows);
    ROS_INFO("Longitude %s", longitude);
    ROS_INFO("Latitude %s", latitude);

    if (num_rows <= 2000)
    {
        fptr << "Point " << num_rows << ",";

        fptr << "\"POINT (" << longitude << " " << latitude << ")\"\n";

        linestring += longitude + " " + latitude + ", ";

        num_rows++;
    }
    else
        ROS_WARN("Number of rows written exceded!");
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "fix_to_csv");
    ros::NodeHandle nh;

    std::string csv_path;
    std::string fix_topic;

    nh.getParam("output_file", csv_path);
    nh.getParam("fix_topic", fix_topic);

    // num_rows = 1;

    fptr.open(csv_path);

    fptr << "Name,WKT\n";
    ros::Subscriber sub = nh.subscribe(fix_topic, 1000, fixCallback);

    while (ros::ok())
        ros::spinOnce();

    fptr << "Line " << num_rows << ",";
    linestring.erase(linestring.length()-2, linestring.length()-1);
    linestring += ")\"";
    fptr << linestring;

    fptr.close();

    return 0;
}